#include "FightTotalMapManager.h"
static string FightDatatName = "config/maps/Fight/TotalMap.txt";
static string FightRoot = "config/maps/Fight/";
FightTotalMapManager* FightTotalMapManager::s_mFightTotalMapManager = nullptr;
FightTotalMapManager::FightTotalMapManager()
{

}
FightTotalMapManager::~FightTotalMapManager()
{

}
void FightTotalMapManager::initData()
{
	XReadJson *json = XReadJson::getIntanse();
	//map<int,vector<string> > map_list;
	{
		Document root;
		json->getJsonRootValue(FightDatatName,root);
		{
			const rapidjson::Value &arrayValue = json->getSubValue_json(root,"maps");
			if (json->isHave(arrayValue,"MAX_Meter"))
			{
				unsigned int meter = json->getInt_json(arrayValue,"MAX_Meter",0);
				m_nMaxMeter = meter;
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
				for (auto iter = list1.begin() ; iter != list1.end(); iter ++)
				{
					m_lTotalSceneItemList.push_back(*iter);
				}
				//map_list.insert(map<int,vector<string> >::value_type(i + 1,list1));
			}
		}

	}
}
FightTotalMapManager* FightTotalMapManager::getManager()
{
	if (!s_mFightTotalMapManager) {
		s_mFightTotalMapManager = new FightTotalMapManager;
		s_mFightTotalMapManager->initData();
	}
	return s_mFightTotalMapManager;
}
unsigned int FightTotalMapManager::getMaxMeter()
{
	return m_nMaxMeter;
}
vector<string> *FightTotalMapManager::getTotalFightSceneItems()
{
	return &m_lTotalSceneItemList;
}