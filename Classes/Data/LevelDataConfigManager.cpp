#include "LevelDataConfigManager.h"
#include "VersionConfig.h"
#include "cocos2d.h"
#include "MsgDefine.h"
#include "UnitConfigManager.h"

#include "DataParse.h"
using namespace cocos2d;

static string DatatName = "config/Level.txt";
LevelDataConfigManager* LevelDataConfigManager::s_mLevelDataManager = nullptr;

LevelDataConfigManager::LevelDataConfigManager()
{
}

LevelDataConfigManager* LevelDataConfigManager::getManager()
{
	if (! s_mLevelDataManager) {
		s_mLevelDataManager = new LevelDataConfigManager;
		s_mLevelDataManager->initData();
	}
	return s_mLevelDataManager;
}
void LevelDataConfigManager::initJsonData(const rapidjson::Value &arrayValue,const string& name)
{
	XReadJson *json = XReadJson::getIntanse();
	UnitConfigManager *unitContext = UnitConfigManager::getManager();
	int size = arrayValue.Size();
	for (int i = 0 ; i < size ; i++)
	{
		vector<OneLevelInfo> levelInfoList;
		const rapidjson::Value &arrayRoot = json->getSubValue_json(arrayValue,i);
		int id = json->getInt_json(arrayRoot,name.c_str(),0);
		const rapidjson::Value &arrayValue2 = json->getSubValue_json(arrayRoot,"lvAttr");
		int size2 = arrayValue2.Size();
		for (int j = 0 ; j < size2 ; j++)
		{	
			OneLevelInfo info;
			const rapidjson::Value &value = json->getSubValue_json(arrayValue2,j);
			if (json->isHave(value,"lv"))
			{
				info.level = json->getInt_json(value,"lv",0);
			}
			if (json->isHave(value,"need_count"))
			{
				info.needCount = json->getInt_json(value,"need_count",0);
			}
			if (json->isHave(value,"need_unitID"))
			{
				info.needUnit = const_cast<Unit*>(unitContext->getOneUnit(json->getInt_json(value,"need_unitID",0)));
			}
			if (json->isHave(value,"name"))
			{
				info.name = json->getString_json(value,"name");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imgFile = json->getString_json(value,"imgFile");
			}
			//string attr;
			if (json->isHave(value,"attribute"))
			{
				string attr = json->getString_json(value,"attribute");
				vector<AttributeStruct> list;
				DataParse::attributeParse(attr,list);
				vector<Attribute> att_list;
				for (vector<AttributeStruct>::iterator iter = list.begin() ; iter != list.end(); iter ++)
				{
					Attribute atttr_info;
					atttr_info.attr = iter->name;
					atttr_info.value = atof(iter->value.c_str());
					att_list.push_back(atttr_info);
				}
				info.attrVec = att_list;
			}

			levelInfoList.push_back(info);
			//info.attrVec 
		}
		m_levelInfoPool.insert(map<unsigned int, vector<OneLevelInfo> >::value_type(id,levelInfoList));
	}
}
void LevelDataConfigManager::initData()
{
//parse file
	
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	json->getJsonRootValue(DatatName,root);
	CCLOG("openFile %s :\n",DatatName.c_str());
	{//item的
		
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"actor");
		initJsonData(arrayValue,"actor_unitID");
	}
	{//item的
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"pet");
		initJsonData(arrayValue,"pet_unitID");
		
	}
	{//item的
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"magic");
		initJsonData(arrayValue,"magic_unitID");
	}
}

const vector<OneLevelInfo>* LevelDataConfigManager::getOneUnitAllLevelInfo(unsigned int actorOrPet_ID)
{
	auto p = m_levelInfoPool.find(actorOrPet_ID);
	if (p != m_levelInfoPool.end())
	{
		return &p->second;
	}else {
		CCASSERT(p != m_levelInfoPool.end(), "invalid unit id");
		return nullptr;
	}
}

const OneLevelInfo* LevelDataConfigManager::getOneUnitLevelInfo(unsigned int actorOrPet_ID, unsigned int level)
{
	auto infoVec = getOneUnitAllLevelInfo(actorOrPet_ID);

	for (int i=0,size=infoVec->size(); i<size; ++i)
	{
		if (infoVec->at(i).level == level){
			return &infoVec->at(i);
		}
	}

	if (1 == DEBUG_uMessageBox)
	{
		cocos2d::MessageBox(cocos2d::StringUtils::format("not found levelInfo by level=[%d]", level).c_str(), "ERROR");
		CCASSERT(false, "");
	}

	return nullptr;
}
int LevelDataConfigManager::getMaxLevel(unsigned int actor_ID)
{
	int level = 0;
	auto p = m_levelInfoPool.find(actor_ID);
	if (p != m_levelInfoPool.end())
	{
		vector<OneLevelInfo> list = p->second;
		for (vector<OneLevelInfo>::iterator iter = list.begin(); iter != list.end() ; iter ++)
		{
			if (level < iter->level)
			{
				level = iter->level;
			}
		}
	}
	return level;
}