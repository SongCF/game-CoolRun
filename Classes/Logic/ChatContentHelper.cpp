
#include "ChatContenthelper.h"
#include "cocos2d.h"

#include <string>

using std::string;
using namespace cocos2d;

#define MAX_SEASON 1
#define MAX_FIGHT 6


#define FILE_NAME "config/ChatContent.txt"

#define KEY_Guide "guide"
#define KEY_Fight_Format "fightEnd_%d"
#define KEY_Season_Format "section_%d_%d"



ChatContentHelper* ChatContentHelper::_chatContentInstance = nullptr;

ChatContentHelper* ChatContentHelper::getInstance()
{
	if (!_chatContentInstance){
		_chatContentInstance = new ChatContentHelper();
		//_chatContentInstance->initData();
	}
	return _chatContentInstance;
}

ChatContentHelper::ChatContentHelper()
	:m_pJsonRoot(nullptr)
{

}

ChatContentHelper::~ChatContentHelper()
{
	if (m_pJsonRoot) delete m_pJsonRoot;
	m_pJsonRoot = nullptr;
}

//聊天文本的数据量太多，而已用到的太少，那么就不要再init中读入所有数据，而是每次取得时候重新获取
#if 0
bool ChatContentHelper::initData()
{
	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_NAME;
	Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
	if (dat.getSize() <= 0)
	{
		// no file 
		CCLOG("%s ---> no file [%s]", __FUNCTION__, filePath.c_str());
		return false;
	}
	char *buf = new char[dat.getSize()+1];
	strncpy(buf, (char*)dat.getBytes(), dat.getSize());
	buf[dat.getSize()] = 0;
	CCLOG("%s ---> %s\n", __FUNCTION__, buf);

	rapidjson::Document d;
	d.Parse<0>(buf);
	delete [] buf;
	if (d.HasParseError())  //解析错误
	{
		CCLOG("%s ---> GetParseError %s\n", __FUNCTION__, d.GetParseError());
		return false;
	}
	else if (d.IsObject()) 
	{
// 		if (d.HasMember(KEY_Root) && d[KEY_Root].IsArray())
// 		{
// 			const rapidjson::Value& arr = d[KEY_Root];
// 			for (int i=0,count=arr.Size(); i<count; ++i)
// 			{
// 				Email temp;
// 				if (arr[i].HasMember(KEY_Id)) temp.id = arr[i][KEY_Id].GetUint();
// 				if (arr[i].HasMember(KEY_Type)) temp.type = (Email::EmailType)arr[i][KEY_Type].GetUint();
// 				if (arr[i].HasMember(KEY_Content)) temp.content = arr[i][KEY_Content].GetString();
// 				if (arr[i].HasMember(KEY_BonusId)) temp.bonus_unitID = arr[i][KEY_BonusId].GetUint();
// 				if (arr[i].HasMember(KEY_BonusCount)) temp.bonusCount = arr[i][KEY_BonusCount].GetUint();
// 				if (arr[i].HasMember(KEY_IsRead)) temp.isAlreadyRead = arr[i][KEY_IsRead].GetBool();
// 				if (arr[i].HasMember(KEY_RecvTime)) temp.recvTime = arr[i][KEY_RecvTime].GetInt();
// 				m_vecEmailList.push_back(temp);
// 			}
// 			return true;
// 		}
	}

	return false;
}
#endif

bool ChatContentHelper::loadFile()
{
	string filePath = FileUtils::getInstance()->fullPathForFilename(FILE_NAME);
	Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
	if (dat.getSize() <= 0)
	{
		// no file 
		CCLOG("%s ---> no file [%s]", __FUNCTION__, filePath.c_str());
		return false;
	}
	char *buf = new char[dat.getSize()+1];
	strncpy(buf, (char*)dat.getBytes(), dat.getSize());
	buf[dat.getSize()] = 0;
	CCLOG("%s ---> %s\n", __FUNCTION__, buf);

	m_pJsonRoot = new rapidjson::Document();
	m_pJsonRoot->Parse<0>(buf);
	delete [] buf;
	if (m_pJsonRoot->HasParseError())  //解析错误
	{
		CCLOG("%s ---> GetParseError %s\n", __FUNCTION__, m_pJsonRoot->GetParseError());
		freeFile();
		return false;
	}
	else if (m_pJsonRoot->IsObject()) 
	{
		return true;
	}

	freeFile();
	return false;
}
void ChatContentHelper::freeFile()
{
	if (m_pJsonRoot) delete m_pJsonRoot;
	m_pJsonRoot = nullptr;
}
bool ChatContentHelper::parseContent(ChatIdx idx, int id, vector<string>* _out)
{
	string rootKet;
	if (idx == eChatGuide)
	{
		rootKet = KEY_Guide;
	}
	else if (idx >= eChatFightEnd1 && idx < eChatFightEnd1+MAX_FIGHT)
	{
		rootKet = String::createWithFormat(KEY_Fight_Format, idx-eChatFightEnd1+1)->getCString();
	}
	else if (idx >= eChatSeason1 && idx < eChatSeason1 + MAX_SEASON)
	{
		rootKet = String::createWithFormat(KEY_Season_Format, idx-eChatSeason1+1, id)->getCString();
	}
	else
	{
		CCASSERT(false, "error idx");
	}


	if (m_pJsonRoot->HasMember(rootKet.c_str()) && (*m_pJsonRoot)[rootKet.c_str()].IsArray())
	{
		const rapidjson::Value& arr = (*m_pJsonRoot)[rootKet.c_str()];
		for (int i=0,count=arr.Size(); i<count; ++i)
		{
			_out->push_back(arr[i].GetString());
		}
		return true;
	}
	return false;
}
vector<string> ChatContentHelper::getChatContent(ChatIdx idx, int id)
{
	vector<string> ret;
	bool err;
	//1. 解析
	err = loadFile();
	if (!err){
		return ret;
	}
	//2.获取
	err = parseContent(idx,id,&ret);
	//3.释放
	freeFile();

	//
	if (!err){
		ret.clear();
		return ret;
	}else {
		return ret;
	}
}