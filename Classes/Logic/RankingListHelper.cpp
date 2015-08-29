#include "RankingListHelper.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "cocos2d.h"
USING_NS_CC;


#define FILE_Path "config/RankingListBonus.txt"
#define KEY_ListArray "RankingList"
//
#define KEY_groupId "groupId"
#define KEY_rankId "rankId"
#define KEY_bonusId "bonus_unitID"
#define KEY_bonusCount "bonusCount"
#define KEY_describe "describe"



RankingListHelper* RankingListHelper::m_sRankinglistHelper = nullptr;

RankingListHelper::RankingListHelper()
{

}

RankingListHelper::~RankingListHelper()
{
}
	
RankingListHelper* RankingListHelper::getHelper()
{
	if (!m_sRankinglistHelper) {
		m_sRankinglistHelper = new RankingListHelper();
		m_sRankinglistHelper->initData();
	}
	return m_sRankinglistHelper;
}

bool RankingListHelper::initData()
{
	string filePath = FileUtils::getInstance()->fullPathForFilename(FILE_Path);
	Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
	if (dat.getSize() <= 0)
	{
		CCLOG("%s ---> load %s error", __FUNCTION__, filePath.c_str());
		return false;
	}

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
		//3. market item
		if (d.HasMember(KEY_ListArray) && d[KEY_ListArray].IsArray() && d[KEY_ListArray].Size() > 0)
		{
			const rapidjson::Value& arr = d[KEY_ListArray];
			RankingData one;
			for (int i=0,count=arr.Size(); i<count; ++i)
			{
				if (arr[i].HasMember(KEY_groupId) && arr[i].HasMember(KEY_rankId) && arr[i].HasMember(KEY_bonusId)
					 && arr[i].HasMember(KEY_bonusCount) && arr[i].HasMember(KEY_describe))
				{
					one.groupId = arr[i][KEY_groupId].GetUint();
					one.rankId = arr[i][KEY_rankId].GetUint();
					one.bonus_unitId = arr[i][KEY_bonusId].GetUint();
					one.bonusCount = arr[i][KEY_bonusCount].GetUint();
					one.describe = arr[i][KEY_describe].GetString();
					m_vecRankingList.push_back(one);
				}
				else 
				{
					return false;
				}
			}
			return true;
		}
	}
	else 
	{
		return false;
	}
	return false;
}

RankingData RankingListHelper::getOneRankingData(unsigned int groupId, unsigned int rankId)
{
	for (int i=0,count=m_vecRankingList.size(); i<count; ++i)
	{
		if (m_vecRankingList.at(i).groupId == groupId && m_vecRankingList.at(i).rankId == rankId){
			return m_vecRankingList.at(i);
		}
	}

	CCASSERT(false, "not found");
	return RankingData();
}

vector<RankingData> RankingListHelper::getOneGroupRankingList(unsigned int groupId)
{
	vector<RankingData> ret;
	for (int i=0,count=m_vecRankingList.size(); i<count; ++i)
	{
		if (m_vecRankingList.at(i).groupId == groupId){
			ret.push_back(m_vecRankingList.at(i));
		}
	}

	return ret;
}
const vector<RankingData>& RankingListHelper::getAllRankingList()
{
	return m_vecRankingList;
}