#include "GameSceneConfigManager.h"
#include "UnitConfigManager.h"
#include "cocos2d.h"
#include "TollgateHelper.h"
#include "FightTotalMapManager.h"
using namespace cocos2d;
static string DatatName = "config/maps/common/map.txt";
static string BackName = "config/maps/Background.txt";
static string FileNameHead = "config/maps/common/";

static string SuperDatatName = "config/maps/super/map.txt";
static string SuperFileNameHead = "config/maps/super/";

static string TollgateRoot = "config/maps/Tollgate/";

static string FightDatatName = "config/maps/Fight/map.txt";
static string FightRoot = "config/maps/Fight/";
GameSceneConfigManager* GameSceneConfigManager::s_mSceneConfigManager = nullptr;


bool sortByPosX(const SceneItem& a, const SceneItem& b)
{
	return a.posX < b.posX;

}


GameSceneConfigManager::GameSceneConfigManager()
	:m_nMaxMeter(0)
{

}

GameSceneConfigManager* GameSceneConfigManager::getManager()
{
	if (!s_mSceneConfigManager) {
		s_mSceneConfigManager = new GameSceneConfigManager;
		s_mSceneConfigManager->initData();
	}
	return s_mSceneConfigManager;
}

void GameSceneConfigManager::initData()
{
	//parse file
	
	int index = 0;
	XReadJson *json = XReadJson::getIntanse();
	map<int,vector<string> > map_list;
	{
		Document root;
		json->getJsonRootValue(DatatName,root);
		{
			const rapidjson::Value &arrayValue = json->getSubValue_json(root,"maps");
			if (json->isHave(arrayValue,"MAX_Meter"))
			{
				unsigned int meter = json->getInt_json(arrayValue,"MAX_Meter",0);
				m_lMaxMeterList.insert(map<Scene_PlayMode,unsigned int>::value_type(mode_Common,meter));
			}
		}
		
		{
			
			const rapidjson::Value &arrayValue = json->getSubValue_json(root,"map");
			int size = arrayValue.Size();
			for (int i = 0 ; i < size; i++)
			{
				const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
				vector<string> list1;
				json->getArrayString_json(value,list1);
				map_list.insert(map<int,vector<string> >::value_type(i + 1,list1));
			}
		}
		
	}
	
	int number = 0;
	for (map<int,vector<string> >::iterator iter = map_list.begin() ; iter != map_list.end() ; iter++)
	{
		Document root;
		number ++;
		unsigned int meter = number;
		int startIndex = index;
		int count = 0;
		for (vector<string>::iterator iter_name = iter->second.begin(); iter_name != iter->second.end() ; iter_name++)
		{
			vector<SceneItem> list;
			vector<int> backList;
			SceneItemInfo info;

			int screen_count = 1;
			getScenItemFromFile(FileNameHead + *iter_name,screen_count,list,backList);
			m_nScreenCount.insert(map<unsigned int,unsigned int >::value_type(index,screen_count) );

			info.list = list;
			info.backList = backList;
			m_sceneItemsConfigPool.insert(map<unsigned int,SceneItemInfo >::value_type(index,info) );
			index++;
			count++;
		}
		MapInfoList mapList;
		mapList.meters = meter;
		mapList.startIndex = startIndex;
		mapList.count = count;
		
		m_lMapList.insert(map<unsigned int , MapInfoList>::value_type(meter,mapList));
	}
	int count = 0;
	//getTollgateSceneItems("cpp_map1_1.txt",count);*/

	initBackData();
	initSuperData();
	initFightData();
}
void GameSceneConfigManager::initSuperData()
{
	int index = 0;
	XReadJson *json = XReadJson::getIntanse();
	map<int,vector<string> > map_list;
	{
		Document root;
		json->getJsonRootValue(SuperDatatName,root);
		{
			const rapidjson::Value &arrayValue = json->getSubValue_json(root,"maps");
			if (json->isHave(arrayValue,"MAX_Meter"))
			{
				unsigned int meter = json->getInt_json(arrayValue,"MAX_Meter",0);
				m_lMaxMeterList.insert(map<Scene_PlayMode,unsigned int>::value_type(mode_Super,meter));
			}
		}

		{

			const rapidjson::Value &arrayValue = json->getSubValue_json(root,"map");
			int size = arrayValue.Size();
			for (int i = 0 ; i < size; i++)
			{
				const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
				vector<string> list1;
				json->getArrayString_json(value,list1);
				map_list.insert(map<int,vector<string> >::value_type(i + 1,list1));
			}
		}

	}

	int number = 0;
	for (map<int,vector<string> >::iterator iter = map_list.begin() ; iter != map_list.end() ; iter++)
	{
		Document root;
		number ++;
		unsigned int meter = number;
		int startIndex = index;
		int count = 0;
		for (vector<string>::iterator iter_name = iter->second.begin(); iter_name != iter->second.end() ; iter_name++)
		{

			vector<SceneItem> list;
			vector<int> backList;
			SceneItemInfo info;
			int screen_count = 1;
			getScenItemFromFile(SuperFileNameHead + *iter_name,screen_count,list,backList);
			info.backList = backList;
			info.list = list;
			m_nSuperScreenCount.insert(map<unsigned int,unsigned int >::value_type(index,screen_count) );
			m_SupersceneItemsConfigPool.insert(map<unsigned int,SceneItemInfo >::value_type(index,info) );
			index++;
			count++;
		}
		MapInfoList mapList;
		mapList.meters = meter;
		mapList.startIndex = startIndex;
		mapList.count = count;

		m_lSuperMapList.insert(map<unsigned int , MapInfoList>::value_type(meter,mapList));
	}
}
void GameSceneConfigManager::initBackData()
{
	Document root;
//	int index = 1;
	XReadJson *json = XReadJson::getIntanse();
	json->getJsonRootValue(BackName,root);

	const rapidjson::Value &arrayValue = json->getSubValue_json(root,"background");
	int size = arrayValue.Size();
	
	for (int i = 0 ; i < size ; i++)
	{
		int id = 0;
		
		const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
		if (json->isHave(value,"ID"))
		{
			id = json->getInt_json(value,"ID",0);
		}
		if (!json->isHave(value,"bg"))
		{
			CCASSERT(NULL,"GameSceneConfigManager : no key = [bg]");
		}
		const rapidjson::Value &arrayValue2 = json->getSubValue_json(value,"bg");

		int size2 = arrayValue2.Size();
		BackgroundList backList;
		SceneBackground scene;
		TableClothBackground table;
		for (int j = 0 ; j < size2 ; j ++)
		{
			const rapidjson::Value &arrayValue3 = json->getSubValue_json(arrayValue2,j);
			Background back;
			if (json->isHave(arrayValue3,"speed"))
			{
				back.speed = json->getFloatValue_json(arrayValue3,"speed",0);
			}
			else
			{
				CCASSERT(NULL,"GameSceneConfigManager : no key = [speed]");
			}
			if (json->isHave(arrayValue3,"zOrder"))
			{
				back.zOrder = json->getInt_json(arrayValue3,"zOrder",0);
			}
			if (json->isHave(arrayValue3,"YOffset"))
			{
				back.YOffset = json->getInt_json(arrayValue3,"YOffset",0);
			}
			if (json->isHave(arrayValue3,"CameraMoveFactor_Y"))
			{
				back.CameraMoveFactor_Y = json->getFloatValue_json(arrayValue3,"CameraMoveFactor_Y",0);
			}
			if (json->isHave(arrayValue3,"Scale"))
			{
				back.Scale = json->getFloatValue_json(arrayValue3,"Scale",1);
			}
			vector<string> list1;
			const rapidjson::Value &arrayValue4 = json->getSubValue_json(arrayValue3,"file");
			json->getArrayString_json(arrayValue4,list1);


			back.mapList = list1;
			backList.backList.push_back(back);
			
			
		}
		if (json->isHave(value,"scene"))
		{
			const rapidjson::Value &arrayValue1 = json->getSubValue_json(value,"scene");

			const rapidjson::Value &arrayValue2 = json->getSubValue_json(arrayValue1,"layers");
			int size3 = arrayValue2.Size();

			for (int k = 0 ; k < size3 ; k ++)
			{
				LayersBackground layers;
				const rapidjson::Value &arrayValue5 = json->getSubValue_json(arrayValue2,k);
				if (json->isHave(arrayValue5,"minRandomWidth"))
				{
					layers.minRandomWidth = json->getInt_json(arrayValue5,"minRandomWidth",0);
				}
				if (json->isHave(arrayValue5,"maxRandomWidth"))
				{
					layers.maxRandomWidth = json->getInt_json(arrayValue5,"maxRandomWidth",0);
				}
				if (json->isHave(arrayValue5,"speed"))
				{
					layers.speed = json->getFloatValue_json(arrayValue5,"speed",0);
				}
				if (json->isHave(arrayValue5,"zOrder"))
				{
					layers.zOrder = json->getInt_json(arrayValue5,"zOrder",0);
				}
				if (json->isHave(arrayValue5,"CameraMoveFactor_Y"))
				{
					layers.CameraMoveFactor_Y = json->getFloatValue_json(arrayValue5,"CameraMoveFactor_Y",0);
				}
				if (json->isHave(arrayValue5,"Scale"))
				{
					layers.Scale = json->getFloatValue_json(arrayValue5,"Scale",1);
				}
				const rapidjson::Value &arrayValue6 = json->getSubValue_json(arrayValue5,"layer");
				
				int size4 = arrayValue6.Size();
				for (int j = 0 ; j < size4 ; j ++)
				{
					const rapidjson::Value &arrayValue7 = json->getSubValue_json(arrayValue6,j);
					LayerBackground layer;
					if (json->isHave(arrayValue7,"fileName"))
					{
						layer.fileName = json->getString_json(arrayValue7,"fileName");
					}
					if (json->isHave(arrayValue7,"armatureName"))
					{
						layer.armatureName = json->getString_json(arrayValue7,"armatureName");
					}
					if (json->isHave(arrayValue7,"animateName"))
					{
						layer.animateName = json->getString_json(arrayValue7,"animateName");
					}
					if (json->isHave(arrayValue7,"tiggertID"))
					{
						layer.tiggertID = json->getInt_json(arrayValue7,"tiggertID",0);
					}
					if (json->isHave(arrayValue7,"actionID"))
					{
						layer.actionID = json->getInt_json(arrayValue7,"actionID",0);
					}
					if (json->isHave(arrayValue7,"YOffset"))
					{
						layer.YOffset = json->getInt_json(arrayValue7,"YOffset",0);
					}
					layers.layerList.push_back(layer);
				}
				scene.layersList.push_back(layers);

			}
			backList.scene = scene;
			//back.speed = 
		}
		else
		{
			//CCASSERT(NULL,"GameSceneConfigManager : no key = [scene]");
		}
		if (json->isHave(value,"TableCloth"))
		{
			const rapidjson::Value &arrayValue1 = json->getSubValue_json(value,"TableCloth");

			const rapidjson::Value &arrayValue2 = json->getSubValue_json(arrayValue1,"layers");
			int size3 = arrayValue2.Size();

			for (int k = 0 ; k < size3 ; k ++)
			{
				LayersBackground layers;
				const rapidjson::Value &arrayValue5 = json->getSubValue_json(arrayValue2,k);
				if (json->isHave(arrayValue5,"minRandomWidth"))
				{
					layers.minRandomWidth = json->getInt_json(arrayValue5,"minRandomWidth",0);
				}
				if (json->isHave(arrayValue5,"maxRandomWidth"))
				{
					layers.maxRandomWidth = json->getInt_json(arrayValue5,"maxRandomWidth",0);
				}
				if (json->isHave(arrayValue5,"speed"))
				{
					layers.speed = json->getFloatValue_json(arrayValue5,"speed",0);
				}
				if (json->isHave(arrayValue5,"zOrder"))
				{
					layers.zOrder = json->getInt_json(arrayValue5,"zOrder",0);
				}
				if (json->isHave(arrayValue5,"CameraMoveFactor_Y"))
				{
					layers.CameraMoveFactor_Y = json->getFloatValue_json(arrayValue5,"CameraMoveFactor_Y",0);
				}
				if (json->isHave(arrayValue5,"Scale"))
				{
					layers.Scale = json->getFloatValue_json(arrayValue5,"Scale",1);
				}
				const rapidjson::Value &arrayValue6 = json->getSubValue_json(arrayValue5,"layer");

				int size4 = arrayValue6.Size();
				for (int j = 0 ; j < size4 ; j ++)
				{
					const rapidjson::Value &arrayValue7 = json->getSubValue_json(arrayValue6,k);
					LayerBackground layer;
					if (json->isHave(arrayValue7,"fileName"))
					{
						layer.fileName = json->getString_json(arrayValue7,"fileName");
					}
					if (json->isHave(arrayValue7,"tiggertID"))
					{
						layer.tiggertID = json->getInt_json(arrayValue7,"tiggertID",0);
					}
					if (json->isHave(arrayValue7,"actionID"))
					{
						layer.actionID = json->getInt_json(arrayValue7,"actionID",0);
					}
					if (json->isHave(arrayValue7,"YOffset"))
					{
						layer.YOffset = json->getInt_json(arrayValue7,"YOffset",0);
					}
					layers.layerList.push_back(layer);
				}
				table.layersList.push_back(layers);

			}
			backList.table = table;
			//back.speed = 
		}
		else
		{
			//CCASSERT(NULL,"GameSceneConfigManager : no key = [scene]");
		}
		m_bgConfigPool.insert(map<unsigned int, BackgroundList >::value_type(id,backList));
	}
}
bool GameSceneConfigManager::readData(int index,Document& root)
{
	XReadJson *json = XReadJson::getIntanse();
	__String* str = __String::createWithFormat("/map_%d.txt",index);
	string fileName = DatatName + str->getCString();
	CCLOG("openFile %s :\n",fileName.c_str());
	if (json->getJsonRootValue(fileName,root))
	{
		return true;
	}
	else
	{
		return false;
	}
}

