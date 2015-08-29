#include "NetworkThreadProc.h"
#include "cocos2d.h"
#include "TimeoutMonitor.h"

using namespace cocos2d;


#define DEFAULT_SERVER_IP		"192.168.2.10"
#define DEFAULT_SERVER_PORT		23577
#define SEND_TIME_OUT	2000	// 发送超时
#define RECV_TIME_OUT	3600000	// 接收超时


NetworkThreadProc * NetworkThreadProc::_networkThreadProc = nullptr;

NetworkThreadProc::NetworkThreadProc()
{

}

NetworkThreadProc::~NetworkThreadProc()
{

}

NetworkThreadProc* NetworkThreadProc::getInstance()
{
	if (! _networkThreadProc){
		_networkThreadProc = new NetworkThreadProc;
		_networkThreadProc->init();
	}
	return _networkThreadProc;
}

void NetworkThreadProc::init()
{
	m_networkState = NetworkThreadProc::NetworkStat_NotConnect;
	m_threadSend = std::thread(&NetworkThreadProc::sendThread, this);
	m_threadRecv = std::thread(&NetworkThreadProc::recvThread, this);
}
	//send data
void NetworkThreadProc::sendData(MessageCmd type, const char* msg_body, int body_length)
{
	//组包
	RequestPackage *pack = new RequestPackage(type, msg_body, body_length);

	//阻塞直到获得锁。
	std::unique_lock<std::mutex> lock(m_mutexSendBuf);
	m_sendBuf.push(pack); //pop之后delete
	m_sendBufCondVar.notify_one();

	//超时监视
	TimeoutMonitor::getInstance()->startMonitor();
}

	//recv dispatch data
RespondPackage* NetworkThreadProc::getRecvData()
{
	//尝试获得引用的互斥锁，未能获得也不阻塞。
	std::unique_lock<std::mutex> lock(m_mutexRecvBuf, std::try_to_lock);

	if (lock.owns_lock() && m_recvBuf.size()>0)
	{
		RespondPackage *p = m_recvBuf.front();
		m_recvBuf.pop();
		return p;
	}
	return nullptr;
}

void NetworkThreadProc::addRspPackage(RespondPackage* rsp)
{
	std::unique_lock<std::mutex> lock(m_mutexRecvBuf);
	m_recvBuf.push(rsp);
}




void NetworkThreadProc::sleep(long millisecond)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	Sleep(millisecond);
#else
	usleep(millisecond*1000);
#endif
}

bool NetworkThreadProc::connectToServer()
{
	if (m_networkState == NetworkStat_Connected)
		return true;

	if (m_socket.Init() == -1)
	{
		CCLOG("%s ---> init error", __FUNCTION__);
		return false;
	}
	if (! m_socket.Create(AF_INET,SOCK_STREAM,0))
	{
		CCLOG("%s ---> create error, err code = %d", __FUNCTION__, m_socket.GetError());
		return false;
	}
	if (! m_socket.Connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT))
	{
		CCLOG("%s ---> connect error [%s-%d], err code = %d", __FUNCTION__, DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, m_socket.GetError());
		closeConnection();
		return false;
	}
	CCLOG("%s ---> connect seccess [%s-%d]", __FUNCTION__, DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);

	//设置状态
	m_networkState = NetworkStat_Connected;

	// 设置超时
	m_socket.SetRecvTimeOut(RECV_TIME_OUT);
	m_socket.SetSendTimeOut(SEND_TIME_OUT);

	return true;
}

void NetworkThreadProc::closeNetwork()
{
	closeConnection();
}
void NetworkThreadProc::closeConnection()
{
	if (m_socket.Close() != 0)
	{
		CCLOG("%s ---> Close Error,errCode = %d", __FUNCTION__, m_socket.GetError());
	}
	if (m_socket.Clean() != 0)
	{
		CCLOG("%s ---> Clean Error,errCode = %d", __FUNCTION__, m_socket.GetError());
	}
	//
	m_networkState = NetworkStat_NotConnect;
	CCLOG(" -----Network closed-----");

	int count = 0;
	RequestPackage *temp;
	std::unique_lock<std::mutex> lock(m_mutexSendBuf);
	while(! m_sendBuf.empty())
	{
		++count;
		temp = m_sendBuf.front();
		m_sendBuf.pop();
		delete temp;
	}
	CCLOG("%s --> throwed %d request packege(s)", __FUNCTION__, count);
}

