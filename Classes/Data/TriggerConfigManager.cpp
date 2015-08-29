#include "TriggerConfigManager.h"
#include "cocos2d.h"

static string DatatName = "config/Trigger.txt";
TriggerConfigManager* TriggerConfigManager::s_mTriggerConfigManager = nullptr;


TriggerConfigManager* TriggerConfigManager::getManager()
{
	if (! s_mTriggerConfigManager) {
		s_mTriggerConfigManager = new TriggerConfigManager;
		s_mTriggerConfigManager->initData();
	}
	return s_mTriggerConfigManager;
}

TriggerConfigManager::TriggerConfigManager()
{
}
TriggerConfigManager::~TriggerConfigManager()
{

}
const vector<Trigger>* TriggerConfigManager::getAllTriggerItem()
{
	return &m_TriggerList;
}
const Trigger* TriggerConfigManager::getTriggerItemByID(unsigned int triggerID)
{
	Trigger *trigger = NULL;
	for (vector<Trigger>::iterator iter = m_TriggerList.begin() ; iter != m_TriggerList.end(); iter ++)
	{
		if (iter->ID == triggerID)
		{
			trigger = &(*iter);
			break;
		}
	}
	if (! trigger) CCLOG("triggerID = %d", triggerID);
	CCASSERT(trigger,"Not has this Achieve ID");
	return trigger;
}

void TriggerConfigManager::initData()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	json->getJsonRootValue(DatatName,root);
	CCLOG("openFile %s :\n",DatatName.c_str());
	const Value &arrayValue = json->getSubValue_json(root,"trigger");
	int size = arrayValue.Size();
	for (int i = 0 ; i < size ; i++)
	{
		Trigger info;
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
		if (json->isHave(value,"actionID"))
		{
			info.actionID = json->getInt_json(value,"actionID",0);
		}
		if (json->isHave(value,"musicFile"))
		{
			info.musicFile = json->getString_json(value,"musicFile");
		}
		if (json->isHave(value,"distance"))
		{
			info.distance = json->getInt_json(value,"distance",0);
		}
		
		m_TriggerList.push_back(info);
	}
}