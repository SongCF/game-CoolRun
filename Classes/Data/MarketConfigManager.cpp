#include "MarketConfigManager.h"
#include "VersionConfig.h"
#include "cocos2d.h"
#include "UnitConfigManager.h"
static string DatatName = "config/Market.txt";
MarketConfigManager* MarketConfigManager::s_mMarketConfigManager = nullptr;
	
static int goodsCompareByID(const void* a, const void* b)
{
	return ((SaleItem*)a)->unitSaleID - ((SaleItem*)b)->unitSaleID;
}

MarketConfigManager::MarketConfigManager()
{
}

MarketConfigManager* MarketConfigManager::getManager()
{
	if (! s_mMarketConfigManager) {
		s_mMarketConfigManager = new MarketConfigManager;
		s_mMarketConfigManager->initData();
	}
	return s_mMarketConfigManager;
}

void MarketConfigManager::initData()
{
	//parse file
	Document root;
	UnitConfigManager *unitContext = UnitConfigManager::getManager();
	XReadJson *json = XReadJson::getIntanse();
	json->getJsonRootValue(DatatName,root);
	CCLOG("openFile %s :\n",DatatName.c_str());
	{
		const Value &arrayValue = json->getSubValue_json(root,"marketGoods");
		initGoods(arrayValue,m_lGoodsVec);
	}
	{
		const Value &arrayValue = json->getSubValue_json(root,"marketCharge");
		initGoods(arrayValue,m_lChargeGoodsVec);
	}
	
}
void MarketConfigManager::initGoods(const rapidjson::Value &arrayValue,vector<SaleItem> &list)
{
	XReadJson *json = XReadJson::getIntanse();
	int size = arrayValue.Size();
	for (int i = 0 ; i < size ; i++)
	{
		SaleItem item;
		const Value &value = json->getSubValue_json(arrayValue,i);
		if (json->isHave(value,"ID"))
		{
			item.saleID = json->getInt_json(value,"ID",0);
		}
		if (json->isHave(value,"unitID_sale"))
		{
			item.unitSaleID = json->getInt_json(value,"unitID_sale",0);
		}
		if (json->isHave(value,"unitID_pay"))
		{
			item.unitPayID = json->getInt_json(value,"unitID_pay",0);
		}
		if (json->isHave(value,"payPrice"))
		{
			item.payPrice = json->getInt_json(value,"payPrice",0);
		}
		if (json->isHave(value,"saleCount"))
		{
			item.saleCount = json->getInt_json(value,"saleCount",0);
		}
		if (json->isHave(value,"productID"))
		{
			item.value = json->getInt_json(value,"productID",0);
		}
		list.push_back(item);
		if (!isPet(item.unitSaleID))
		{
			m_lDelPetGoodsVec.push_back(item);
		}
	}
}

const SaleItem* MarketConfigManager::getOneGoods(unsigned int id)
{
	for (vector<SaleItem>::iterator iter = m_lGoodsVec.begin() ; iter != m_lGoodsVec.end() ; iter ++)
	{
		if (iter->saleID == id)
		{
			return (const SaleItem*)(&*iter);
		}
	}

	for (vector<SaleItem>::iterator iter = m_lChargeGoodsVec.begin() ; iter != m_lChargeGoodsVec.end() ; iter ++)
	{
		if (iter->saleID == id)
		{
			return (const SaleItem*)(&*iter);
		}
	}

	{
		if (1 == DEBUG_uMessageBox)
		{
			cocos2d::MessageBox(cocos2d::StringUtils::format("not found Goods by Id=[%d]", id).c_str(), "ERROR");
			CCASSERT(false, "");
		}
		return nullptr;
	}
}

const vector<SaleItem>* MarketConfigManager::getAllGoods()
{
	return &m_lGoodsVec;
}

const vector<SaleItem>* MarketConfigManager::getAllChargeGoods()
{
	return &m_lChargeGoodsVec;
}

const vector<SaleItem>* MarketConfigManager::getAllDelPetGoods()
{
	return &m_lDelPetGoodsVec;
}
bool MarketConfigManager::isPet(unsigned int id)
{
	vector<unsigned int> petList;
	petList.push_back(unitID_PiPiZhu);
	petList.push_back(unitID_XiXiaoYang);
	petList.push_back(unitID_BuGuNiao);
	for (vector<unsigned int>::iterator iter = petList.begin() ; iter != petList.end(); iter ++)
	{
		if (*iter == id)
		{
			return true;
		}
	}
	return false;
}