
/*****************************************************************************


    客户端,服务端公用的结构和方法


*****************************************************************************/



#pragma once
#include <string>
#include <vector>
#include <string.h>



//道具类型 PS: 邮件、礼包、奖励也使用这个类型
enum AttachType {
	AttachType_Invalid 		= 0,	//无效类型
	AttachType_Gold 		= 1,	//金币
	AttachType_Diamond 		= 2,	//宝石
	AttachType_Props 		= 3,	//道具 
	AttachType_Debris 	    = 4,	//碎片
	AttachType_GiftBag		= 5,	//礼包
	AttachType_Vitality		= 6,	//体力
};


//排行榜奖励ID类型
enum RankBonusID {		
	RankBonusID_Invalid     = 0,    //无效id
	RankBonusID_1           = 1,    //第1名
	RankBonusID_2           = 2,    //第2名
	RankBonusID_3           = 3,    //第3名
	RankBonusID_4_5         = 4,    //4-5名
	RankBonusID_6_10        = 5,    //6-10名
	RankBonusID_11_20       = 6,    //11-20名
	RankBonusID_21_30       = 7,    //21-30名
};


//排行榜分组ID类型
enum RankGroupID {		
	RankGroupID_0           = 0,    //default 分组
	RankGroupID_1           = 1,    //第2组
	RankGroupID_2           = 2,    //
	RankGroupID_3           = 3,    //
	RankGroupID_4           = 4,    //
	RankGroupID_5           = 5,    //
	RankGroupID_6           = 6,    //
	RankGroupID_7           = 7,    //
	RankGroupID_8           = 8,    //
	//RankGroupID_9           = 9,    //
    RankGroup_Max,                  //最大分组
};


namespace CS
{
	/* 排行榜 */
	//排名获取奖励id
    RankBonusID get_rank_bonus_id(int order);
}

