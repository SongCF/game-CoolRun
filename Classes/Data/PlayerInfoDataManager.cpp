#include "PlayerInfoDataManager.h"
#include "StructToJsonClass.h"
#include "ActorHelper.h"
#include "BagDataManager.h"
static string PlayerInfoDataName = "PlayerInfo.json";
static string TempFilePath = "infor/PlayerInfo.json";
PlayerInfoDataManager* PlayerInfoDataManager::s_mPlayerInfoDataManager = nullptr;

PlayerInfoDataManager* PlayerInfoDataManager::getManager()
{
	if (! s_mPlayerInfoDataManager) {
		s_mPlayerInfoDataManager = new PlayerInfoDataManager;
		s_mPlayerInfoDataManager->initData();
	}
	return s_mPlayerInfoDataManager;
}

PlayerInfoDataManager::PlayerInfoDataManager()
{
	for (int i = 0 ; i < 4 ; i ++)
	{
		m_bIsUpdate[i] = false;
	}
	m_lLastLoginTime.year = 0;
	m_lLastLoginTime.month = 0;
	m_lLastLoginTime.data = 0;
	m_lLastLoginTime.hours = 0;
	m_sPlayerName = "";
}

void PlayerInfoDataManager::initData()
{
	addHandler("PlayerName",SEL_CallFunString(&PlayerInfoDataManager::getPlayerName),1);
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	string writeablePath = FileUtils::getInstance()->getWritablePath();
	writeablePath += PlayerInfoDataName;
	if (!FileUtils::getInstance()->isFileExist(writeablePath))
	{
		copyFile();
	}
	if (!json->getJsonRootValue(FileUtils::getInstance()->fullPathForFilename(writeablePath),root))
	{
		CCLOG("openFile %s :faild\n",PlayerInfoDataName.c_str());
		return;
	}
	CCLOG("openFile %s :success\n",PlayerInfoDataName.c_str());
	{//item的
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"PlayerInfo");
		int size = 0;
		if (!arrayValue.IsNull())
		{
			size = arrayValue.Size();
		}

		for (int i = 0 ; i < size ; i++)
		{
			PlayerInfo info;
			const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
			if (json->isHave(value,"key") && json->isHave(value,"value"))
			{
				info.key = json->getString_json(value,"key");
				info.value = json->getInt_json(value,"value",0);
				m_lPlayerInfoList.push_back(info);
			}
			

			{
				vector<int> list;
				if (json->isHave(value,"Achievement"))
				{
					const rapidjson::Value &value_list = json->getSubValue_json(value,"Achievement");
					json->getArrayInt_json(value_list,list);
					for (vector<int>::iterator iter = list.begin() ; iter != list.end() ; iter++)
					{
						m_lGotAchievement.push_back(*iter);
					}
				}
			}
			{
				vector<int> list;
				if (json->isHave(value,"Tollgate"))
				{
					const rapidjson::Value &value_list = json->getSubValue_json(value,"Tollgate");
					json->getArrayInt_json(value_list,list);
					for (vector<int>::iterator iter = list.begin() ; iter != list.end() ; iter++)
					{
						m_lGotTollgate.push_back(*iter);
					}
				}
			}
			{
				vector<int> list;
				if (json->isHave(value,"TollgateAward"))
				{
					const rapidjson::Value &value_list = json->getSubValue_json(value,"TollgateAward");
					json->getArrayInt_json(value_list,list);
					for (vector<int>::iterator iter = list.begin() ; iter != list.end() ; iter++)
					{
						m_lGotTollgateAwardList.push_back(*iter);
					}
				}
			}
		}
		if (json->isHave(root,"loginTime"))
		{
			const rapidjson::Value &arrayValue = json->getSubValue_json(root,"loginTime");
			int size = 0;
			if (!arrayValue.IsNull())
			{
				size = arrayValue.Size();
			}
			if (size > 0)
			{
				const rapidjson::Value &value = json->getSubValue_json(arrayValue,0);
				if (json->isHave(value,"year"))
				{
					m_lLastLoginTime.year = json->getInt_json(value,"year",0);
				}
				if (json->isHave(value,"month"))
				{
					m_lLastLoginTime.month = json->getInt_json(value,"month",0);
				}
				if (json->isHave(value,"day"))
				{
					m_lLastLoginTime.data = json->getInt_json(value,"day",0);
				}
				if (json->isHave(value,"hours"))
				{
					m_lLastLoginTime.hours = json->getInt_json(value,"hours",0);
				}
				if (json->isHave(value,"scoreUpdate"))
				{
					m_bIsUpdate[Update_Score] = json->getInt_json(value,"scoreUpdate",0);
				}
			}
			
		}
		if (json->isHave(root,"PlayerName"))
		{
			m_sPlayerName = json->getString_json(root,"PlayerName");
		}
	}
}
void PlayerInfoDataManager::saveData()
{
	FILE *fp = NULL;
	string writeablePath = FileUtils::getInstance()->getWritablePath();
	writeablePath += PlayerInfoDataName;
	fp = fopen(FileUtils::getInstance()->fullPathForFilename(writeablePath).c_str(),"w+");
	if (!fp)
	{
		CCASSERT(fp,"PalyerInfo open faild!");
		return;
	}
	string json = WriteToJson();
	fwrite(json.c_str(),sizeof(char),json.size(),fp);
	fclose(fp);
}
string PlayerInfoDataManager::WriteToJson()
{
	removeAllObject();
	vector<WriteJson*> list;
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		StructToJsonClass *json = new StructToJsonClass();
		json->addAttribute(1,AttributeType_String,"key");
		json->addAttribute(2,AttributeType_Int,"value");
		json->addValue(1,iter->key);
		json->addValue(2,iter->value);
		list.push_back((WriteJson*)json);
	}
	vector<int> achievement_list;
	for (vector<unsigned int>::iterator iter = m_lGotAchievement.begin() ; iter != m_lGotAchievement.end() ; iter++)
	{
		achievement_list.push_back(*iter);
	}
	{
		StructToJsonClass *json = new StructToJsonClass();
		json->addAttribute(1,AttributeType_ArrayInt,"Achievement");
		json->addValue(1,achievement_list);
		list.push_back((WriteJson*)json);
	}

	vector<int> tollgate_list;
	for (vector<unsigned int>::iterator iter = m_lGotTollgate.begin() ; iter != m_lGotTollgate.end() ; iter++)
	{
		tollgate_list.push_back(*iter);
	}
	{
		StructToJsonClass *json = new StructToJsonClass();
		json->addAttribute(1,AttributeType_ArrayInt,"Tollgate");
		json->addValue(1,tollgate_list);
		list.push_back((WriteJson*)json);
	}

	vector<int> tollgateAward_list;
	for (vector<unsigned int>::iterator iter = m_lGotTollgateAwardList.begin() ; iter != m_lGotTollgateAwardList.end() ; iter++)
	{
		tollgateAward_list.push_back(*iter);
	}
	{
		StructToJsonClass *json = new StructToJsonClass();
		json->addAttribute(1,AttributeType_ArrayInt,"TollgateAward");
		json->addValue(1,tollgateAward_list);
		list.push_back((WriteJson*)json);
	}
	
	addJsonArrayObject("PlayerInfo",list);

	{
		StructToJsonClass *json = new StructToJsonClass();
		json->addAttribute(1,AttributeType_Int,"year");
		json->addAttribute(2,AttributeType_Int,"month");
		json->addAttribute(3,AttributeType_Int,"day");
		json->addAttribute(4,AttributeType_Int,"hours");
		json->addAttribute(5,AttributeType_Int,"scoreUpdate");
		json->addValue(1,m_lLastLoginTime.year);
		json->addValue(2,m_lLastLoginTime.month);
		json->addValue(3,m_lLastLoginTime.data);
		json->addValue(4,m_lLastLoginTime.hours);
		json->addValue(5,(int)m_bIsUpdate[Update_Score]);
		this->addJsonObject("loginTime",(WriteJson*)json);
	}
	
	return this->getJsonString();
}
void PlayerInfoDataManager::setDeafaultHero(unsigned int unitID)
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "deafaultHero")
		{
			iter->value = unitID;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = "deafaultHero";
	info.value = unitID;
	m_lPlayerInfoList.push_back(info);
	saveData();
}
unsigned int PlayerInfoDataManager::getDeafaultHero()
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "deafaultHero")
		{
			return iter->value;
		}
	}
	return unitID_Actor1;
}
void PlayerInfoDataManager::setDeafaultPet(unsigned int unitID)
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "deafaultPet")
		{
			iter->value = unitID;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = "deafaultPet";
	info.value = unitID;
	m_lPlayerInfoList.push_back(info);
	saveData();
}
unsigned int PlayerInfoDataManager::getDeafaultPet()
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "deafaultPet")
		{
			return iter->value;
		}
	}
	return 0;
}
void PlayerInfoDataManager::setDeafaultMagicItem(unsigned int unitID)
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "deafaultItem")
		{
			iter->value = unitID;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = "deafaultItem";
	info.value = unitID;
	m_lPlayerInfoList.push_back(info);
	saveData();
}
unsigned int PlayerInfoDataManager::getDeafaultMagicItem()
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "deafaultItem")
		{
			return iter->value;
		}
	}
	return unitID_FaGuangWuXing;
}



