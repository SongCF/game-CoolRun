#include "BagDataManager.h"
#include "cocos2d.h"
#include "MsgDefine.h"
#include "UnitConfigManager.h"
#include "StructToJsonClass.h"
#include "GoodsHelper.h"
#include "AchievementHelper.h"
#include "VersionConfig.h"
//#include "sdk/SdkManager.h"

using namespace cocos2d;
static string DataName = "Bag.json";
static string TempFilePath = "infor/Bag.json";

BagDataManager* BagDataManager::s_mBagManager = nullptr;


BagDataManager::BagDataManager()
{
}

BagDataManager* BagDataManager::getManager()
{
	if (! s_mBagManager) {
		s_mBagManager = new BagDataManager;
		s_mBagManager->initData();
	}
	return s_mBagManager;
}

void BagDataManager::initData()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	string writeablePath = FileUtils::getInstance()->getWritablePath();
	writeablePath += DataName;
	if (!FileUtils::getInstance()->isFileExist(writeablePath))
	{
		copyFile();
	}
	if (!json->getJsonRootValue(FileUtils::getInstance()->fullPathForFilename(writeablePath),root))
	{
		{
			BagItem info;
			info.id = ID_Actor_Start;
			info.value = 1;
			m_BagItemVec.push_back(info);
		}
		{
			BagItem info;
			info.id = unitID_Gold;
			info.value = 10000000;
			m_BagItemVec.push_back(info);
		}
		{
			BagItem info;
			info.id = unitID_Diamond;
			info.value = 10000000;
			m_BagItemVec.push_back(info);
		}
		{
			BagItem info;
			info.id = unitID_FaGuangWuXing;
			info.value = 1;
			m_BagItemVec.push_back(info);
		}
		//
		{
			BagItem info;
			info.id = unitID_PiPiZhu;
			info.value = 1;
			m_BagItemVec.push_back(info);
		}
		{
			BagItem info;
			info.id = unitID_TangGuo;
			info.value = 10000;
			m_BagItemVec.push_back(info);
		}
		{
			BagItem info;
			info.id = unitID_GuoDong;
			info.value = 10000;
			m_BagItemVec.push_back(info);
		}
		{
			BagItem info;
			info.id = unitID_DanGao;
			info.value = 10000;
			m_BagItemVec.push_back(info);
		}
		saveData();
		return;
	}
	CCLOG("openFile %s :\n",DataName.c_str());
	{//item的
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"items");
		int size = 0;
		if (!arrayValue.IsNull())
		{
			size = arrayValue.Size();
		}
		
		for (int i = 0 ; i < size ; i++)
		{
			BagItem info;
			const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
			if (json->isHave(value,"ID"))
			{
				info.id = json->getInt_json(value,"ID",0);
			}
			if (json->isHave(value,"value"))
			{
				info.value = json->getInt_json(value,"value",0);
			}
			if (json->isHave(value,"value_2"))
			{
				info.value_2 = json->getLong_json(value,"value_2",0);
			}
			
			m_BagItemVec.push_back(info);
		}
	}

}

const BagItem* BagDataManager::getOneUnit(unsigned int id)
{
	for (int i=0,count=m_BagItemVec.size(); i<count; ++i)
	{
		if (m_BagItemVec[i].id == id) return &m_BagItemVec[i];
	}

	return nullptr;
}
const vector<BagItem>* BagDataManager::getAllUnits()
{
	return &m_BagItemVec;
}

void BagDataManager::changeOneUnitCount(unsigned int id, int num)
{
	for (int i=0,count=m_BagItemVec.size(); i<count; ++i)
	{
		if (m_BagItemVec[i].id == id){
			m_BagItemVec[i].value += num;

			//__NotificationCenter::getInstance()->postNotification(MSG_BagDataChanged, nullptr);
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_BagDataChanged);
			event.setUserData(&m_BagItemVec[i]);
			eventDspc->dispatchEvent(&event);
			saveData();

			if (id == unitID_Gold)
			{
				Achieve* info = nullptr;
				//检测小富豪成就
				if (GoodsHelper::getHelper()->getGoldNum() >= 20000)
				{
					info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_XiaoFuHao);
					if (!info->isOver)
					{
						AchievementHelper::getHelper()->completeOneAchievement(achieveID_XiaoFuHao, 1);
					}
				}

				//检测大土豪成就
				if (GoodsHelper::getHelper()->getGoldNum() >= 50000)
				{
					info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_DaTuHao);
					if (!info->isOver)
					{
						AchievementHelper::getHelper()->completeOneAchievement(achieveID_DaTuHao, 1);
					}
				}
			}

			return;
		}
	}

	if (num > 0){
		BagItem temp;
		temp.id = id;
		temp.value = num;
		m_BagItemVec.push_back(temp);

		//__NotificationCenter::getInstance()->postNotification(MSG_BagDataChanged, nullptr);
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_BagDataChanged);
		event.setUserData(&temp);
		eventDspc->dispatchEvent(&event);
		saveData();
		return;
	}
	else if (1 == DEBUG_uMessageBox){
		cocos2d::MessageBox(cocos2d::StringUtils::format("BagItem Id=[%d] is null, can not sub", id).c_str(), "ERROR");
		CCASSERT(false, "");
	}
}

