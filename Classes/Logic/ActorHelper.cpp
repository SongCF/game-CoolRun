#include "ActorHelper.h"
#include "UnitConfigManager.h"
#include "BagDataManager.h"
#include "LevelDataConfigManager.h"
#include "cocos2d.h"
#include "MsgDefine.h"
#include "sdk/SdkManager.h"
#define UnitTypeSelect(__id__,__start__,__end__) (__id__ >= __start__ && __id__ <= __end__)
using namespace cocos2d;
ActorHelper* ActorHelper::s_mActorHelper = nullptr;


static unsigned int s_levelUpUnitID = 0;
ActorHelper::ActorHelper()
{
	EventListenerCustom* listener = EventListenerCustom::create(MSG_PayResult, [&](EventCustom* evt) 
	{
		bool bIsLvMsg = false;
		MsgData* payData=(MsgData*)evt->getUserData();
		int gameProductID = payData->value;
		if (gameProductID == ProductID_RoleLevelUp || gameProductID == ProductID_MagicLevelUp)
		{
			bIsLvMsg = true;
			if (payData->errorCode == Rsp_Success)
			{
				BagDataManager::getManager()->changeOneUnitLevel(s_levelUpUnitID,1,0);
			}
		}
		else if (gameProductID == ProductID_RoleFullLevel || gameProductID == ProductID_MagicFullLevel)
		{
			bIsLvMsg = true;
			if (payData->errorCode == Rsp_Success)
			{
				int maxLv = ActorHelper::getHelper()->getMaxLevel(s_levelUpUnitID);
				int curLv = BagDataManager::getManager()->getOneUnit(s_levelUpUnitID)->value;
				BagDataManager::getManager()->changeOneUnitLevel(s_levelUpUnitID,maxLv-curLv,0);
			}
		}

		//
		if (bIsLvMsg)
		{
			log("ActorHelper ---> on MSG_PayResult");
			int errCode = payData->errorCode;
			Director::getInstance()->getRunningScene()->runAction(Sequence::create(
				DelayTime::create(1),
				CallFunc::create([=](){
					log("lambda ---> begin dispatch MSG_LevelUp_Rsp");
					EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					EventCustom event(MSG_LevelUp_Rsp);
					MsgData data;
					data.errorCode = errCode;
					event.setUserData(&data);
					eventDspc->dispatchEvent(&event);
					log("lambda ---> end dispatch MSG_LevelUp_Rsp");
			}),
				nullptr));
		}

	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,-1);  //优先级应该比UI层高
}

ActorHelper* ActorHelper::getHelper()
{
	if (! s_mActorHelper) s_mActorHelper = new ActorHelper;
	return s_mActorHelper;
}

//所有角色
const vector<Actor> ActorHelper::getAllHero()
{
	const vector<Unit> *unitList = UnitConfigManager::getManager()->getAllUnit();
	vector<Actor> list;
	for (vector<Unit>::const_iterator iter = unitList->begin() ; iter != unitList->end() ; iter ++)
	{
		if (iter->type == unittype_Actor)
		{
			Actor actor;
			actor.unit.ID = iter->ID;
			actor.unit.attribute = iter->attribute;
			actor.unit.descrigbe = iter->descrigbe;
			actor.unit.imageFile = iter->imageFile;
			actor.unit.name = iter->name;
			actor.unit.type = iter->type;
			actor.unit.rect = iter->rect;
			const BagItem *item = BagDataManager::getManager()->getOneUnit(actor.unit.ID);
			if (item)
			{
				actor.level = item->value;
			}
			else
			{
				actor.level = 0;
			}
			list.push_back(actor);
		}
		
	}
	return list;
}

//所有宠物
const vector<Actor> ActorHelper::getAllPet()
{
	const vector<Unit> *unitList = UnitConfigManager::getManager()->getAllUnit();
	vector<Actor> list;
	for (vector<Unit>::const_iterator iter = unitList->begin() ; iter != unitList->end() ; iter ++)
	{
		if (iter->type == unittype_Pet)
		{
			Actor actor;
			actor.unit.ID = iter->ID;
			actor.unit.attribute = iter->attribute;
			actor.unit.descrigbe = iter->descrigbe;
			actor.unit.imageFile = iter->imageFile;
			actor.unit.name = iter->name;
			actor.unit.type = iter->type;
			actor.unit.rect = iter->rect;
			const BagItem *item = BagDataManager::getManager()->getOneUnit(actor.unit.ID);
			if (item)
			{
				actor.level = item->value;
			}
			else
			{
				actor.level = 0;
			}
			list.push_back(actor);
		}

	}
	return list;
}
//所有魔法物
const vector<Actor> ActorHelper::getAllMagic()
{
	const vector<Unit> *unitList = UnitConfigManager::getManager()->getAllUnit();
	vector<Actor> list;
	for (vector<Unit>::const_iterator iter = unitList->begin() ; iter != unitList->end() ; iter ++)
	{
		if (iter->type == unittype_Magic)
		{
			Actor actor;
			actor.unit.ID = iter->ID;
			actor.unit.attribute = iter->attribute;
			actor.unit.descrigbe = iter->descrigbe;
			actor.unit.imageFile = iter->imageFile;
			actor.unit.name = iter->name;
			actor.unit.type = iter->type;
			actor.unit.rect = iter->rect;
			const BagItem *item = BagDataManager::getManager()->getOneUnit(actor.unit.ID);
			if (item)
			{
				actor.level = item->value;
			}
			else
			{
				actor.level = 0;
			}
			list.push_back(actor);
		}

	}
	return list;
}


void ActorHelper::fullLevelUp(unsigned int unitID)
{
	int product_id = 0;

	s_levelUpUnitID = unitID;
	if (unitID >= ID_Actor_Start && unitID <= ID_Actor_End){
		//SdkManager::getInstance()->pay(ProductID_RoleFullLevel);
		product_id = ProductID_RoleFullLevel;
	}
	else if (unitID >= ID_Magic_Start && unitID <= ID_Magic_End){
		//SdkManager::getInstance()->pay(ProductID_MagicFullLevel);
		product_id = ProductID_MagicFullLevel;
	}
	else {
		CCASSERT(false, "error!");
	}

	//test win32
#if DEBUG_ALWAYS_BUY_SUCCESS == 1
	EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	EventCustom event(MSG_PayResult);
	MsgData data;
	data.value = product_id;
	data.errorCode = Rsp_Success;
	event.setUserData(&data);
	eventDspc->dispatchEvent(&event);
#endif
}
void ActorHelper::levelUp(unsigned int heroOrPet_unitID)
{
	const BagItem *item = BagDataManager::getManager()->getOneUnit(heroOrPet_unitID);

	if (item)
	{
		int max_level = LevelDataConfigManager::getManager()->getMaxLevel(heroOrPet_unitID);
		if (max_level > item->value)
		{
			const OneLevelInfo *info = LevelDataConfigManager::getManager()->getOneUnitLevelInfo(heroOrPet_unitID,item->value + 1);

			//0. 如果是付费
			if (info->needUnit->ID == unitID_RMB)
			{
				int product_id = 0;

				s_levelUpUnitID = heroOrPet_unitID;
				if (heroOrPet_unitID >= ID_Actor_Start && heroOrPet_unitID <= ID_Actor_End){
					//SdkManager::getInstance()->pay(ProductID_RoleLevelUp);
					product_id = ProductID_RoleLevelUp;
				}
				else if (heroOrPet_unitID >= ID_Magic_Start && heroOrPet_unitID <= ID_Magic_End){
					//SdkManager::getInstance()->pay(ProductID_MagicLevelUp);
					product_id = ProductID_MagicLevelUp;
				}
				else {
					CCASSERT(false, "error!");
				}

				//test win32
#if DEBUG_ALWAYS_BUY_SUCCESS == 1
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_PayResult);
				MsgData data;
				data.value = product_id;
				data.errorCode = Rsp_Success;
				event.setUserData(&data);
				eventDspc->dispatchEvent(&event);
#endif

				return;
			}

			const BagItem *bag = BagDataManager::getManager()->getOneUnit(info->needUnit->ID);			
			//1. 背包内有
			if (bag)
			{
				if (bag->value >= info->needCount)
				{
					int price = info->needCount;
					BagDataManager::getManager()->changeOneUnitCount(info->needUnit->ID,-price);

//					BagDataManager::getManager()->changeOneUnitLevel(heroOrPet_unitID,1);

					BagDataManager::getManager()->changeOneUnitLevel(heroOrPet_unitID,1,0);


					EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					EventCustom event(MSG_LevelUp_Rsp);
					MsgData data;
					data.errorCode = Rsp_Success;
					event.setUserData(&data);
					eventDspc->dispatchEvent(&event);
				}
				else
				{
					EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					EventCustom event(MSG_LevelUp_Rsp);
					MsgData data;
					if (info->needUnit->ID == unitID_JinBi)
					{
						data.errorCode = Rsp_Gold_UnEnough;
					}
					else if (info->needUnit->ID == unitID_Diamond)
					{
						data.errorCode = Rsp_Jewel_UnEnough;
					}
					else
					{
						data.errorCode = Rsp_Need_UnEnough;
					}
					event.setUserData(&data);
					eventDspc->dispatchEvent(&event);
				}
			}
			//2. 背包内没有
			else
			{
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_LevelUp_Rsp);
				MsgData data;
				
				if (info->needUnit->ID == unitID_JinBi)
				{
					data.errorCode = Rsp_Gold_UnEnough;
				}
				else if (info->needUnit->ID == unitID_Diamond)
				{
					data.errorCode = Rsp_Jewel_UnEnough;
				}
				else if(info->needUnit->ID ==unitID_RMB){
                    data.errorCode=Rsp_NotSupport;
				}else{
				
					data.errorCode = Rsp_Need_UnEnough;
				}
				event.setUserData(&data);
				eventDspc->dispatchEvent(&event);
			}
			
		}
	}
	else
	{
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_LevelUp_Rsp);
		MsgData data;
		data.errorCode = Rsp_DontHaveThing;
		event.setUserData(&data);
		eventDspc->dispatchEvent(&event);
	}
	//LevelDataConfigManager::getManager()->getOneUnitLevelInfo(hero_unitID,)
	//BagDataManager::getManager()->changeOneUnitLevel(hero_unitID,1);
}
bool ActorHelper::isMaxLevel(unsigned int heroOrPet_unitID)
{
	const BagItem *item = BagDataManager::getManager()->getOneUnit(heroOrPet_unitID);

	if (item)
	{
		int max_level = LevelDataConfigManager::getManager()->getMaxLevel(heroOrPet_unitID);
		if (max_level > item->value)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
Actor ActorHelper::getOneHero(unsigned int hero_unitID)
{
	Actor actor;
	vector<Actor> list = getAllHero();
	for (vector<Actor>::iterator iter = list.begin() ; iter != list.end() ; iter ++)
	{
		
		if (iter->unit.ID == hero_unitID)
		{
			actor = (*iter);
			break;
		}
	}
	return actor;
}
Actor ActorHelper::getOnePet(unsigned int pet_unitID)
{
	Actor pet;
	pet.level = 0;
	vector<Actor> list = getAllPet();
	for (vector<Actor>::iterator iter = list.begin() ; iter != list.end() ; iter ++)
	{

		if (iter->unit.ID == pet_unitID)
		{
			pet = (*iter);
			break;
		}
	}
	return pet;
}
Actor ActorHelper::getOneMagic(unsigned int magic_unitID)
{
	Actor magic;
	vector<Actor> list = getAllMagic();
	magic.level = 0;
	for (vector<Actor>::iterator iter = list.begin() ; iter != list.end() ; iter ++)
	{

		if (iter->unit.ID == magic_unitID)
		{
			magic = (*iter);
			break;
		}
	}
	return magic;
}

Actor ActorHelper::getOneHeroWithLevelInfo(unsigned int hero_unitID, unsigned int hero_level)
{
	Actor actor = ActorHelper::getHelper()->getOneHero(hero_unitID);
	if (hero_level == 0)
		actor.unit.attribute = ActorHelper::getHelper()->getOneActorLevelInfo(actor.unit.ID, actor.level)->attrVec;
	else 
		actor.unit.attribute = ActorHelper::getHelper()->getOneActorLevelInfo(actor.unit.ID, hero_level)->attrVec;
	return actor;
}
Actor ActorHelper::getOnePetWithLevelInfo(unsigned int pet_unitID, unsigned int pet_level)
{
	Actor actor = ActorHelper::getHelper()->getOnePet(pet_unitID);
	if (pet_level == 0)
		actor.unit.attribute = ActorHelper::getHelper()->getOneActorLevelInfo(actor.unit.ID, actor.level)->attrVec;
	else 
		actor.unit.attribute = ActorHelper::getHelper()->getOneActorLevelInfo(actor.unit.ID, pet_level)->attrVec;
	return actor;
}
Actor ActorHelper::getOneMagicWithLevelInfo(unsigned int magic_unitID, unsigned int magic_level)
{
	Actor actor = ActorHelper::getHelper()->getOneMagic(magic_unitID);

	const OneLevelInfo *info = nullptr;
	if (magic_level == 0)
		info = ActorHelper::getHelper()->getOneActorLevelInfo(actor.unit.ID, actor.level);
	else 
		info = ActorHelper::getHelper()->getOneActorLevelInfo(actor.unit.ID, magic_level);
	actor.unit.attribute = info->attrVec;
	actor.unit.imageFile = info->imgFile;
	actor.unit.name = info->name;

	return actor;
}

bool ActorHelper::getIsUnlockItem(unsigned int id)
{
	const BagItem* item = BagDataManager::getManager()->getOneUnit(id);
	if (item)
	{
		return true;
	}
	return false;
}
int ActorHelper::getMaxLevel(unsigned int id)
{
	return LevelDataConfigManager::getManager()->getMaxLevel(id);
}
const OneLevelInfo *ActorHelper::getOneActorLevelInfo(unsigned int id,unsigned int level)
{
	return LevelDataConfigManager::getManager()->getOneUnitLevelInfo(id,level);
}
unsigned int ActorHelper::levelUpNeed(unsigned int heroOrPet_unitID)
{
	unsigned int need = 0;
	const BagItem *item = BagDataManager::getManager()->getOneUnit(heroOrPet_unitID);

	if (item)
	{
		int max_level = LevelDataConfigManager::getManager()->getMaxLevel(heroOrPet_unitID);
		if (max_level > item->value)
		{
			const OneLevelInfo *info = LevelDataConfigManager::getManager()->getOneUnitLevelInfo(heroOrPet_unitID,item->value + 1);

			const BagItem *bag = BagDataManager::getManager()->getOneUnit(info->needUnit->ID);

			if (bag)
			{
				need = info->needCount - bag->value_2;
			}
		}
	}
	return need;
}
void ActorHelper::feeding(unsigned int heroOrPet_unitID,unsigned int count /* = 1 */)
{
	const BagItem *item = BagDataManager::getManager()->getOneUnit(heroOrPet_unitID);

	if (item)
	{
		int max_level = LevelDataConfigManager::getManager()->getMaxLevel(heroOrPet_unitID);
		if (max_level > item->value)
		{
			const OneLevelInfo *info = LevelDataConfigManager::getManager()->getOneUnitLevelInfo(heroOrPet_unitID,item->value + 1);

			const BagItem *bag = BagDataManager::getManager()->getOneUnit(info->needUnit->ID);

			if (bag)
			{
				if ((info->needCount - item->value_2) > count)
				{
					if (bag->value >= count)
					{
						int price = count;
						BagDataManager::getManager()->changeOneUnitCount(info->needUnit->ID,-price);
						BagDataManager::getManager()->changeOneUnitLevel(heroOrPet_unitID,0,count);
						EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
						EventCustom event(MSG_Feed_Rsp);
						MsgData data;
						data.errorCode = Rsp_Success;
						event.setUserData(&data);
						eventDspc->dispatchEvent(&event);
					}
					else
					{
						EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
						EventCustom event(MSG_Feed_Rsp);
						MsgData data;
						data.errorCode = Rsp_DontHaveThing;
						event.setUserData(&data);
						eventDspc->dispatchEvent(&event);
					}
				}
				else
				{
					if (bag->value >= count)
					{
						int price = count;
						BagDataManager::getManager()->changeOneUnitCount(info->needUnit->ID,-price);
						BagDataManager::getManager()->changeOneUnitLevel(heroOrPet_unitID,1,bag->value_2 + count - info->needCount);

						EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
						EventCustom event(MSG_Feed_Rsp);
						MsgData data;
						data.errorCode = Rsp_Success;
						event.setUserData(&data);
						eventDspc->dispatchEvent(&event);
					}
				}
			}
			else
			{
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_Feed_Rsp);
				MsgData data;
				data.errorCode = Rsp_DontHaveThing;
				event.setUserData(&data);
				eventDspc->dispatchEvent(&event);
			}

		}
	}
	else
	{
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_Feed_Rsp);
		MsgData data;
		data.errorCode = Rsp_DontHaveThing;
		event.setUserData(&data);
		eventDspc->dispatchEvent(&event);
	}
}
vector<OneLevelInfo> ActorHelper::getOneActorAllLevelInfo(unsigned int id,unsigned int level)
{
	vector<OneLevelInfo> listInfo;
	int max = getMaxLevel(id);
	for (int i = level + 1; i < max ; i ++)
	{
		listInfo.push_back(*getOneActorLevelInfo(id,i));
	}
	return listInfo;
}