void PlayerInfoDataManager::setPlayeriscomteacher(unsigned int unitID){
	 
	 for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "IsCompleteach")
		{
			iter->value = unitID;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = "IsCompleteach";
	info.value = unitID;
	m_lPlayerInfoList.push_back(info);
	saveData();
	   
}

unsigned int PlayerInfoDataManager::getPlayeriscomteacher(){
    
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "IsCompleteach")
		{
			return iter->value;
		}
	}
	return 0;
}


void PlayerInfoDataManager::setGotAchievement(unsigned int unitID)
{
	m_lGotAchievement.push_back(unitID);
	saveData();
}
const vector<unsigned int> *PlayerInfoDataManager::getGotAchievementList()
{
	return &m_lGotAchievement;
}
// void PlayerInfoDataManager::setGotTollgate(unsigned int tollgateID)
// {
// 	m_lGotTollgate.push_back(tollgateID);
// 	saveData();
// }
// const vector<unsigned int> *PlayerInfoDataManager::getGotTollgate()
// {
// 	return &m_lGotTollgate;
// }
// void PlayerInfoDataManager::setGotTollgateAward(unsigned int tollgateID)
// {
// 	m_lGotTollgateAwardList.push_back(tollgateID);
// 	saveData();
// }
// const vector<unsigned int> *PlayerInfoDataManager::getGotTollgateAward()
// {
// 	return &m_lGotTollgateAwardList;
// }