BackgroundList* GameSceneConfigManager::getBackground(unsigned int bgID)
{
	map<unsigned int, BackgroundList >::iterator iter = m_bgConfigPool.find(bgID);
	if (iter != m_bgConfigPool.end())
	{
		return &iter->second;
	}
	return nullptr;
}
int GameSceneConfigManager::getMeterByFightCount(unsigned int fightNum,int count)
{
	int meter = 0;
	if (fightNum >m_lFightCountList.size())
	{
		fightNum = 1;
	}
	for (auto iter = m_lFightCountList.begin() ; iter != m_lFightCountList.end() ; iter ++)
	{
		if (iter->first != fightNum)
		{
			meter += iter->second;
		}
		else
		{
			if (count <= iter->second)
			{
				meter += count;
			}
			else
			{
				meter += 1;
			}
			break;
		}
	}
	return meter;
}
unsigned int GameSceneConfigManager::getSceneCount(unsigned int id,Scene_PlayMode mode,int count)
{
	switch (mode)
	{
	case mode_Common:
		{
			map<unsigned int,MapInfoList>::iterator iter = m_lMapList.find(id);
			if (iter != m_lMapList.end())
			{
				return iter->second.count;
			}
		}
		break;
	case mode_Super:
		{
			map<unsigned int,MapInfoList>::iterator iter = m_lSuperMapList.find(id);
			if (iter != m_lSuperMapList.end())
			{
				return iter->second.count;
			}
		}
		break;
	case mode_Fight:
		{
			int meter = getMeterByFightCount(id,count);
			map<unsigned int,MapInfoList>::iterator iter = m_lFightMapList.find(meter);
			if (iter != m_lFightMapList.end())
			{
				return iter->second.count;
			}
		}
		break;
	default:
		break;
	}
	
	return 0;
}
const SceneItemInfo *GameSceneConfigManager::getTollgateSceneItems(string name,int &screenCount)
{
	m_lTollgateSceneItemList.list.clear();
	m_lTollgateSceneItemList.backList.clear();
	int index = 0;
	XReadJson *json = XReadJson::getIntanse();
	Document root;

	json->getJsonRootValue(TollgateRoot + name,root);

	const rapidjson::Value &arrayValue = json->getSubValue_json(root,"sceneItem");
	int size = arrayValue.Size();
	screenCount = 1;
	if (json->isHave(root,"screenCount"))
	{
		screenCount = json->getInt_json(root,"screenCount",0);
	}
	if (json->isHave(root,"backgroundID"))
	{
		const rapidjson::Value &arrayValue2 = json->getSubValue_json(root,"backgroundID");
		json->getArrayInt_json(arrayValue2,m_lTollgateSceneItemList.backList);
	}
	else
	{
		m_lTollgateSceneItemList.backList.push_back(1);
	}
	for (int i = 0 ; i < size ; i++)
	{
		SceneItem info;
		const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
		if (json->isHave(value,"ID"))
		{
			info.unit = const_cast<Unit*>(UnitConfigManager::getManager()->getOneUnit(json->getInt_json(value,"ID",0)));
		}
		if (json->isHave(value,"x"))
		{
			info.posX = json->getFloatValue_json(value,"x",0);
		}
		if (json->isHave(value,"y"))
		{
			info.posY = json->getFloatValue_json(value,"y",0);
		}
		if (json->isHave(value,"rotat"))
		{
			info.rotation = json->getInt_json(value,"rotat",0);
		}
		if (json->isHave(value,"random"))
		{
			info.chance = json->getInt_json(value,"random",0);
		}
		if (json->isHave(value,"actionID"))
		{
			info.actionID = json->getInt_json(value,"actionID",0);
		}
		if (json->isHave(value,"zOrder"))
		{
			info.zOrder = json->getInt_json(value,"zOrder",0);
		}
		if (json->isHave(value,"triggerID"))
		{
			vector<int> list;
			const rapidjson::Value &array = json->getSubValue_json(value,"triggerID");
			if (array.IsArray())
			{
				json->getArrayInt_json(array,list);
				for (vector<int>::iterator iter = list.begin() ; iter != list.end() ; iter++)
				{
					info.triggerID.push_back(*iter);
				}
			}

		}
		if (json->isHave(value,"alpha"))
		{
			info.alpha = json->getInt_json(value,"alpha",0);
		}
		if (json->isHave(value,"scale"))
		{
			info.scale = json->getFloatValue_json(value,"scale",0);
		}
		if (json->isHave(value,"rect"))
		{
			const rapidjson::Value &value1 = json->getSubValue_json(value,"rect");
			if (!value1.IsNull())
			{
				if (json->isHave(value1,"x"))
				{
					info.rect.origin.x = json->getFloatValue_json(value1,"x",0);
				}
				if (json->isHave(value1,"y"))
				{
					info.rect.origin.y = json->getFloatValue_json(value1,"y",0);
				}
				if (json->isHave(value1,"width"))
				{
					info.rect.size.width = json->getFloatValue_json(value1,"width",0);
				}
				if (json->isHave(value1,"height"))
				{
					info.rect.size.height = json->getFloatValue_json(value1,"height",0);
				}
			}
		}
		m_lTollgateSceneItemList.list.push_back(info);
	}
	sort(m_lTollgateSceneItemList.list.begin(), m_lTollgateSceneItemList.list.end(), sortByPosX);
	return &m_lTollgateSceneItemList;
}

