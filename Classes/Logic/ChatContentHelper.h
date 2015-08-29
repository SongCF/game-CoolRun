//
//剧情对话
//对话内容管理类
//

#pragma once


#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

class ChatContentHelper
{
protected:
	ChatContentHelper();
	static ChatContentHelper* _chatContentInstance;
	bool initData();
public:
	enum ChatIdx
	{
		eChatGuide = 1,
		eChatFightEnd1 = 11, //11-16 fight 
		eChatSeason1 = 101,
		//eChatSeason2 ---> use: eChatSeason1+1
	};
	static ChatContentHelper* getInstance();
	~ChatContentHelper();

	/*
	* ChatIdx idx : 对话类型，教学的?season1的?season2的?
	* int id      : 如果是season对话，那么传入season中section关卡号
	*/
	vector<string> getChatContent(ChatIdx idx, int id=0);
protected:
	bool loadFile();
	void freeFile();
	bool parseContent(ChatIdx idx, int id, vector<string>* _out);

private:
	rapidjson::Document* m_pJsonRoot;
};