UnitType ActorHelper::getTypeFromUnit(unsigned int id)
{
	if (UnitTypeSelect(id,ID_Special_Start,ID_Special_End))
	{
		return unittype_Special;
	}
	if (UnitTypeSelect(id,ID_Actor_Start,ID_Actor_End))
	{
		return unittype_Actor;
	}
	if (UnitTypeSelect(id,ID_Pet_Start,ID_Pet_End))
	{
		return unittype_Pet;
	}
	if (UnitTypeSelect(id,ID_Magic_Start,ID_Magic_End))
	{
		return unittype_Magic;
	}
	if (UnitTypeSelect(id,ID_Land_Start,ID_Land_End))
	{
		return unittype_Land;
	}
	if (UnitTypeSelect(id,ID_Item_Start,ID_Item_End))
	{
		return unittype_Item;
	}
	if (UnitTypeSelect(id,ID_Obstacle_Start,ID_Obstacle_End))
	{
		return unittype_Obstacle;
	}

	if (UnitTypeSelect(id,ID_Decorative_Start,ID_Decorative_End))
	{
		return unittype_Decorative;
	}

	if (UnitTypeSelect(id,ID_Tool_Start,ID_Tool_End))
	{
		return unittype_Tool;
	}

	/*
	if (UnitTypeSelect(id,ID_Achievement_Start,ID_Achievement_End))
	{
		return unittype_Achievement;
	}
	*/
	return unittype_None;
}

