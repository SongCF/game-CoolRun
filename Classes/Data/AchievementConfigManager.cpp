#include "AchievementConfigManager.h"
#include "cocos2d.h"

static string DatatName = "config/Achievement.txt";
AchievementConfigManager* AchievementConfigManager::s_mAchievementConfigManager = nullptr;


AchievementConfigManager* AchievementConfigManager::getManager()
{
	if (! s_mAchievementConfigManager) {
		s_mAchievementConfigManager = new AchievementConfigManager;
		s_mAchievementConfigManager->initData();
	}
	return s_mAchievementConfigManager;
}

AchievementConfigManager::AchievementConfigManager()
{
}
AchievementConfigManager::~AchievementConfigManager()
{

}
const vector<Achieve>* AchievementConfigManager::getAllAchievement()
{
	return &m_lAchieveVec;
}
const Achieve* AchievementConfigManager::getAchievementByID(unsigned int achieveID)
{
	Achieve *achieve = NULL;
	for (vector<Achieve>::iterator iter = m_lAchieveVec.begin() ; iter != m_lAchieveVec.end(); iter ++)
	{
		if (iter->ID == achieveID)
		{
			achieve = &(*iter);
			break;
		}
	}
	CCASSERT(achieve,"No this Achieve ID");
	return achieve;
}

void AchievementConfigManager::initData()
{
	CCLOG("%s ---> start parse file %s", __FUNCTION__, DatatName.c_str());
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	json->getJsonRootValue(DatatName,root);
	{
		const Value &arrayValue = json->getSubValue_json(root,"Achievement");
		int size = arrayValue.Size();
		for (int i = 0 ; i < size ; i++)
		{
			Achieve info;
			const Value &value = json->getSubValue_json(arrayValue,i);
			if (json->isHave(value,"ID"))
			{
				info.ID = json->getInt_json(value,"ID",0);
			}
			if (json->isHave(value,"name"))
			{
				info.name = json->getString_json(value,"name");
			}
			if (json->isHave(value,"describe"))
			{
				info.describe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imgFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"count"))
			{
				info.count = json->getInt_json(value,"count",0);
			}
			if (json->isHave(value,"unitID"))
			{
				info.award_unitID = json->getInt_json(value,"unitID",0);
			}
			m_lAchieveVec.push_back(info);
		}
	}
	{
		const Value &arrayValue = json->getSubValue_json(root,"AchieveSpecialAward");
		int size = arrayValue.Size();
		for (int i = 0 ; i < size ; i++)
		{
			AchieveSpecialAward info;
			const Value &value = json->getSubValue_json(arrayValue,i);
			if (json->isHave(value,"ID"))
			{
				info.ID = json->getInt_json(value,"ID",0);
			}
			if (json->isHave(value,"award_unitID"))
			{
				info.award_unitID = json->getInt_json(value,"award_unitID",0);
			}
			if (json->isHave(value,"award_value"))
			{
				info.award_value = json->getInt_json(value,"award_value",0);
			}
			if (json->isHave(value,"ex_value"))
			{
				info.ex_value = json->getInt_json(value,"ex_value",0);
			}
			m_lAchieveSpecialAwardVec.push_back(info);
		}
	}


}
const vector<AchieveSpecialAward>* AchievementConfigManager::getAllAchieveSpecialAwardVec()
{
	return &m_lAchieveSpecialAwardVec;
}