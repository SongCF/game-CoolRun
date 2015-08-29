/*
 *json format:
 {
 "version":"20140925",
 "GiftBag":[
 {
 "unitID":98,
 "value":1,
 "randomWeight":30
 }
 ],
 "MarketItem":[
 {
 "unitID":98,
 "value":1,
 "randomWeight":30
 }
 ]
 }
*/


#include "RandomGiftBagHelper.h"
#include <random>

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#define FILE_GiftTable "RandomGiftTable.txt"
#define TEMP_FILE_PATH "infor/RandomGiftTable.txt"


#define KEY_version "version"
#define KEY_GiftBag "GiftBag"
#define KEY_MarketItem "MarketItem"


#define KEY_unitId "unitID"
#define KEY_value "value"
#define KEY_randomWeight "randomWeight"


RandomGiftBagHelper* RandomGiftBagHelper::s_randomGiftbagHelper = nullptr;

RandomGiftBagHelper::RandomGiftBagHelper()
{
	m_giftbag_iTotalWeight = 0;
	m_market_iTotalWeight = 0;
}

RandomGiftBagHelper* RandomGiftBagHelper::getHelper()
{
	if (! s_randomGiftbagHelper){
		s_randomGiftbagHelper = new RandomGiftBagHelper();
		if (! s_randomGiftbagHelper->getFileData()){
			CCASSERT(false, "init data error!");
		}
	}
	return s_randomGiftbagHelper;
}


bool RandomGiftBagHelper::getFileData()
{
	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_GiftTable;
	Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
	if (dat.getSize() > 0)
	{
		CCLOG("%s from write path", filePath.c_str());
		return initData(dat);
	}
	else 
	{
		filePath = FileUtils::getInstance()->fullPathForFilename(TEMP_FILE_PATH);
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
bool RandomGiftBagHelper::initData(Data dat)
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
			m_giftbag_strTableVersion = d[KEY_version].GetString();
		//2. gift bag
		if (d.HasMember(KEY_GiftBag) && d[KEY_GiftBag].IsArray() && d[KEY_GiftBag].Size() > 0)
		{
			const rapidjson::Value& arr = d[KEY_GiftBag];
			int weight = 0;
			BagItem item;
			for (int i=0,count=arr.Size(); i<count; ++i)
			{
				if (arr[i].HasMember(KEY_unitId) && arr[i].HasMember(KEY_randomWeight) && arr[i].HasMember(KEY_value))
				{
					weight += arr[i][KEY_randomWeight].GetUint();
					item.id = arr[i][KEY_unitId].GetUint();
					item.value = arr[i][KEY_value].GetUint();
					m_giftbag_vecGiftTable.push_back(pair<BagItem, unsigned int>(item, arr[i][KEY_randomWeight].GetUint()));
				}
				else 
				{
					return false;
				}
			}
			m_giftbag_iTotalWeight = weight;
		}
		//3. market item
		if (d.HasMember(KEY_MarketItem) && d[KEY_MarketItem].IsArray() && d[KEY_MarketItem].Size() > 0)
		{
			const rapidjson::Value& arr = d[KEY_MarketItem];
			int weight = 0;
			BagItem item;
			for (int i=0,count=arr.Size(); i<count; ++i)
			{
				if (arr[i].HasMember(KEY_unitId) && arr[i].HasMember(KEY_randomWeight) && arr[i].HasMember(KEY_value))
				{
					weight += arr[i][KEY_randomWeight].GetUint();
					item.id = arr[i][KEY_unitId].GetUint();
					item.value = arr[i][KEY_value].GetUint();
					m_market_vecGiftTable.push_back(pair<BagItem, unsigned int>(item, arr[i][KEY_randomWeight].GetUint()));
				}
				else 
				{
					return false;
				}
			}
			m_market_iTotalWeight = weight;
		}
		return true;
	}
	else 
	{
		return false;
	}
}
bool RandomGiftBagHelper::saveData()
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//1.version
	rapidjson::Value version(rapidjson::kStringType);
	version.SetString(m_giftbag_strTableVersion.c_str());
	//2. gift bag
	rapidjson::Value giftBag(rapidjson::kArrayType);
	for (int i=0,count=m_giftbag_vecGiftTable.size(); i<count; ++i)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember<unsigned int>(KEY_unitId, m_giftbag_vecGiftTable[i].first.id, allocator);
		object.AddMember<unsigned int>(KEY_value, m_giftbag_vecGiftTable[i].first.value, allocator);
		object.AddMember<unsigned int>(KEY_randomWeight, m_giftbag_vecGiftTable[i].second, allocator);
		giftBag.PushBack(object, allocator);
	}
	//3. market item
	rapidjson::Value marketItem(rapidjson::kArrayType);
	for (int i=0,count=m_market_vecGiftTable.size(); i<count; ++i)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember<unsigned int>(KEY_unitId, m_market_vecGiftTable[i].first.id, allocator);
		object.AddMember<unsigned int>(KEY_value, m_market_vecGiftTable[i].first.value, allocator);
		object.AddMember<unsigned int>(KEY_randomWeight, m_market_vecGiftTable[i].second, allocator);
		marketItem.PushBack(object, allocator);
	}

	document.AddMember(KEY_version, version, allocator);
	document.AddMember(KEY_GiftBag, giftBag, allocator);
	document.AddMember(KEY_MarketItem, marketItem, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);


	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += FILE_GiftTable;
	FILE* fp = fopen(FileUtils::getInstance()->fullPathForFilename(filePath).c_str(),"w+");
	if (!fp)
	{
		CCASSERT(fp,"RandomGiftBag.dat open faild!");
		return false;
	}
	fwrite(buffer.GetString(),sizeof(char),buffer.Size(),fp);
	fclose(fp);

	CCLOG("%s ---> %s", __FUNCTION__, buffer.GetString());
	return true;
}

	//pair<unsigned int, unsigned int>   --->   unitID  权重
void RandomGiftBagHelper::setGiftBagTable(const vector<pair<BagItem, unsigned int>>& table)
{
	m_giftbag_vecGiftTable = table;
	saveData();
}
const vector<pair<BagItem, unsigned int>>& RandomGiftBagHelper::getGiftBagTable()
{
	return m_giftbag_vecGiftTable;
}

	//抽取一个随机礼包 
bool RandomGiftBagHelper::getOneRandomGiftBag(BagItem& item)
{
	// 1. 权重<=0 则未初始化
	if (m_giftbag_iTotalWeight <= 0) 
		return false;

	// 随机
	std::mt19937 eng(time(nullptr));
	std::uniform_int_distribution<int> dist(1, m_giftbag_iTotalWeight);
	int weight = dist(eng);
	int total = 0;
	for (int i=0,count=m_giftbag_vecGiftTable.size(); i<count; ++i)
	{
		total += m_giftbag_vecGiftTable[i].second;
		if (total >= weight){
			item = m_giftbag_vecGiftTable[i].first;
			break;
		}
	}

	return true;
}

bool RandomGiftBagHelper::getOneRandomMarketItem(BagItem& item)
{
	// 1. 权重<=0 则未初始化
	if (m_market_iTotalWeight <= 0) 
		return false;

	// 随机
	std::mt19937 eng(time(nullptr));
	std::uniform_int_distribution<int> dist(1, m_market_iTotalWeight);
	int weight = dist(eng);
	int total = 0;
	for (int i=0,count=m_market_vecGiftTable.size(); i<count; ++i)
	{
		total += m_market_vecGiftTable[i].second;
		if (total >= weight){
			item = m_market_vecGiftTable[i].first;
			break;
		}
	}

	return true;
}