//短链接
void NetworkThreadProc::sendThread()
{
	while (true)
	{
		RequestPackage *msg = nullptr;

		//1. 是否有数据需要发送
		{
			std::unique_lock<std::mutex> lock(m_mutexSendBuf);
			if (m_sendBuf.empty())
			{
				CCLOG("%s ---> waiting send buffer", __FUNCTION__);
				//1.先释放lock  2.然后等待被唤醒  3.被唤醒后等待获取lock
				m_sendBufCondVar.wait(lock);
			}
			msg = m_sendBuf.front();
			m_sendBuf.pop();
		}

		//2. 检测网络连接, 没连上则连接
		if (NetworkThreadProc::NetworkStat_NotConnect == m_networkState)
		{
			if (! connectToServer())
			{
				CCLOG("%s ---> connect to server Fail", __FUNCTION__);
				addRspPackage(new RespondPackage(RespondPackage::Type_Conncet_Err));
				delete msg;
				sleep(2000);
				continue;
			}
			// 通知链接成功
			addRspPackage(new RespondPackage(RespondPackage::Type_Conncet_Suc));
			m_recvBufCondVar.notify_one();
			CCLOG("%s ---> connect to server OK", __FUNCTION__);
		}

		//3. send data
		int len = m_socket.Send(msg->getData(), msg->getDataLength(), 0);
		if (len != msg->getDataLength())
		{
			CCLOG("%s ---> send data error, real size = %d, send size = %d", __FUNCTION__, msg->getDataLength(), len);
			// 通知发送数据出错
			addRspPackage(new RespondPackage(RespondPackage::Type_Send_Err));
			delete msg;
			continue;
		}
		else
		{
			CCLOG("%s ---> send data success", __FUNCTION__);
			delete msg;
			continue;
		}
	}
}
void NetworkThreadProc::recvThread()
{
	while (true)
	{
		if (NetworkThreadProc::NetworkStat_Connected == m_networkState)
		{
			CCLOG("%s ---> recv data .....", __FUNCTION__);
			//1. 读取消息头
			char* head = new char[sizeof(SPHead)];
			int len = m_socket.Recv(head,sizeof(SPHead),0);
			if ( len != sizeof(SPHead))
			{
				CCLOG("%s ---> recv Head error , total length = %d, readed length = %d", __FUNCTION__, sizeof(SPHead), len);
				addRspPackage(new RespondPackage(RespondPackage::Type_Recv_Err));
				// 出问题了
				closeConnection();
				delete head;
				continue;
			}
			//2. 检测消息头
			if (! isValiedServerHead((SPHead*)head))
			{
				CCLOG("%s ---> recv Head is invalied", __FUNCTION__);
				addRspPackage(new RespondPackage(RespondPackage::Type_Recv_Err));
				// 出问题了
				closeConnection();
				delete head;
				continue;
			}
			CCLOG("%s ---> recv head success!", __FUNCTION__);

			//3. 获取数据包
			int data_len = ntohl(((SPHead*)head)->data_len);
			char* body = new char[data_len];

			int len_body = m_socket.Recv(body, data_len, 0);
			if ( len_body != data_len)
			{
				CCLOG("%s ---> recv body error , total length = %d, readed length = %d", __FUNCTION__, data_len, len_body);
				addRspPackage(new RespondPackage(RespondPackage::Type_Recv_Err));
				// 出问题了
				closeConnection();
				delete head;
				delete body;
				continue;
			}
			CCLOG("%s ---> recv body success!", __FUNCTION__);

			//4. 校验数据包
			unsigned int recvAdler;
			memcpy(&recvAdler, body+len_body-sizeof(unsigned int), sizeof(unsigned int));
			recvAdler = ntohl(recvAdler);
			unsigned int adler = Adler::adler32(1, (const Bytef*)body, len_body-sizeof(unsigned int));
			if (adler != recvAdler)
			{
				CCLOG("%s ---> recv body error, adler code error", __FUNCTION__);
				addRspPackage(new RespondPackage(RespondPackage::Type_Recv_Err));
				// 出问题了
				closeConnection();
				delete head;
				delete body;
				continue;
			}

			//5. 通知接收数据成功
			addRspPackage(new RespondPackage(RespondPackage::Type_Recv_OK, head, body));
			closeConnection(); //短链接
		}
		else
		{
			std::unique_lock<std::mutex> lock(m_mutexRecvBuf);
			m_recvBufCondVar.wait(lock);
			continue;
		}
	}
}