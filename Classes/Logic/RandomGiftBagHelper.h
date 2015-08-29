//============================================================
//* 随机礼包帮助类
//* 逻辑层
//* 随机礼包的所有操作：重置随机表、抽取随机礼包
//============================================================

#pragma once

#include "cocos2d.h"
#include "ID_Define.h"
#include "BagDataManager.h"
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::pair;
USING_NS_CC;

class RandomGiftBagHelper
{
private:
	static RandomGiftBagHelper* s_randomGiftbagHelper;
	RandomGiftBagHelper();
	bool initData(Data dat);
	bool getFileData();
	bool saveData();
public:
	static RandomGiftBagHelper* getHelper();

#pragma region 活跃度的随机礼包
public:
	//pair<BagItem, unsigned int>   --->   BagItem  权重
	void setGiftBagTable(const vector<pair<BagItem, unsigned int>>& table);
	const vector<pair<BagItem, unsigned int>>& getGiftBagTable();
	string getVersion(){return m_giftbag_strTableVersion;}
	//抽取一个随机礼包 
	bool getOneRandomGiftBag(BagItem& item);
private:
	vector<std::pair<BagItem, unsigned int>>	m_giftbag_vecGiftTable;
	string										m_giftbag_strTableVersion;
	
	int											m_giftbag_iTotalWeight;
#pragma endregion


#pragma region 商城里面的随机宝箱
public:
	bool getOneRandomMarketItem(BagItem& item);

private:
	vector<std::pair<BagItem, unsigned int>>	m_market_vecGiftTable;
	int											m_market_iTotalWeight;
#pragma endregion

};