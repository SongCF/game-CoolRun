#include "EmailHelper.h"
#include "cocos2d.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "NetmsgManger.h"

using namespace cocos2d;


#define FILE_Name "EmailInfo.txt"
#define KEY_Root "EmailList"
#define KEY_Id			"id"
#define KEY_Type		"type"
#define KEY_Content		"content"
#define KEY_BonusId		"bonus_unitID"
#define KEY_BonusCount  "bonusCount"
#define KEY_IsRead		"isAlreadyRead"
#define KEY_RecvTime	"recvTime"
// {
// 	"EmailList":[
// 		{
// 			"id":1,
// 			"type":1,
// 			"content":"string",
// 			"bonus_unitID":99,
// 			"bonusCount":10,
// 			"isAlreadyRead":false,
// 			"recvTime":11111
// 		}
// 	]
// }



EmailHelper* EmailHelper::m_sEmailHelper = nullptr;

EmailHelper* EmailHelper::getHeper()
{
	if (!m_sEmailHelper )
	{
		m_sEmailHelper = new EmailHelper();
		m_sEmailHelper->initData();
	}
	return m_sEmailHelper;
}

EmailHelper::EmailHelper()
{

}

bool EmailHelper::initData()
{
	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_Name;
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
		if (d.HasMember(KEY_Root) && d[KEY_Root].IsArray()/* && d[KEY_Root].Size() > 0*/)
		{
			const rapidjson::Value& arr = d[KEY_Root];
			for (int i=0,count=arr.Size(); i<count; ++i)
			{
				Email temp;
				if (arr[i].HasMember(KEY_Id)) temp.id = arr[i][KEY_Id].GetUint();
				if (arr[i].HasMember(KEY_Type)) temp.type = (Email::EmailType)arr[i][KEY_Type].GetUint();
				if (arr[i].HasMember(KEY_Content)) temp.content = arr[i][KEY_Content].GetString();
				if (arr[i].HasMember(KEY_BonusId)) temp.bonus_unitID = arr[i][KEY_BonusId].GetUint();
				if (arr[i].HasMember(KEY_BonusCount)) temp.bonusCount = arr[i][KEY_BonusCount].GetUint();
				if (arr[i].HasMember(KEY_IsRead)) temp.isAlreadyRead = arr[i][KEY_IsRead].GetBool();
				if (arr[i].HasMember(KEY_RecvTime)) temp.recvTime = arr[i][KEY_RecvTime].GetInt();
				m_vecEmailList.push_back(temp);
			}
			return true;
		}
	}

	return false;
}

bool EmailHelper::saveData()
{
	rapidjson::Document document;
	document.SetObject(); //Set this value as an empty object.
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//value
	rapidjson::Value emailArr(rapidjson::kArrayType);
	for (int i=0,count=m_vecEmailList.size(); i<count; ++i)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember<unsigned int>(KEY_Id, m_vecEmailList[i].id, allocator);
		object.AddMember<unsigned int>(KEY_Type, m_vecEmailList[i].type, allocator);
		object.AddMember<const char*>(KEY_Content, m_vecEmailList[i].content.c_str(), allocator);
		object.AddMember<unsigned int>(KEY_BonusId, m_vecEmailList[i].bonus_unitID, allocator);
		object.AddMember<unsigned int>(KEY_BonusCount, m_vecEmailList[i].bonusCount, allocator);
		object.AddMember<bool>(KEY_IsRead, m_vecEmailList[i].isAlreadyRead, allocator);
		object.AddMember<int>(KEY_RecvTime, m_vecEmailList[i].recvTime, allocator);
		emailArr.PushBack(object, allocator);
	}
	document.AddMember(KEY_Root, emailArr, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);


	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_Name;
	FILE* fp = fopen(FileUtils::getInstance()->fullPathForFilename(filePath).c_str(),"w+");//具有读写属性，写的时候如果文件存在，会被清空，从头开始写。
	if (!fp)
	{
		CCASSERT(fp,"file open faild!");
		return false;
	}
	fwrite(buffer.GetString(),sizeof(char),buffer.Size(),fp);
	fclose(fp);

	CCLOG("%s %s ---> %s", __FUNCTION__, filePath.c_str(), buffer.GetString());
	return true;
}

