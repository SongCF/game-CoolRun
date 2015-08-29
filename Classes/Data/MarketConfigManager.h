//============================================================
//* 商店管理类
//* 所有商店的操作逻辑
//============================================================

#pragma once

#include "UnitDefine.h"
#include "XReadJson.h"
struct SaleItem
{
	unsigned int saleID;
	unsigned int unitSaleID;
	unsigned int saleCount;
	unsigned int unitPayID;
	unsigned int payPrice;
	int value; //扩展字段 （1.若是付费，那么对应productID）


	SaleItem(){
		this->saleID = 0;
		this->unitSaleID = 0;
		this->saleCount = 0;
		this->unitPayID = 0;
		this->payPrice = 0;
		this->value = 0;
	}
	SaleItem(const SaleItem& _in){
		this->saleID = _in.saleID;
		this->unitSaleID = _in.unitSaleID;
		this->saleCount = _in.saleCount;
		this->unitPayID = _in.unitPayID;
		this->payPrice = _in.payPrice;
		this->value = _in.value;
	}
	const SaleItem& operator=(const SaleItem& _in){
		this->saleID = _in.saleID;
		this->unitSaleID = _in.unitSaleID;
		this->saleCount = _in.saleCount;
		this->unitPayID = _in.unitPayID;
		this->payPrice = _in.payPrice;
		this->value = _in.value;
		return *this;
	}
};

class MarketConfigManager
{
private:
	static MarketConfigManager* s_mMarketConfigManager;
	MarketConfigManager();
	void initData();
	bool isPet(unsigned int id);

	void initGoods(const rapidjson::Value &arrayValue,vector<SaleItem> &list);
public:
	static MarketConfigManager* getManager();

	const SaleItem* getOneGoods(unsigned int id);
	const vector<SaleItem>* getAllGoods();
	const vector<SaleItem>* getAllChargeGoods();
	const vector<SaleItem>* getAllDelPetGoods();
protected:
	vector<SaleItem> m_lGoodsVec;
	vector<SaleItem> m_lDelPetGoodsVec;

	vector<SaleItem> m_lChargeGoodsVec;
};