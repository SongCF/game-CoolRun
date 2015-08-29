#include "UnitConfigManager.h"
#include "cocos2d.h"
#include <string.h>
#include "DataParse.h"
#include "VersionConfig.h"
static string DatatName = "config/Unit.txt";
static string SkinName = "config/skin.txt";
UnitConfigManager* UnitConfigManager::s_mUnitConfigManager = nullptr;


static int unitCompareByID(const void* a, const void* b)
{
	return ((Unit*)a)->ID - ((Unit*)b)->ID;
}


UnitConfigManager* UnitConfigManager::getManager()
{
	if (! s_mUnitConfigManager) {
		s_mUnitConfigManager = new UnitConfigManager;
		s_mUnitConfigManager->initData();
	}
	return s_mUnitConfigManager;
}

UnitConfigManager::UnitConfigManager()
{
}

void UnitConfigManager::initData()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	CCLOG("%s ---> start parse file %s", __FUNCTION__, DatatName.c_str());
	bool parseRet = json->getJsonRootValue(DatatName,root);
	CCLOG("%s ---> Parse %s", __FUNCTION__, parseRet?"true":"false");
	initUnitData(root,"special",unittype_Special);
	initUnitData(root,"actor",unittype_Actor);
	initUnitData(root,"pet",unittype_Pet);
	initUnitData(root,"magic",unittype_Magic);
	initUnitData(root,"land",unittype_Land);
	initUnitData(root,"items",unittype_Item);
	initUnitData(root,"obstacle",unittype_Obstacle);
	initUnitData(root,"tools",unittype_Tool);
	initUnitData(root,"decorative",unittype_Decorative);

	CCLOG("%s ---> start parse file %s", __FUNCTION__, DatatName.c_str());
	parseRet = json->getJsonRootValue(SkinName,root);
	CCLOG("%s ---> Parse %s", __FUNCTION__, parseRet?"true":"false");
	initUnitData(root,"skin",unittype_Skin);
	/*{//item的
		const Value &arrayValue = json->getSubValue_json(root,"items");
		int size = arrayValue.Size();
//		CCLOG("parse items, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}
			
			info.type = unittype_Item;
			m_lunitVec.push_back(info);
		}
	}
	{//障碍物的
		const Value &arrayValue = json->getSubValue_json(root,"obstacle");
		int size = arrayValue.Size();
//		CCLOG("parse obstacle, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}

			info.type = unittype_Obstacle;
			m_lunitVec.push_back(info);
		}
	}
	{//actor
		const Value &arrayValue = json->getSubValue_json(root,"actor");
		int size = arrayValue.Size();
//		CCLOG("parse actors, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}
			info.type = unittype_Actor;
			m_lunitVec.push_back(info);
		}
	}
	{//pet
		const Value &arrayValue = json->getSubValue_json(root,"pet");
		int size = arrayValue.Size();
//		CCLOG("parse pets, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}
			info.type = unittype_Pet;
			m_lunitVec.push_back(info);
		}
	}
	{//Magic
		const Value &arrayValue = json->getSubValue_json(root,"magic");
		int size = arrayValue.Size();
//		CCLOG("parse magics, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}
			info.type = unittype_Magic;
			m_lunitVec.push_back(info);
		}
	}
	{//藤条
		const Value &arrayValue = json->getSubValue_json(root,"land");
		int size = arrayValue.Size();
//		CCLOG("parse land, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}
			info.type = unittype_Land;
			m_lunitVec.push_back(info);
		}
	}
	{//special的
		const Value &arrayValue = json->getSubValue_json(root,"special");
		int size = arrayValue.Size();
		//		CCLOG("parse items, count = %d",size);
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}

			info.type = unittype_Item;
			m_lunitVec.push_back(info);
		}
	}*/
//	CCLOG("%s ---> parse end!", __FUNCTION__);
	qsort(&m_lunitVec[0], m_lunitVec.size(), sizeof(Unit), unitCompareByID);
//	CCLOG("%s ---> sort unit end", __FUNCTION__);
}

const Unit* UnitConfigManager::getOneUnit(unsigned int id)
{
	Unit* p = (Unit* )bsearch(&id, &m_lunitVec[0], m_lunitVec.size(), sizeof(Unit), unitCompareByID);
	if (p) {
		return p;
	}
	else {
		if (1 == DEBUG_uMessageBox)
		{
			cocos2d::MessageBox(cocos2d::StringUtils::format("not found unit by Id=[%d]", id).c_str(), "ERROR");
			CCASSERT(false, "");
		}
		return nullptr;
	}
}

const vector<Unit>* UnitConfigManager::getAllUnit()
{
	return &m_lunitVec;
}
void UnitConfigManager::initUnitData(const rapidjson::Value& root,const string& name,UnitType type)
{
	XReadJson *json = XReadJson::getIntanse();
	{
		const Value &arrayValue = json->getSubValue_json(root,name.c_str());
		int size = arrayValue.Size();
		for (int i = 0 ; i < size ; i++)
		{
			Unit info;
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
				info.descrigbe = json->getString_json(value,"describe");
			}
			if (json->isHave(value,"imgFile"))
			{
				info.imageFile = json->getString_json(value,"imgFile");
			}
			if (json->isHave(value,"skinID"))
			{
				info.skinIDList.clear();
				const Value &arrayValue = json->getSubValue_json(value,"skinID");
				for (int iTemp=0; iTemp<arrayValue.Size(); ++iTemp)
				{
					info.skinIDList.push_back(vector<int>());
					json->getArrayInt_json(arrayValue[iTemp],info.skinIDList[iTemp]);
				}
			}
			if (json->isHave(value,"music"))
			{
				info.musicFile = json->getString_json(value,"music");
			}
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
				info.attribute = att_list;
			}
			if (json->isHave(value,"rect"))
			{
				const Value &arrayValue1 = json->getSubValue_json(value,"rect");
				vector<cocos2d::Rect> rectList;
				int size = arrayValue1.Size();
				for (int j = 0 ; j < size ;  j++)
				{
					const Value &value1 = json->getSubValue_json(arrayValue1,j);
					string rectStr = json->getString_json(value1);
					cocos2d::Rect rect;
					UnitRect unitRect;
					DataParse::rectParse(rectStr,unitRect);
					rect.origin.x = unitRect.x;
					rect.origin.y = unitRect.y;
					rect.size.width = unitRect.width;
					rect.size.height = unitRect.height;
					rectList.push_back(rect);

				}
				info.rect = rectList;
			}

			info.type = type;
			m_lunitVec.push_back(info);
		}
	}
}