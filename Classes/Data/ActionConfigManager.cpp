#include "ActionConfigManager.h"
#include "cocos2d.h"

static string DatatName = "config/Action.txt";
ActionConfigManager* ActionConfigManager::s_mActionConfigManager = nullptr;


ActionConfigManager* ActionConfigManager::getManager()
{
	if (! s_mActionConfigManager) {
		s_mActionConfigManager = new ActionConfigManager;
		s_mActionConfigManager->initData();
	}
	return s_mActionConfigManager;
}

ActionConfigManager::ActionConfigManager()
{
}
ActionConfigManager::~ActionConfigManager()
{

}
configActionID ActionConfigManager::getTypeById(unsigned int actionID)
{
	CCASSERT(false, "error call");
	//统一使用actionID，不用再转换一次
// 	if (actionID == 1) return action_FeiXingBi;
// 	else if (actionID == 2) return action_AiShuZhangChu;
// 	else if (actionID == 3) return action_GaoShuZhangChu;
// 	else if (actionID == 4) return action_YeZhuHuXi;
// 	else if (actionID == 7) return action_MiFengFeiXing;
// 	else if (actionID == 16) return action_ChongJi;
// 
 	return action_None;
}

const vector<ActionItem>* ActionConfigManager::getAllActionItem()
{
	return &m_ActionList;
}
const ActionItem* ActionConfigManager::getActionItemByID(unsigned int actionID)
{
	ActionItem *action = NULL;
	for (vector<ActionItem>::iterator iter = m_ActionList.begin() ; iter != m_ActionList.end(); iter ++)
	{
		if (iter->ID == actionID)
		{
			action = &(*iter);
			break;
		}
	}

	if (! action) CCLOG("actionID = %d", actionID);
	CCASSERT(action,"Not has this Achieve ID");
	return action;
}

void ActionConfigManager::initData()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	json->getJsonRootValue(DatatName,root);
	CCLOG("openFile %s :\n",DatatName.c_str());
	const Value &arrayValue = json->getSubValue_json(root,"action");
	int size = arrayValue.Size();
	for (int i = 0 ; i < size ; i++)
	{
		ActionItem info;
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
		if (json->isHave(value,"type"))
		{
			info.type = json->getInt_json(value,"type",0);
		}
		if (json->isHave(value,"musicFile"))
		{
			info.musicFile = json->getString_json(value,"musicFile");
		}
		if (json->isHave(value,"actionFile"))
		{
			info.actionFile = json->getString_json(value,"actionFile");
		}
		if (json->isHave(value,"value"))
		{
			info.value = json->getFloatValue_json(value,"value",0);
		}
		m_ActionList.push_back(info);
	}
}