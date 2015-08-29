//============================================================
//* 邮件帮助类
//* 逻辑层
//* 所有邮件操作
//============================================================

#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;


class Email
{
public:
	enum EmailType
	{
		ET_Invalid = 0,
		ET_SystemBonus,
		ET_SystemNotice,
		ET_FriendChat,
	};

	//
	unsigned int id;
	EmailType type;
	string content;
	unsigned int bonus_unitID;
	unsigned int bonusCount;
	bool isAlreadyRead;
	int recvTime; //收到邮件的时间

	Email()
	{
		id = 0;
		type = ET_Invalid;
		bonus_unitID = 0;
		bonusCount = 0;
		isAlreadyRead = false;
		recvTime = 0;
	}
	const Email& operator=(const Email& in)
	{
		this->id = in.id;
		this->type = in.type;
		this->content = in.content;
		this->bonus_unitID = in.bonus_unitID;
		this->bonusCount = in.bonusCount;
		this->isAlreadyRead = in.isAlreadyRead;
		this->recvTime = in.recvTime;
		return *this;
	}
};

class EmailHelper
{
	EmailHelper();
	static EmailHelper* m_sEmailHelper;
	bool initData();
	bool saveData();  
public:
	static EmailHelper* getHeper();
	void save();//用户退出邮件界面后  保存一次

	vector<Email> getEmailList(Email::EmailType type, bool includeAlreadyRead = false);
	vector<Email> getAllEmailList(int num = 0);// 0代表所有， 其余的表示前多少封， 30表示取前30封

	//最新的排在最前面
	void sortEmailByTime(vector<Email>& list);

	void setOneEmailAlreadyRead(unsigned int emailId);
	void addEmail(Email em);
	void removeEmail(unsigned int emailId);
	bool isUnreadmessages();
	
private:
	vector<Email> m_vecEmailList;
};