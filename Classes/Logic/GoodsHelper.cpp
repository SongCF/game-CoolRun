#include "GoodsHelper.h"
#include "UnitConfigManager.h"
#include "PlayerInfoDataManager.h"
#include "ActorHelper.h"

GoodsHelper* GoodsHelper::s_mGoodsHelper = nullptr;

GoodsHelper::GoodsHelper()
{

}

GoodsHelper* GoodsHelper::getHelper()
{
	if (! s_mGoodsHelper) s_mGoodsHelper = new GoodsHelper;
	return s_mGoodsHelper;
}

const vector<BagItem> GoodsHelper::getAllMagicItems()
{
	const vector<BagItem> *bagList = BagDataManager::getManager()->getAllUnits();
	vector<BagItem> list;
	for (vector<BagItem>::const_iterator iter = bagList->begin(); iter != bagList->end(); iter++)
	{
		if (iter->id >= ID_Magic_Start && iter->id <= ID_Magic_End)
		{
			BagItem item;
			item.id = iter->id;
			item.value = iter->value;
			item.value_2 = iter->value_2;
			list.push_back(item);
		}
	}
	return list;
}
const vector<BagItem> GoodsHelper::getAllAchievementItems()
{
	const vector<BagItem> *bagList = BagDataManager::getManager()->getAllUnits();
	vector<BagItem> list;
	for (vector<BagItem>::const_iterator iter = bagList->begin(); iter != bagList->end(); iter++)
	{
		if (iter->id >= ID_Achievement_Start && iter->id <= ID_Achievement_End)
		{
			BagItem item;
			item.id = iter->id;
			item.value = iter->value;
			item.value_2 = iter->value_2;
			list.push_back(item);
		}
	}
	return list;
}
const BagItem* GoodsHelper::getOneItem(unsigned int id)
{
	const vector<BagItem> *bagList = BagDataManager::getManager()->getAllUnits();
	for (vector<BagItem>::const_iterator iter = bagList->begin(); iter != bagList->end(); iter++)
	{
		if (iter->id == id)
		{
			return &*iter;
		}
	}
	return nullptr;
}

unsigned int GoodsHelper::getGoldNum()
{
	const BagItem *item = getOneItem(unitID_Gold);
	if (!item)
	{
		return 0;
	}
	return item->value;
}
unsigned int GoodsHelper::getDiamondNum()
{
	const BagItem *item = getOneItem(unitID_Diamond);
	if (!item)
	{
		return 0;
	}
	return item->value;
}
unsigned int GoodsHelper::getJewelNum()
{
	const BagItem *item = getOneItem(unitID_RedJewel);
	if (!item)
	{
		return 0;
	}
	return item->value;
}

unsigned int GoodsHelper::getActorFragmentNum(UnitID actorId)
{
	UnitID fragmentId = (UnitID)ActorHelper::getHelper()->getActorUnlockNeedFragmentId(actorId);

	const BagItem *item = getOneItem(fragmentId);
	if (!item)
	{
		return 0;
	}
	return item->value;
}


unsigned int GoodsHelper::getDaogaiNum(){
	const BagItem *item = getOneItem(unitID_DanGao);
	if (!item)
	{
		return 0;
	}
	return item->value;
}
unsigned int GoodsHelper::getGuodongNum(){
	const BagItem *item = getOneItem(unitID_GuoDong);//甜筒
	if (!item)
	{
		return 0;
	}
	return item->value;
}
unsigned int GoodsHelper::getTangguoNum(){
	const BagItem *item = getOneItem(unitID_TangGuo);
	if (!item)
	{
		return 0;
	}
	return item->value;
}

const Unit *GoodsHelper::getUnitByID(unsigned int id)
{
	return UnitConfigManager::getManager()->getOneUnit(id);
}
const vector<BagItem> GoodsHelper::getAllGoods()
{
	const vector<BagItem> *bagList = BagDataManager::getManager()->getAllUnits();
	vector<BagItem> list;
	for (vector<BagItem>::const_iterator iter = bagList->begin(); iter != bagList->end(); iter++)
	{
		BagItem item;
		item.id = iter->id;
		item.value = iter->value;
		list.push_back(item);
	}
	return list;
}
bool GoodsHelper::useOneGoods(unsigned int id,int count)
{
	const BagItem* item = BagDataManager::getManager()->getOneUnit(id);
	if (item)
	{
		if (item->value >= 1)
		{
			BagDataManager::getManager()->changeOneUnitCount(id,-count);
			return true;
		}
	}
	return false;
}
void GoodsHelper::changeOneUnitCount(unsigned int id, int count)
{
	BagDataManager::getManager()->changeOneUnitCount(id,count);
}

void GoodsHelper::setRandomMarketItem(unsigned int unitId)
{
	PlayerInfoDataManager::getManager()->setOnePlayerInfo("RandomMarketItem", unitId);
}

unsigned int GoodsHelper::getRandomMarketItem()
{
	return PlayerInfoDataManager::getManager()->getOnePlayerInfo("RandomMarketItem");
}
void GoodsHelper::useRandomMarketItem()
{
	PlayerInfoDataManager::getManager()->setOnePlayerInfo("RandomMarketItem", unitID_None);
}