// 奔跑的历史最高纪录（距离）
// void PlayerInfoDataManager::setHistoryMaxDistance(unsigned int meter)
// {
// 	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
// 	{
// 		if (iter->key == "HistoryMaxDistance")
// 		{
// 			iter->value = meter;
// 			saveData();
// 			return;
// 		}
// 	}
// 	PlayerInfo info;
// 	info.key = "HistoryMaxDistance";
// 	info.value = meter;
// 	m_lPlayerInfoList.push_back(info);
// 	saveData();
// }
// unsigned int PlayerInfoDataManager::getHistoryMaxDistance()
// {
// 	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
// 	{
// 		if (iter->key == "HistoryMaxDistance")
// 		{
// 			return iter->value;
// 		}
// 	}
// 	return 0;
// }
void PlayerInfoDataManager::setUserId(unsigned int id)
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "UserID")
		{
			iter->value = id;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = "UserID";
	info.value = id;
	m_lPlayerInfoList.push_back(info);
	saveData();
}
unsigned int PlayerInfoDataManager::getUserId()
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "UserID")
		{
			return iter->value;
		}
	}
	return 0;
}
void PlayerInfoDataManager::changeLoginTime()
{
	struct tm * tm;
	time_t time_sec ;
	time(&time_sec);
	tm = localtime(&time_sec);
	m_lLastLoginTime.year = tm->tm_year + 1900;
	m_lLastLoginTime.month = tm->tm_mon + 1;
	m_lLastLoginTime.data = tm->tm_mday;
	m_lLastLoginTime.hours = tm->tm_hour;
	saveData();
}
const LoginTime *PlayerInfoDataManager::getLastLoginTime()
{
	return &m_lLastLoginTime;
}



