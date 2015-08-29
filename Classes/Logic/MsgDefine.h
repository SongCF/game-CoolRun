//============================================================
//* 各层、模块之间的消息定义
//============================================================

#pragma once

#include <string>
using std::string;

#define UI_LayerChanged "layerChanged"
//数据层 数据变动 通知上层
#define MSG_BagDataChanged "bagitem_changed"

//网络消息发送通知
#define MSG_SendResult "Msg_SendResult"


//上层请求操作后，下层操作完成返回对应消息
#define MSG_MarketBuyGoods_Rsp "rsp_market_buy_goods"
#define MSG_LevelUp_Rsp "rsp_level_up_goods"
#define MSG_Feed_Rsp "rsp_Feed_pet_goods"

//用户操作
#define CustomButton_Jump  "hero_jumpButton_pressed"
#define CustomButton_Down  "hero_downButton_pressed"
#define CustomButton_Fight "hero_fightButton_pressed"
#define CustomButton_Super "hero_Super_pressed"

//UI间消息通知，减少耦合
#define MSG_HeroDead "hero_die"
#define MSG_HeroWin  "hero_win"
#define MSG_MarketClose "market_close"


//支付回调消息
#define MSG_PayResult "pay_result"

//消息返回 error code
enum Rsp_ErrorCode
{
	Rsp_Success = 0,//成功
	Rsp_Error, //未知错误
	Rsp_Cancle,//取消操作
	Rsp_NetWorkError,//网络连接错误
	Rsp_TimeOut,//超时
	Rsp_NotSupport,//目前不支持该功能

	Rsp_Gold_UnEnough,//金币不足
	Rsp_Jewel_UnEnough,//钻石不足
	Rsp_RedJewel_UnEnough,//红宝石不足
	Rsp_Suipian_UnEnough,//碎片不足
	Rsp_Need_UnEnough,//所需物品不足
	Rsp_DontHaveThing,//没有该物品

};
struct MsgData
{
	unsigned int errorCode;//Rsp_ErrorCode
	unsigned int value;//自定义
	void *pData;//临时任意数据

	MsgData(){
		errorCode = Rsp_Error;
		value = 0;
		pData = nullptr;
	}
};