void BagDataManager::changeOneUnitLevel(unsigned int id, int addLv,int value)
{
	//CCASSERT(addLv > 0, "couldn't sub level");

	{
		map<string,string> info;
		time_t curTime = time(0);
		struct tm *curTm = localtime(&curTime);//时区时间， 非时区时间使用struct tm *gmtime(const time_t *timep); 
		char * timeStr = asctime(curTm);
		info.insert(std::make_pair("LevelUpTime", timeStr));
		info.insert(std::make_pair("LevelUpId", String::createWithFormat("%u", id)->getCString()));
		info.insert(std::make_pair("LevelUpNum", String::createWithFormat("%d", addLv)->getCString()));
		//SdkManager::getInstance()->logEvent(analyticsEVENT_Role, &info);
	}

	for (int i=0,count=m_BagItemVec.size(); i<count; ++i)
	{
		if (m_BagItemVec[i].id == id){
			m_BagItemVec[i].value += addLv;
			m_BagItemVec[i].value_2 += value;
			//__NotificationCenter::getInstance()->postNotification(MSG_BagDataChanged, nullptr);
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_BagDataChanged);
			event.setUserData(&m_BagItemVec[i]);
			eventDspc->dispatchEvent(&event);
			saveData();
			return;
		}
	}

	if (1 == DEBUG_uMessageBox)
	{		
		cocos2d::MessageBox(cocos2d::StringUtils::format("BagItem Id=[%d] is null, can not sub", id).c_str(), "ERROR");
		CCASSERT(false, "");
	}
}
string BagDataManager::WriteToJson()
{
	removeAllObject();
	vector<WriteJson*> list;
	for (vector<BagItem>::iterator iter = m_BagItemVec.begin() ; iter != m_BagItemVec.end() ; iter++)
	{
		StructToJsonClass *json = new StructToJsonClass();
		json->addAttribute(1,AttributeType_Int,"ID");
		json->addAttribute(2,AttributeType_Int,"value");
		json->addAttribute(3,AttributeType_Long,"value_2");
		json->addValue(1,(int)iter->id);
		json->addValue(2,(int)iter->value);
		json->addValue(3,(long)iter->value_2);
		list.push_back((WriteJson*)json);
	}
	addJsonArrayObject("items",list);
	return this->getJsonString();
}
void BagDataManager::saveData()
{
	FILE *fp = NULL;
	string writeablePath = FileUtils::getInstance()->getWritablePath();
	writeablePath += DataName;
	fp = fopen(FileUtils::getInstance()->fullPathForFilename(writeablePath).c_str(),"w+");
	if (!fp)
	{
		CCASSERT(fp,"bag data open faild!");
		return;
	}
	string json = WriteToJson();
	fwrite(json.c_str(),sizeof(char),json.size(),fp);
	fclose(fp);
}
void BagDataManager::changeOneAchievement(unsigned int achieveID, int data,long time)
{
	CCASSERT(data > 0, "couldn't sub achieve");

	for (int i=0,count=m_BagItemVec.size(); i<count; ++i)
	{
		if (m_BagItemVec[i].id == achieveID){
			m_BagItemVec[i].value += data;
			m_BagItemVec[i].value_2 = time;
			//__NotificationCenter::getInstance()->postNotification(MSG_BagDataChanged, nullptr);
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_BagDataChanged);
			event.setUserData(&m_BagItemVec[i]);
			eventDspc->dispatchEvent(&event);
			saveData();
			return;
		}
	}
	
	if (data > 0){
		BagItem temp;
		temp.id = achieveID;
		temp.value = data;
		temp.value_2 = time;
		m_BagItemVec.push_back(temp);

		//__NotificationCenter::getInstance()->postNotification(MSG_BagDataChanged, nullptr);
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_BagDataChanged);
		event.setUserData(&temp);
		eventDspc->dispatchEvent(&event);
		saveData();
		return;
	}
	if (1 == DEBUG_uMessageBox)
	{
		cocos2d::MessageBox(cocos2d::StringUtils::format("BagItem Id=[%d] is null, can not sub", achieveID).c_str(), "ERROR");
		CCASSERT(false, "");
	}

}
void BagDataManager::copyFile()
{
	string fileName = cocos2d::FileUtils::getInstance()->fullPathForFilename(TempFilePath.c_str());

	cocos2d::Data dat = cocos2d::FileUtils::getInstance()->getDataFromFile(fileName);
	long size = dat.getSize();
	char *pTempData = (char*)malloc(size + 1);
	memcpy(pTempData, dat.getBytes(), size);
	pTempData[size] = 0;

	string writeablePath = FileUtils::getInstance()->getWritablePath();
	writeablePath += DataName;
	FILE *fp = fopen(writeablePath.c_str(),"wb+");
	fwrite(pTempData,sizeof(char),size,fp);
	fclose(fp);
	delete pTempData;
}