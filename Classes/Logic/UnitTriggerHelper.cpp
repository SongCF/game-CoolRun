
#include "UnitActionHelper.h"
#include "UnitTriggerHelper.h"

// UnitTriggerHelper* UnitTriggerHelper::m_sTriggerHelper;
// 
// UnitTriggerHelper::UnitTriggerHelper()
// {
// 
// }
// 
// UnitTriggerHelper* UnitTriggerHelper::getHelper()
// {
// 	if (! m_sTriggerHelper) m_sTriggerHelper = new UnitTriggerHelper;
// 	return m_sTriggerHelper;
// }


const Trigger* UnitTriggerHelper::getOneTrigger(unsigned int triggerId)
{
	return TriggerConfigManager::getManager()->getTriggerItemByID(triggerId);
}


void UnitTriggerHelper::runTriggerActionById(cocos2d::Node* target, unsigned int trigger)
{
	const Trigger* trig = getOneTrigger(trigger);
	if (trig){
		UnitActionHelper::runActionByTrigger(target, trig);
	}
}