std::pair<int,int> ActorHelper::getUnlockHeroNeedFragmentAndGold(unsigned int unitID)
{
	if (unitID == unitID_Actor2){
		return std::make_pair(40, 20000);
	}else {
		CCASSERT(false, "");
		return std::make_pair(0xffff, 0xffff);
	}
}
UnitID ActorHelper::getActorUnlockNeedFragmentId(unsigned int unitID)
{
	if (unitID == unitID_Actor2){
		return unitID_Actor2Fragment;
	}
	else {
		//CCASSERT(false, "");
		return unitID_None;
	}
}
void ActorHelper::unlockHeroByFragment(unsigned int unitID)//解锁角色
{
	UnitID needFragId = this->getActorUnlockNeedFragmentId(unitID);

	const BagItem *fragItem = BagDataManager::getManager()->getOneUnit(needFragId);
	const BagItem *goldItem = BagDataManager::getManager()->getOneUnit(unitID_Gold);
	int needFrag = this->getUnlockHeroNeedFragmentAndGold(unitID).first;
	int needGold = this->getUnlockHeroNeedFragmentAndGold(unitID).second;
	if (fragItem==nullptr || fragItem->value < needFrag)
	{
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_MarketBuyGoods_Rsp);
		MsgData data;
		//data.errorCode = Rsp_Need_UnEnough;
		if(needFragId == unitID_Actor2Fragment ){
			data.errorCode = Rsp_Suipian_UnEnough;
		}else{
			data.errorCode = Rsp_Need_UnEnough;
		}
		event.setUserData(&data);
		eventDspc->dispatchEvent(&event);
		return;
	}
	else if (goldItem==nullptr || goldItem->value < needGold)
	{
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_MarketBuyGoods_Rsp);
		MsgData data;

		data.errorCode = Rsp_Gold_UnEnough;
		event.setUserData(&data);
		eventDspc->dispatchEvent(&event);
		return;
	}
		
	BagDataManager::getManager()->changeOneUnitCount(fragItem->id, -needFrag);
	BagDataManager::getManager()->changeOneUnitCount(goldItem->id, -needGold);
	BagDataManager::getManager()->changeOneUnitCount(unitID, 1);
	EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	EventCustom event(MSG_MarketBuyGoods_Rsp);
	MsgData data;
	data.errorCode = Rsp_Success;
	event.setUserData(&data);
	eventDspc->dispatchEvent(&event);
}
