#pragma once


#include "cocos2d.h"
#include "ID_Define.h"
#include <string>
#include <vector>

#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"


USING_NS_CC;

using std::string;
using std::vector;





//邮件类型
/*
enum EmailType
	{
		ET_Invalid = 0,
		ET_SystemBonus,
		ET_SystemNotice,
		ET_FriendChat,
	};
	*/
enum MailType_layer{ 

	MailType_Invalid ,	//无效类型
	MailType_Bonus 	,	//系统奖励
	MailType_Notice ,	//系统通知 
	MailType_Friend ,	//好友交互
};


struct Datanet{

	 char* msg;
	 int size;

};

struct Layerst{
	 
	  int parent_number;
	  int son_number;
};

struct UnitLayer {
     int type ;         //  required 类型 
     bool is_activate ;    //  required 激活状态 true:已激活, false:未激活
     bool is_use     ;    // required 使用状态 true:已使用, false:未使用
     int level   ;    //optional  等级 魔法物无等级
};

struct NetPalyermsg{

    int cur_group;    //当前所在分组
    int last_group;    //上次结算时所在分组 (没有就没有奖励, 有就发送奖励)
    int last_order;    //上次结算时排名

	NetPalyermsg(){
		cur_group=-1;
		last_group=-1;
		last_order=-1;
	}

};

struct RankUser{


	int user_id ;    //用户id
    int user_order;    //排名
    int score ;    //入榜时的分数
	int distance;  //入榜时跑的距离
    string user_name;    // optional 用户名

	UnitLayer hero; 
	UnitLayer pet;    //宠物
    UnitLayer magic;    //魔法物

	RankUser(){
		user_id=-1;
		pet.level=-1;
		magic.level=-1;
	}



};


//道具信息
struct ItemInfo_layer { 
    int type  ;	//道具类型
    int count ;	//道具数量
};


//附件信息
struct AttachInfo_layer { 

    vector<ItemInfo_layer> items ;    //奖励的道具
    int vitality;    //体力值
};


//单个邮件信息
struct  MailInfo_layer
{ 
    MailType_layer type;	//   required MailType
    bool is_read;	//阅读状态 true:已读, false:未读
    int id;    //邮件ID
    int addresser_id;    //发件人ID 0:为系统, >0:玩家

    AttachInfo_layer attach;    //奖励
    string addresse;    //发件人姓名
    string content;    //邮件描述或者内容
};


//道具信息
struct ItInfo { 
    int type;	//道具类型  required
    int count;	//道具数量  required
	ItInfo(){

		type=unitID_Gold;
		count=0;
	}
};


struct AInfo { 
    vector<ItInfo> items;    //奖励的道具  repeated
    int vitality;    //体力值
};

//单个排行榜奖励信息
struct RBonusInfo { 
    int id;             //排行榜奖励ID  required
    
    int group_id;       //分组ID
    string group_name;  //排名分组组名: 对应cs_common.h中的RankGroupID
    
    int bonus_id;      //组内奖励ID: 对应cs_common.h中的RankBonusID
    AInfo attach;      //奖励
};


struct Warld{

	int  id;         //ward ID
	int  count;      //物品数量
	int  type;       //物品id
    bool is_get;	//是否领取奖励 false:未领取, true:已领取
    bool enable;	//是否可以领取奖励 false:不能领取 true:可以领取
};