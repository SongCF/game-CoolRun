#pragma once
#include "cocos2d.h"
#include "string.h"

#include "protocol_message.pb.h"
#include "AchievementHelper.h"
#include "extensions/cocos-ext.h"
#include "GoodsHelper.h"
#include "MarketHelper.h"
#include "ActorHelper.h"
#include "MsgDefine.h"
#include "TollgateHelper.h"
#include "PlayerInfoHelper.h"
#include "BaseLayer.h"
#include "Uistruct.h"
#include <iostream> 
#include <cmath>
#include <functional>
#include "RandomGiftBagHelper.h"
#include "BaselisttableLayer.h"

USING_NS_CC;

typedef std::function<void(int tag)> MsgC;

class NetmsgManger
{
public:
	NetmsgManger(void);
	~NetmsgManger(void);

	static NetmsgManger* create();//create静态函数，在其中设置单例，并调用init方法
	static NetmsgManger* getNetManager();


	vector<Actor> heros;
	vector<Actor> pets;
	vector<Actor> magis;

	void  setAllsale();

	void msgProc(int err, unsigned short type, void* data,unsigned int size);

	void  isregist();


	//
	void load_data();

	//2, 为 排行旁
	vector<RankUser> netSports;
	NetPalyermsg msgNet;
	void loadNetRank();

	int GetDaysInMonth(int y,int m);
	//3 排行旁奖励
	vector<RBonusInfo>  bounds;
	void loadNetboundsinfo();

	//获取我的组名
	string getMy_name();
	//获得我的奖励
    ItInfo get_myItinfo();

	//修改昵称
    void updateNickname(string nick_name);


	void sendUpdateLibao();

	int tag;// 1为注册 2, 为 排行旁 3 排行旁奖励  4 为修改昵称

	int iswhoLoad; // 1 为baselist  2为Sports
	
	void reiveMsg(int tag,int error);

	vector<int>  cs;
	void loadCsdata();
    
	//
	void sendLoginday();
	vector<Warld> wards;
	void linquDay(int warld_Id);


	vector<MailInfo_layer> mails;
	void sendMail();


	void updateUserinfo();
	
	string playername;
	int value_num;

	//65535 max 最大
	//
    unsigned int feizhuangoneint(unsigned int id,unsigned int value);
	//解析
	MsgData  getUnitdata(int he); 


	//登陆操作
	void Loginoparte();

	void Czdata();
	int mark;
	bool islogin;
	void setisLogin();
	int getCurrenttime();

	
	int errorrank;

	bool chaochuSevenday(int time1,int time2);
public:

	
	static NetmsgManger* sharedNetmsgManger;//全局单例标志，以此来返回单例


	//登陆
	CoolRun::LoginReq getLoginReq();


	/*
	  返回注册类
	*/
	CoolRun::RegistReq  getregistReq(CoolRun::PlatformType type,string acount="1",string name="2",string pass="3");

    // id 周期score还没实现  更新用户信息请求包 
	CoolRun::UpdateInfoReq  getupdateInfoReq();
    
    // 查询排行榜请求包
	CoolRun::QueryRankListReq getQueryRankListReq();


	//查询排行榜奖励请求包
	CoolRun::QueryRankBonusReq getQueryRankBonusReq();



	CoolRun::QuerySystemBonusReq getQuerySystemBonusReq();

   
	//领取系统奖励请求包
	CoolRun::GetSystemBonusReq   getGetSystemBonusReq(int wardId);


	//查询信息
	CoolRun::QueryMailReq  getQueryMailReq();

	
	//阅读邮件请求包
	CoolRun::ReadMailReq  getReadMailReq(int ID);


	//修改昵称
	CoolRun::AlterInfoReq getAlterInfoReq(string nickname);


	//获取随机道具权重
	CoolRun::QueryGiftBagReq  getQueryGiftBagReq();


public:

	bool  yinyue_state;
	bool  yinxiao_state;
	bool  texiao_state;

	int IsLeapYear();
	int month_day(int m);
	int DaySearch();
	int year;
	int month;
	int day;

//一个数据接收后的通知  如修改昵称
	vector<pair<BagItem, unsigned int>> lbs;
public:
	void MsgCenter(int tag)    //1 为成功 0 为失败
	{
		if(msg_c){
		 msg_c(tag);
		  CCLOG("1234");
		}
	}
	void setMsg_Center(const MsgC &func){
		msg_c=func;
	}
	void Csmsgis_null();
	void setMsgc_null();
	bool isdesdoy;
	void sendMsg(std::string type,int mark);
	bool isstart;
private:
	MsgC msg_c;

	
};

