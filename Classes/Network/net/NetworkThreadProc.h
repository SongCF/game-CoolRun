#pragma once

#include "ODSocket.h"
#include "protocol.h"
#include "adler32.h"
#include <queue>
#include <thread>
#include <mutex>


//1. 网络请求的消息包
class RequestPackage
{
private:
	char *m_data;
	int m_length;
public:
	RequestPackage(MessageCmd type, const char* body, int bodyLength)
	{
		SPHead* pHead = RequestPackage::newHead(type, bodyLength);

		m_length = sizeof(SPHead) + bodyLength + sizeof(unsigned int); //最后4字节校验位
		unsigned int adler = htonl(Adler::adler32(1, (const Bytef*)body, bodyLength));
		int headLength = sizeof(SPHead);

		m_data = new char[m_length];
		memcpy(m_data, pHead, headLength);
		memcpy(m_data+headLength, body, bodyLength);
		memcpy(m_data+headLength+bodyLength, &adler, sizeof(unsigned int));

		delete pHead;
	}
	~RequestPackage()
	{
		delete [] m_data;
	}

	char* getData(){return m_data;}
	int getDataLength(){return m_length;}

	static SPHead* newHead(MessageCmd type, int dataLength) //传入body数据的size
	{
		SPHead *p = new SPHead;
		p->protocol_tag = SimpleProtocol_Tag_Client;
		p->protocol_ver = Current_ProtocolVersion;
		p->cmd			= type;
		p->data_len		= dataLength + sizeof(unsigned int); // 校验码
		hton_SPHead(p);
		return p;
	}
};


//2. 网络回调的消息包
class RespondPackage
{
public:	
	enum PackageType
	{
		Type_None = 0,
		Type_Conncet_Suc,
		Type_Conncet_Err,
		Type_Send_Err,
		Type_Recv_Err,
		Type_Recv_OK,		// 只有这种情况RespondPackage才有数据
	};

private:
	PackageType m_type;
	char* m_head;
	char* m_body;
public:
	//返回包数据，在返回包对象生命结束的时候delete (这里的数据是包含校验码的)
	RespondPackage(PackageType type ,char* head=nullptr, char* body=nullptr)
	{
		m_type = type;
		m_head = head;
		if (head) ntoh_SPHead((SPHead*)m_head);
		m_body = body;
	}
	~RespondPackage()
	{
		if(m_head) delete [] m_head;
		if(m_body) delete [] m_body;
	}
	PackageType getType(){return m_type;}
	char* getHead(){return m_head;}
	char* getBody(){return m_body;}
};


//3. network process
//singleton
class NetworkThreadProc
{
public:
	enum NetworkStat
	{
		NetworkStat_NotConnect = 1,
		NetworkStat_Connected  = 2,
	};
private:
	NetworkThreadProc();
	static NetworkThreadProc * _networkThreadProc;
	void init();
public:
	~NetworkThreadProc();
	static NetworkThreadProc* getInstance();

	//send data
	void sendData(MessageCmd type, const char* msg_body, int body_length);
	//recv dispatch data
	void addRspPackage(RespondPackage* rsp);
	RespondPackage* getRecvData(); //返回的数据不为空时，需要调用者delete

	void closeNetwork();

protected:
	void recvThread();
	void sendThread();

	void sleep(long millisecond);
	bool connectToServer();
	void closeConnection();

private:
	ODSocket m_socket;
	NetworkStat m_networkState;

	std::thread m_threadSend;
	std::queue<RequestPackage*> m_sendBuf;
	std::mutex m_mutexSendBuf;
	std::condition_variable   m_sendBufCondVar;//只能等待unique_lock<mutex>的条件变量

	std::thread m_threadRecv;
	std::queue<RespondPackage*> m_recvBuf;
	std::mutex m_mutexRecvBuf;
	std::condition_variable   m_recvBufCondVar;
};