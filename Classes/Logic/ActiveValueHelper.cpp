
#include "ActiveValueHelper.h"
#include "RandomGiftBagHelper.h"
#include "PlayerInfoDataManager.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#define FILE_Path "active.txt"
#define TEMP_FILE_Path "infor/active.txt"
//
#define KEY_active "active"
#define KEY_version "version"

#define KEY_id "ID"
#define KEY_value "value"
#define KEY_describe "describe"


ActiveValueHelper* ActiveValueHelper::s_ActiveValueHelper = nullptr;

ActiveValueHelper::ActiveValueHelper()
{
}

ActiveValueHelper* ActiveValueHelper::getHelper()
{
	if (! s_ActiveValueHelper){
		s_ActiveValueHelper = new ActiveValueHelper();
		if (! s_ActiveValueHelper->getFileData())
		{
			CCASSERT(false, "init data error!");
		}
	}
	return s_ActiveValueHelper;
}

bool ActiveValueHelper::getFileData()
{
	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_Path;
	Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
	if (dat.getSize() > 0)
	{
		CCLOG("%s from write path", filePath.c_str());
		return initData(dat);
	}
	else 
	{
		filePath = FileUtils::getInstance()->fullPathForFilename(TEMP_FILE_Path);
		Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
		if (dat.getSize() > 0)
		{
			CCLOG("%s from full path", filePath.c_str());
			bool ret = initData(dat);
			saveData();
			return ret;
		}
	}
	return false;
}
bool ActiveValueHelper::initData(Data dat)
{
	char *buf = new char[dat.getSize()+1];
	strncpy(buf, (char*)dat.getBytes(), dat.getSize());
	buf[dat.getSize()] = 0;
	CCLOG("%s ---> %s\n", __FUNCTION__, buf);

	rapidjson::Document d;
	d.Parse<0>(buf);
	delete [] buf;

	if (d.HasParseError())  //打印解析错误
	{
		CCLOG("%s ---> GetParseError %s\n", __FUNCTION__, d.GetParseError());
		return false;
	}
	else if (d.IsObject()) 
	{
		//1. version
		if (d.HasMember(KEY_version)) 
		{
			m_strVersionCode = d[KEY_version].GetString();
			CCLOG("%s version : %s", FILE_Path, m_strVersionCode.c_str());
		}
		//2. active
		if (d.HasMember(KEY_active) && d[KEY_active].IsArray() && d[KEY_active].Size() > 0)
		{
			Active one;
			const rapidjson::Value& arr = d[KEY_active];
			for (int i=0,count=arr.Size(); i<count; ++i)
			{
				if (arr[i].HasMember(KEY_id) && arr[i].HasMember(KEY_value) && arr[i].HasMember(KEY_describe))
				{
					one.id = arr[i][KEY_id].GetUint();
					one.describe = arr[i][KEY_describe].GetString();
					one.value = arr[i][KEY_value].GetUint();
					m_vecAllActive.push_back(one);
				}
				else 
				{
					return false;
				}
			}
		}
		return true;
	}
	else 
	{
		return false;
	}
}

bool ActiveValueHelper::saveData()
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//1.version
	rapidjson::Value version(rapidjson::kStringType);
	version.SetString(m_strVersionCode.c_str());
	//2. gift bag
	rapidjson::Value activeVec(rapidjson::kArrayType);
	for (int i=0,count=m_vecAllActive.size(); i<count; ++i)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember<unsigned int>(KEY_id, m_vecAllActive[i].id, allocator);
		object.AddMember<unsigned int>(KEY_value, m_vecAllActive[i].value, allocator);
		object.AddMember<const char*>(KEY_describe, m_vecAllActive[i].describe.c_str(), allocator);
		activeVec.PushBack(object, allocator);
	}
	

	document.AddMember(KEY_version, version, allocator);
	document.AddMember(KEY_active, activeVec, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);


	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_Path;
	FILE* fp = fopen(FileUtils::getInstance()->fullPathForFilename(filePath).c_str(),"w+");
	if (!fp)
	{
		CCASSERT(fp,"active value file open faild!");
		return false;
	}
	fwrite(buffer.GetString(),sizeof(char),buffer.Size(),fp);
	fclose(fp);

	CCLOG("%s ---> %s", __FUNCTION__, buffer.GetString());
	return true;
}

int ActiveValueHelper::getHasActiveCount()
{
	return PlayerInfoDataManager::getManager()->getOnePlayerInfo(HasActiveCount);
}
void ActiveValueHelper::setHasActiveCount(int count)
{
	PlayerInfoDataManager::getManager()->setOnePlayerInfo(HasActiveCount, count);
}
bool ActiveValueHelper::canGetGift()
{
	return getHasActiveCount() >= GetGiftNeedActiveNum;
}

BagItem ActiveValueHelper::getOneGift_useActiveNum()
{
	BagItem item;
	if (canGetGift())
	{
		RandomGiftBagHelper::getHelper()->getOneRandomGiftBag(item);
		setHasActiveCount(getHasActiveCount() - GetGiftNeedActiveNum);
	}
	return item;
}

void ActiveValueHelper::addActiveByType(ActiveID type)
{
	for (int i=0,count=m_vecAllActive.size(); i<count; ++i)
	{
		if (type == m_vecAllActive[i].id){
			int all = getHasActiveCount() + m_vecAllActive[i].value;
			setHasActiveCount(all);
			return;
		}
	}
	CCASSERT(false, "未找到");
}

const vector<Active>& ActiveValueHelper::getAllActive()
{
	return m_vecAllActive;
}