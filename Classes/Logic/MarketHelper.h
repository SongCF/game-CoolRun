//============================================================
//* 商店帮助类
//* 逻辑层
//* 所有商店的操作、UI层调用，数据访问都是通过该类实现
//============================================================

#pragma once
#include <vector>
#include "MarketConfigManager.h"
//全是静态函数
class MarketHelper
{
private:
	static MarketHelper* s_mMarketHelper;
	MarketHelper();
public:
	static MarketHelper* getHelper();

	//获取商城所卖所有marketCharge
	const vector<SaleItem>* getAllChargeItems();

	//获取商城所卖所有道具列表
	const vector<SaleItem>* getAllSaleItems();

	//用户操作
	void buyItem(unsigned int saleId); //not unit id

	const SaleItem *getSaleItemByID(unsigned int id);
	//void buyHero(unsigned int unitID);
	//void buyPet(unsigned int unitID);
};