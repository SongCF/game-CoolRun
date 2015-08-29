/*
//商品信息
*/


#pragma once

#include "cocos2d.h"

#include <string>
#include <vector>
#include <map>
using namespace cocos2d;
using std::string;
using std::vector;
using std::map;


//游戏内产品id   (productID.txt中对应每个渠道的productID)
enum ProductID
{
	ProductID_None 				= 0,
	ProductID_20Diamond 		= 1,
	ProductID_70Diamond 		= 2,
	ProductID_140Diamond 		= 3,
	ProductID_5000Gold 			= 4,
	ProductID_RoleLevelUp 		= 5,
	ProductID_RoleFullLevel		= 6,
	ProductID_MagicLevelUp		= 7,
	ProductID_MagicFullLevel	= 8,
};


struct ProductInfo
{
	int ID;
	string describe;
	int price;  //部分sdk不支持浮点，所以必须全部为整型
	map<string,string> channelProduct;  // channelID : productID
};

class ProductInfoHelper
{
private:
	static ProductInfoHelper* _productInfoHelper;
	ProductInfoHelper();
	bool initData();
public:
	static ProductInfoHelper* getHelper();
	const ProductInfo* getOneProductInfo(ProductID productID);
	
private:
	vector<ProductInfo> m_productList;
};
