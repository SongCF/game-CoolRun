#include "MarketHelper.h"
#include "MarketConfigManager.h"
#include "GoodsHelper.h"
#include "cocos2d.h"
#include "MsgDefine.h"
#include "StringsMap.h"
#include "AchievementHelper.h"
#include "RandomGiftBagHelper.h"
#include "NetmsgManger.h"
#include "sdk/SdkManager.h"

using namespace FX;
using namespace cocos2d;
MarketHelper* MarketHelper::s_mMarketHelper = nullptr;


static int s_buy_SaleItemId = 0;
MarketHelper::MarketHelper()
{
	EventListenerCustom* listener = EventListenerCustom::create(MSG_PayResult, [&](EventCustom* evt) 
	{  
		MsgData* payData=(MsgData*)evt->getUserData();
		int gameProductID = payData->value;
		if (gameProductID == ProductID_20Diamond 
			|| gameProductID == ProductID_70Diamond
			|| gameProductID == ProductID_140Diamond
			|| gameProductID == ProductID_5000Gold)
		{
			if (payData->errorCode == Rsp_Success)
			{
				const SaleItem* saleItem = MarketConfigManager::getManager()->getOneGoods(s_buy_SaleItemId);
				if (saleItem){
					BagDataManager::getManager()->changeOneUnitCount(saleItem->unitSaleID,saleItem->saleCount);
				}
			}

			//
			int errCode = payData->errorCode;
			Director::getInstance()->getRunningScene()->runAction(Sequence::create(
				DelayTime::create(1),
				CallFunc::create([=](){
					EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					EventCustom event(MSG_MarketBuyGoods_Rsp);
					MsgData data;
					data.errorCode = errCode;
					event.setUserData(&data);
					eventDspc->dispatchEvent(&event);
			}),
				nullptr));
		}

	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,-1);  //优先级应该比UI层高
}

MarketHelper* MarketHelper::getHelper()
{
	if (! s_mMarketHelper) s_mMarketHelper = new MarketHelper;
	return s_mMarketHelper;
}


//获取商城所卖所有道具列表
const vector<SaleItem>* MarketHelper::getAllSaleItems(){

	return MarketConfigManager::getManager()->getAllGoods();

}
const vector<SaleItem>* MarketHelper::getAllChargeItems(){

	return MarketConfigManager::getManager()->getAllChargeGoods();

}
void MarketHelper::buyItem(unsigned int scaleID)
{
	const SaleItem* saleItem = MarketConfigManager::getManager()->getOneGoods(scaleID);
	if (saleItem)
	{
		//0. 付费---直接进入sdk
		if (saleItem->unitPayID == unitID_RMB)
		{
			s_buy_SaleItemId = scaleID;
			//SdkManager::getInstance()->pay((ProductID)saleItem->value);

			//test win32
#if DEBUG_ALWAYS_BUY_SUCCESS == 1
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_PayResult);
			MsgData data;
			data.value = saleItem->value;
			data.errorCode = Rsp_Success;
			event.setUserData(&data);
			eventDspc->dispatchEvent(&event);
#endif
			return;
		}

		const BagItem *item = GoodsHelper::getHelper()->getOneItem(saleItem->unitPayID);
		//1.背包内有 买saleId所需的payId
		if (item)
		{
			if (item->value >= saleItem->payPrice)
			{
				unsigned int rid=0;
				unsigned int rvalue=0;
				int price = saleItem->payPrice;
				BagDataManager::getManager()->changeOneUnitCount(item->id,-price);
				//如果是购买随机礼包
				if (saleItem->unitSaleID == unitID_RandomItem)
				{
					BagItem item;
					if (RandomGiftBagHelper::getHelper()->getOneRandomMarketItem(item)){
						scaleID = item.id;
						rid=item.id;
						rvalue=item.value;
						//本次抽取的会把上一次的覆盖掉
						GoodsHelper::getHelper()->setRandomMarketItem(item.id);
//						BagDataManager::getManager()->changeOneUnitCount(item.id,item.value);

					}else {
						CCASSERT(false, "生成随机包失败！");
					}
				}
				else 
				{
					BagDataManager::getManager()->changeOneUnitCount(saleItem->unitSaleID,saleItem->saleCount);
				}
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_MarketBuyGoods_Rsp);
				MsgData data;
				data.errorCode = Rsp_Success;
				data.value =rvalue;
				event.setUserData(&data);
				eventDspc->dispatchEvent(&event);
			}
			else
			{
				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom event(MSG_MarketBuyGoods_Rsp);
				MsgData data;
				if (item->id == unitID_Gold)
				{
					data.errorCode = Rsp_Gold_UnEnough;
				}
				else if (item->id == unitID_Diamond)
				{
					data.errorCode = Rsp_Jewel_UnEnough;
				}else if (item->id == unitID_RedJewel)
				{
					data.errorCode = Rsp_RedJewel_UnEnough;
				}
				data.value = saleItem->saleID;
				event.setUserData(&data);
				eventDspc->dispatchEvent(&event);
			}
		}
		//2. 没有payId
		else
		{
			EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
			EventCustom event(MSG_MarketBuyGoods_Rsp);
			MsgData data;
			data.errorCode=Rsp_Need_UnEnough;
			if (saleItem->unitPayID == unitID_Gold)
			{
				data.errorCode = Rsp_Gold_UnEnough;
			}
			else if (saleItem->unitPayID == unitID_Diamond)
			{
				data.errorCode = Rsp_Jewel_UnEnough;
			}
			data.value = saleItem->saleID;
			event.setUserData(&data);
			eventDspc->dispatchEvent(&event);
		}
	}
	else
	{
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom event(MSG_MarketBuyGoods_Rsp);
		MsgData data;
		data.errorCode = Rsp_DontHaveThing;
		data.value = saleItem->saleID;
		event.setUserData(&data);
		eventDspc->dispatchEvent(&event);
	}
	
	//GoodsHelper::getHelper()->getGoldNum();
}
const SaleItem *MarketHelper::getSaleItemByID(unsigned int id)
{
	return MarketConfigManager::getManager()->getOneGoods(id);
}