void EmailHelper::save()
{
	saveData();
}

vector<Email> EmailHelper::getAllEmailList(int num)
{
	vector<Email> ret;

	if (num == 0){
		return m_vecEmailList;
	}
	else if (num > 0){
		sortEmailByTime(m_vecEmailList);

		for (vector<Email>::iterator it = m_vecEmailList.begin(); it != m_vecEmailList.end();){   
			if(NetmsgManger::getNetManager()->chaochuSevenday(time(0),it->recvTime)){
				it=m_vecEmailList.erase(it);
			}else{
				it++;
			}
		}

		vector<Email> m_today;
		vector<Email> m_yesterday;

		for(auto i:m_vecEmailList){
			if(i.isAlreadyRead==true){
				m_yesterday.push_back(i);
			}else{
				m_today.push_back(i);
			}
		}

		m_vecEmailList.clear();

		for(auto i:m_today){
			m_vecEmailList.push_back(i);
		}

		for(auto i:m_yesterday){
			m_vecEmailList.push_back(i);
		}


		ret.insert(ret.begin(), m_vecEmailList.begin(), num>m_vecEmailList.size() ? m_vecEmailList.end() : m_vecEmailList.begin()+num);
		return ret;
	}
	else {
		CCASSERT(false, "error branch!");
		return ret;
	}
}

vector<Email> EmailHelper::getEmailList(Email::EmailType type, bool includeAlreadyRead/* = false*/)
{
	vector<Email> ret;
	for (int i=0,count=m_vecEmailList.size(); i<count; ++i)
	{
		if ((m_vecEmailList.at(i).type == type) && (includeAlreadyRead ? true : !m_vecEmailList.at(i).isAlreadyRead))
		{
			ret.push_back(m_vecEmailList.at(i));
		}
	}

	return ret;
}

void EmailHelper::sortEmailByTime(vector<Email>& list)
{
	for (int i=0,count=list.size(); i<count; ++i)
	{
		for (int j=i+1; j<count; ++j)
		{
			//时间大的邮件最新
			if (list.at(j).recvTime > list.at(i).recvTime)
			{
				std::swap(list.at(i),list.at(j));
			}
		}
	}
}

void EmailHelper::setOneEmailAlreadyRead(unsigned int emailId)
{
	for (int i=0,count=m_vecEmailList.size(); i<count; ++i)
	{
		if (emailId == m_vecEmailList.at(i).id)
		{
			m_vecEmailList.at(i).isAlreadyRead = true;
			return;
		}
	}
}

void EmailHelper::addEmail(Email em)
{
	for (int i=0,count=m_vecEmailList.size(); i<count; ++i)
	{
		if (em.id == m_vecEmailList.at(i).id)
		{
			CCLOG("%s ---> already have this email [id = %u]", __FUNCTION__, em.id);
			return;
		}
	}
	m_vecEmailList.push_back(em);
}

void EmailHelper::removeEmail(unsigned int emailId)
{
	for (int i=0,count=m_vecEmailList.size(); i<count; ++i)
	{
		if (emailId == m_vecEmailList.at(i).id)
		{
			m_vecEmailList.erase(m_vecEmailList.begin()+i);
			return;
		}
	}
}

bool EmailHelper::isUnreadmessages(){


	bool isread=false;
	for (int i=0,count=m_vecEmailList.size(); i<count; ++i)
	{
		if ( m_vecEmailList.at(i).isAlreadyRead==false)
		{	
			isread=true;
			break;
		}
	}
	return isread;
}