#if 0
const SceneItemInfo* GameSceneConfigManager::getSceneItems(unsigned int sceneID,int num,Scene_PlayMode playMode,int &screenCount,RunningMode mode,int count)
{
	int index = 0;
	
	if (mode == RunningMode::tollgate)
	{
		const TollgateItem *item = TollgateHelper::getHelper()->getOneTollgate(sceneID);
		if (item)
		{
			return getTollgateSceneItems(item->mapName,screenCount);
		}
		return nullptr;
	}
	else
	{
		switch (playMode)
		{
		case mode_Common:
			{
				map<unsigned int,MapInfoList>::iterator iter = m_lMapList.find(sceneID);
				if (iter != m_lMapList.end())
				{
					if (num > iter->second.count)
					{
						return nullptr;
					}
					index = iter->second.startIndex + num - 1;
				}
				else
				{
					return nullptr;
				}
			}
			{
				{
					map<unsigned int,unsigned int >::iterator iter = m_nScreenCount.find(index);
					if (iter != m_nScreenCount.end())
					{
						screenCount = iter->second;
					}
				}
				{
					map<unsigned int,SceneItemInfo >::iterator iter = m_sceneItemsConfigPool.find(index);
					if (iter != m_sceneItemsConfigPool.end())
					{
						return &iter->second;
					}
					return nullptr;
				}

			}
			break;
		case mode_Super:
			{
				map<unsigned int,MapInfoList>::iterator iter = m_lSuperMapList.find(sceneID);
				if (iter != m_lSuperMapList.end())
				{
					if (num > iter->second.count)
					{
						return nullptr;
					}
					index = iter->second.startIndex + num - 1;
				}
				else
				{
					return nullptr;
				}
			}
			{
				{
					map<unsigned int,unsigned int >::iterator iter = m_nSuperScreenCount.find(index);
					if (iter != m_nSuperScreenCount.end())
					{
						screenCount = iter->second;
					}
				}
				{
					map<unsigned int,SceneItemInfo >::iterator iter = m_SupersceneItemsConfigPool.find(index);
					if (iter != m_SupersceneItemsConfigPool.end())
					{
						return &iter->second;
					}
					return nullptr;
				}
			}
			break;
		case mode_Fight:
			{
				int meter = getMeterByFightCount(sceneID,count);
				map<unsigned int,MapInfoList>::iterator iter = m_lFightMapList.find(meter);
				if (iter != m_lFightMapList.end())
				{
					if (num > iter->second.count)
					{
						return nullptr;
					}
					index = iter->second.startIndex + num - 1;
				}
				else
				{
					return nullptr;
				}
			
				{
					map<unsigned int,unsigned int >::iterator iter = m_nFightScreenCount.find(index);
					if (iter != m_nFightScreenCount.end())
					{
						screenCount = iter->second;
					}
				}
				{
					map<unsigned int,SceneItemInfo >::iterator iter = m_FightsceneItemsConfigPool.find(index);
					if (iter != m_FightsceneItemsConfigPool.end())
					{
						return &iter->second;
					}
					return nullptr;
				}

			}
			break;
		default:
			break;
		}
	}
	if (playMode == mode_Super)
	{
		
	}
	else
	{
		

		
	}
	return nullptr;
}
#endif

