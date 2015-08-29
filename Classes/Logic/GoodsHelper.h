//============================================================
//* 背包帮助类
//* 逻辑层
//* 提供所有背包的逻辑操作接口，供UI层使用，UI层仅能通过该帮助类来获取背包数据层数据。
//============================================================

#pragma once

#include "UnitDefine.h"
#include <vector>
#include "BagDataManager.h"
class GoodsHelper
{
private:
	static GoodsHelper* s_mGoodsHelper;
	GoodsHelper();
public:
	static GoodsHelper* getHelper();

	const vector<BagItem> getAllMagicItems();
	const vector<BagItem> getAllAchievementItems();
	const BagItem* getOneItem(unsigned int id);
	const vector<BagItem> getAllGoods();
	unsigned int getGoldNum();
	unsigned int getDiamondNum();
	unsigned int getJewelNum();
	unsigned int getActorFragmentNum(UnitID actorId);

	//
	unsigned int getDaogaiNum();
	unsigned int getGuodongNum();
	unsigned int getTangguoNum();
	//
	bool useOneGoods(unsigned int id,int count = 1);

	const Unit *getUnitByID(unsigned int id);

	void changeOneUnitCount(unsigned int id, int count);

	//random giftbag helper抽取到的随机道具，下一个会覆盖上一个，入场一定使用
	void setRandomMarketItem(unsigned int unitId);
	unsigned int getRandomMarketItem();
	void useRandomMarketItem();
};