void PlayerInfoDataManager::setOnePlayerInfo(string key, int value)
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == key)
		{
			iter->value = value;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = key;
	info.value = value;
	m_lPlayerInfoList.push_back(info);
	saveData();
}
int PlayerInfoDataManager::getOnePlayerInfo(string key)
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == key)
		{
			return iter->value;
		}
	}
	return 0;
}
void PlayerInfoDataManager::addLoginDays()
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "LoginDays")
		{
			iter->value++;
			saveData();
			return;
		}
	}
	PlayerInfo info;
	info.key = "LoginDays";
	info.value = 1;
	m_lPlayerInfoList.push_back(info);
	saveData();
}
unsigned int PlayerInfoDataManager::getLoginDays()
{
	for (vector<PlayerInfo>::iterator iter = m_lPlayerInfoList.begin() ; iter != m_lPlayerInfoList.end() ; iter++)
	{
		if (iter->key == "LoginDays")
		{
			return iter->value;
		}
	}
	return 0;
}
bool PlayerInfoDataManager::getIsUpdate(UpdateTag tag)
{
	return m_bIsUpdate[tag];
}
void PlayerInfoDataManager::setUpdata(UpdateTag tag)
{
	m_bIsUpdate[tag] = true;
	saveData();
}

float PlayerInfoDataManager::getMaxFightPower()
{
	float fRet = 0;

	float fMaxHeroFighrPower = 0;
	float fMaxPetFighrPower = 0;
	float fMaxMagicFightPower = 0;

	vector<Actor> actors = ActorHelper::getHelper()->getAllHero();

	Actor info;
	Actor newInfo;
	for (size_t i = 0; i < actors.size(); ++i)
	{
		info = actors.at(i);

		//只判断拥有的角色
		if (BagDataManager::getManager()->getOneUnit(info.unit.ID))
		{
			newInfo = ActorHelper::getHelper()->getOneHeroWithLevelInfo(info.unit.ID, info.level);
			if (newInfo.unit.getAttributeValue(Attr_Power) > fMaxHeroFighrPower)
			{
				fMaxHeroFighrPower = newInfo.unit.getAttributeValue(Attr_Power);
			}
		}
	}

	actors = ActorHelper::getHelper()->getAllMagic();

	for (size_t i = 0; i < actors.size(); ++i)
	{
		info = actors.at(i);
		//只判断拥有的魔法物
		if (BagDataManager::getManager()->getOneUnit(info.unit.ID))
		{
			newInfo = ActorHelper::getHelper()->getOneMagicWithLevelInfo(info.unit.ID, info.level);

			if (newInfo.unit.haveAttribute(Attr_AddPower) && newInfo.unit.getAttributeValue(Attr_AddPower) > fMaxMagicFightPower)
			{
				fMaxMagicFightPower = newInfo.unit.getAttributeValue(Attr_AddPower);
			}
		}
	}

	actors = ActorHelper::getHelper()->getAllPet();

	for (size_t i = 0; i < actors.size(); ++i)
	{
		info = actors.at(i);
		//只判断拥有的宠物
		if (BagDataManager::getManager()->getOneUnit(info.unit.ID))
		{
			newInfo = ActorHelper::getHelper()->getOnePetWithLevelInfo(info.unit.ID, info.level);

			if (newInfo.unit.haveAttribute(Attr_AddPower) && newInfo.unit.getAttributeValue(Attr_AddPower) > fMaxPetFighrPower)
			{
				fMaxPetFighrPower = newInfo.unit.getAttributeValue(Attr_AddPower);
			}
		}
	}

	fRet = fMaxHeroFighrPower*(1+fMaxPetFighrPower+fMaxMagicFightPower);
	
	return fRet;
}

void PlayerInfoDataManager::copyFile()
{	
	string fileName = cocos2d::FileUtils::getInstance()->fullPathForFilename(TempFilePath.c_str());

	cocos2d::Data dat = cocos2d::FileUtils::getInstance()->getDataFromFile(fileName);
	long size = dat.getSize();
	char *pTempData = (char*)malloc(size + 1);
	memcpy(pTempData, dat.getBytes(), size);
	pTempData[size] = 0;

	string writeablePath = FileUtils::getInstance()->getWritablePath();
	writeablePath += PlayerInfoDataName;
	FILE *fp = fopen(writeablePath.c_str(),"wb+");
	fwrite(pTempData,sizeof(char),size,fp);
	fclose(fp);
	delete pTempData;
	
}