//关卡模式 （大关id， 小关id， 难度）
const SceneItemInfo* GameSceneConfigManager::getSceneItems(unsigned int seasonId, unsigned int sectionId, int difficultLevel, int &screenCount)
{
	const Section *item = TollgateHelper::getHelper()->getOneTollgateSection(seasonId, sectionId);
	if (item)
	{
		return getTollgateSceneItems(item->mapName,screenCount);
	}
	return nullptr;
}
const MapInfoList* GameSceneConfigManager::getOnePhaseMapInfo(unsigned int phaseId)
{
	map<unsigned int,MapInfoList>::iterator iter = m_lMapList.find(phaseId);
	if (iter != m_lMapList.end())
		return &iter->second;
	else 
		return nullptr;
}
//无尽模式  
//1. mode 模式(超跑、战斗、正常)
//2. phaseId 阶段id(第一场，第二场)
//3. mapIdx 阶段地图索引(cpp_map?_1，cpp_map?_2)
//4. screencount _输出_ 屏数
const SceneItemInfo* GameSceneConfigManager::getSceneItems(Scene_PlayMode mode, unsigned int phaseId, unsigned int mapIdx, int &screenCount)
{
	int index;
	switch (mode)
	{
	case mode_Common:
		{
			map<unsigned int,MapInfoList>::iterator iter = m_lMapList.find(phaseId);
			if (iter != m_lMapList.end())
			{
				if (mapIdx > iter->second.count)
				{
					return nullptr;
				}
				index = iter->second.startIndex + mapIdx - 1;
			}
			else
			{
				return nullptr;
			}
		}
		{
			{
				map<unsigned int,unsigned int >::iterator iter = m_nScreenCount.find(index);
				if (iter != m_nScreenCount.end())
				{
					screenCount = iter->second;
				}
			}
			{
				map<unsigned int,SceneItemInfo >::iterator iter = m_sceneItemsConfigPool.find(index);
				if (iter != m_sceneItemsConfigPool.end())
				{
					return &iter->second;
				}
				return nullptr;
			}

		}
		break;
	case mode_Super:
		{
			map<unsigned int,MapInfoList>::iterator iter = m_lSuperMapList.find(phaseId);
			if (iter != m_lSuperMapList.end())
			{
				if (mapIdx > iter->second.count)
				{
					return nullptr;
				}
				index = iter->second.startIndex + mapIdx - 1;
			}
			else
			{
				return nullptr;
			}
		}
		{
			{
				map<unsigned int,unsigned int >::iterator iter = m_nSuperScreenCount.find(index);
				if (iter != m_nSuperScreenCount.end())
				{
					screenCount = iter->second;
				}
			}
			{
				map<unsigned int,SceneItemInfo >::iterator iter = m_SupersceneItemsConfigPool.find(index);
				if (iter != m_SupersceneItemsConfigPool.end())
				{
					return &iter->second;
				}
				return nullptr;
			}
		}
		break;
	case mode_Fight:
		{
			//int meter = getMeterByFightCount(sceneID,count);
			map<unsigned int,MapInfoList>::iterator iter = m_lFightMapList.find(phaseId);
			if (iter != m_lFightMapList.end())
			{
				if (mapIdx > iter->second.count)
				{
					return nullptr;
				}
				index = iter->second.startIndex + mapIdx - 1;
			}
			else
			{
				return nullptr;
			}

			{
				map<unsigned int,unsigned int >::iterator iter = m_nFightScreenCount.find(index);
				if (iter != m_nFightScreenCount.end())
				{
					screenCount = iter->second;
				}
			}
			{
				map<unsigned int,SceneItemInfo >::iterator iter = m_FightsceneItemsConfigPool.find(index);
				if (iter != m_FightsceneItemsConfigPool.end())
				{
					return &iter->second;
				}
				return nullptr;
			}

		}
		break;
	default:
		break;
	}

	return nullptr;
}

unsigned int GameSceneConfigManager::getBackgroundCount()
{
	return m_bgConfigPool.size();
}
unsigned int GameSceneConfigManager::getSceneItemCount(Scene_PlayMode mode)
{
	switch (mode)
	{
	case mode_Common:
		{
			return m_sceneItemsConfigPool.size();
		}
		break;
	case mode_Super:
		{
			return m_SupersceneItemsConfigPool.size();
		}
		break;
	case mode_Fight:
		{
			return m_FightsceneItemsConfigPool.size();
		}
		break;
	default:
		break;
	}
	return 0;
	
}
unsigned int GameSceneConfigManager::getMaxMeter(Scene_PlayMode mode)
{
	unsigned int meter = 0;
	map<Scene_PlayMode,unsigned int>::iterator iter = m_lMaxMeterList.find(mode);
	if (iter != m_lMaxMeterList.end())
	{
		meter = iter->second;
	}
	return meter;
}
void GameSceneConfigManager::getScenItemFromFile(const string &file,int &screenCount,vector<SceneItem> &list,vector<int> &backList)
{
	list.clear();
	int index = 0;
	XReadJson *json = XReadJson::getIntanse();
	Document root;

	json->getJsonRootValue(file,root);;

	getSceneItemFromJson(root,screenCount,list,backList);
	//return &m_lTollgateSceneItemList;
}
void GameSceneConfigManager::getSceneItemFromJson(const rapidjson::Value& root,int &screenCount,vector<SceneItem> &list,vector<int> &backList)
{
	XReadJson *json = XReadJson::getIntanse();
	if (!json->isHave(root,"sceneItem"))
	{
		return;
	}
	const rapidjson::Value &arrayValue = json->getSubValue_json(root,"sceneItem");
	if (arrayValue.IsNull() || !arrayValue.IsArray())
	{
		return;
	}
	int size = arrayValue.Size();
	screenCount = 1;
	
	for (int i = 0 ; i < size ; i++)
	{
		SceneItem info;
		const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
		if (json->isHave(value,"ID"))
		{
			info.unit = const_cast<Unit*>(UnitConfigManager::getManager()->getOneUnit(json->getInt_json(value,"ID",0)));
		}
		if (json->isHave(value,"x"))
		{
			info.posX = json->getFloatValue_json(value,"x",0);
		}
		if (json->isHave(value,"y"))
		{
			info.posY = json->getFloatValue_json(value,"y",0);
		}
		if (json->isHave(value,"rotat"))
		{
			info.rotation = json->getInt_json(value,"rotat",0);
		}
		if (json->isHave(value,"random"))
		{
			info.chance = json->getInt_json(value,"random",0);
		}
		if (json->isHave(value,"actionID"))
		{
			info.actionID = json->getInt_json(value,"actionID",0);
		}
		if (json->isHave(value,"zOrder"))
		{
			info.zOrder = json->getInt_json(value,"zOrder",0);
		}
		if (json->isHave(value,"triggerID"))
		{
			vector<int> list;
			const rapidjson::Value &array = json->getSubValue_json(value,"triggerID");
			if (array.IsArray())
			{
				json->getArrayInt_json(array,list);
				for (vector<int>::iterator iter = list.begin() ; iter != list.end() ; iter++)
				{
					info.triggerID.push_back(*iter);
				}
			}

		}
		if (json->isHave(value,"alpha"))
		{
			info.alpha = json->getInt_json(value,"alpha",0);
		}
		if (json->isHave(value,"scale"))
		{
			info.scale = json->getFloatValue_json(value,"scale",0);
		}

		if (json->isHave(value,"rect"))
		{
			const rapidjson::Value &value1 = json->getSubValue_json(value,"rect");
			if (!value1.IsNull())
			{
				if (json->isHave(value1,"x"))
				{
					info.rect.origin.x = json->getFloatValue_json(value1,"x",0);
				}
				if (json->isHave(value1,"y"))
				{
					info.rect.origin.y = json->getFloatValue_json(value1,"y",0);
				}
				if (json->isHave(value1,"width"))
				{
					info.rect.size.width = json->getFloatValue_json(value1,"width",0);
				}
				if (json->isHave(value1,"height"))
				{
					info.rect.size.height = json->getFloatValue_json(value1,"height",0);
				}
			}
		}
		list.push_back(info);
	}

	if (json->isHave(root,"sceneItemList"))
	{
		const rapidjson::Value &value = json->getSubValue_json(root,"sceneItemList");
		if (!value.IsNull())
		{
			int size = value.Size();
			for (int i = 0 ; i < size; i++)
			{
				const rapidjson::Value &value1 = json->getSubValue_json(value,i);
				getSceneItemFromJson(value1,screenCount,list,backList);
			}
			
		}
	}
	if (json->isHave(root,"screenCount"))
	{
		screenCount = json->getInt_json(root,"screenCount",0);
	}
	if (json->isHave(root,"backgroundID"))
	{
		const rapidjson::Value &arrayValue2 = json->getSubValue_json(root,"backgroundID");
		json->getArrayInt_json(arrayValue2,backList);
	}
	else
	{
		backList.push_back(1);
	}
	sort(list.begin(), list.end(), sortByPosX);
}
void GameSceneConfigManager::initFightData()
{
	FightTotalMapManager *manager = FightTotalMapManager::getManager();
	vector<string> *maps_list = manager->getTotalFightSceneItems();
	m_lMaxMeterList.insert(map<Scene_PlayMode,unsigned int>::value_type(mode_Fight,manager->getMaxMeter()));
	int count = 0;
	int number = 0;
	int index = 0;

	//totalMap list
	for (vector<string>::iterator iter = maps_list->begin() ; iter != maps_list->end() ; iter ++)
	{
		count ++;

		
		// one fight map list,  include one or more cpp_map
		XReadJson *json = XReadJson::getIntanse();
		map<int,vector<string> > map_list;
		{
			Document root;
			if (json->getJsonRootValue(FightRoot + *iter,root))
			{
				const rapidjson::Value &arrayValue = json->getSubValue_json(root,"maps");
				if (json->isHave(arrayValue,"MAX_Meter"))
				{
					unsigned int meter = json->getInt_json(arrayValue,"MAX_Meter",0);
					m_lFightCountList.insert(map<int,int>::value_type(count,meter));
				}
				{
					const rapidjson::Value &arrayValue = json->getSubValue_json(root,"map");
					int size = arrayValue.Size();
					for (int i = 0 ; i < size; i++)
					{
						const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
						vector<string> list1;
						json->getArrayString_json(value,list1);
						map_list.insert(map<int,vector<string> >::value_type(i + 1,list1));
					}
				}
			}
		}

		
		//这里的每一个iter代表 每一场战斗  ， vector<string>表示每一场战斗可能有多个 cpp_map组成
		for (map<int,vector<string> >::iterator iter = map_list.begin() ; iter != map_list.end() ; iter++)
		{
			Document root;
			number ++;
			unsigned int meter = number;
			int startIndex = index;
			int map_count = 0;
			//每一个cpp_map
			for (vector<string>::iterator iter_name = iter->second.begin(); iter_name != iter->second.end() ; iter_name++)
			{

				vector<SceneItem> list;
				vector<int> backList;
				SceneItemInfo info;
				int screen_count = 1;
				getScenItemFromFile(FightRoot + *iter_name,screen_count,list,backList);
				info.backList = backList;
				info.list = list;
				m_nFightScreenCount.insert(map<unsigned int,unsigned int >::value_type(index,screen_count) );
				m_FightsceneItemsConfigPool.insert(map<unsigned int,SceneItemInfo >::value_type(index,info) );
				index++;
				map_count++;
			}
			MapInfoList mapList;
			mapList.meters = meter;
			mapList.startIndex = startIndex;
			mapList.count = map_count;

			m_lFightMapList.insert(map<unsigned int , MapInfoList>::value_type(meter,mapList));
		}
	}
	
}