#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
#include "string.h"
#include "ImageHelper.h"
#include "BaseSprite.h"
#include "AchievementHelper.h"
#include "MarketHelper.h"
#include "FuncTools.h"


#include "PlayerInfoHelper.h"
#include "ActorHelper.h"
#include "GoodsHelper.h"
#include "BaselisttableLayer.h"
#include "StringsMap.h"
#include "ImageHelper.h"
#include "Music.h"
#include "protocol_message.pb.h"

#include "NetmsgManger.h"
#include "ID_Define.h"
#include "EditBoxTest.h"
#include "Uistruct.h"
#include "BaselisttableLayer.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

USING_NS_CC;
USING_NS_CC_EXT; 



#define power "Power"
#define hp     "HP"

#define Tophight 90

#define Design_width 960
#define Design_hight 640
#define Gold_labeltag   1
#define Jewel_labeltag  3


#define Mainui_return  17

#define  MaxNodes 30

#define Daoju_list   101
#define Chengjiu_list  102
#define Shangchen_list  103

#define Juese_list   104
#define Chongwu_list 105
#define Gongju_list  106
#define Toll_list  107
#define Sports_list 108
#define Msgnet_list 109



#define Sdbuy "shangdian"
#define Cj  "chengjiu"

#define Layerchange "Layer_p"
#define MSG_TollgateLayer_comein "MSG_goto_TollgateLayer"
#define Changeacive "acives"
#define GkMsg "msg"
#define GkMsg_1 "msg_1"

#define Randxiaoguo "randxiaoguo"

#define Huoyuedu    "huoyueduTj"

#define Youjiantx   "youjiantx"

#define Huoyuelinjian "huoyuelinjian"

#define UI_Layerc "uilayerch"

#define Load_rankBounds  "bouds"
#define Load_ranList  "ranklist"

#define Biti_imgstr  "ui_anniu_tongyong_zhuye_lanse.png"

#define Biti_imgstr2  "ui_anniu_tongyong_jiaoxue.png"

#define Biti_imgstr3  "ui_anniu_tongyong_jiaoxue.png"

#define Colorzdy  Color3B(100,100,0)

#define update_jiwel -50


#define Logoinmsg   "logmsg"
#define Topmsg_ch   "ch_topmsg2"

using namespace cocostudio; 



struct LayerData
{
	unsigned int push;//返回页面id
	unsigned int current;//当前页面id 不如zha
	unsigned int heroid;// 
	unsigned int petid;// 
	unsigned int magicid;// 
};


enum Layertag{

	 gold_tag=106,
	 gold_tag_close,
	 zs_tag,
	 zs_tag_close,
	 yinyue_set_tag,
	 yinyue_o_tag,
	 yinxiao_o_tag,
	 blayertag,
	 yinyue_qian_kuan,
	 yinyue_hou_kuan,
	 yinyueone_b,
	 yinyuetwo_b,
	 yinxiao_qian_kuan,
	 yinxiao_hou_kuan,
	 yinxiao_one_tag,
	 yinxiao_two_tag,
	 close_set_layer_tag,
	 xinshou_yindao_tag,
	 game_returntag,
	 menu_tag,
};


typedef std::function<void(void)> Ward;



class BaseLayer :public Layer
{
public:

	
	
	CREATE_FUNC(BaseLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	
	



	BaseLayer(void);
	BaseLayer(std::string msg);
	~BaseLayer(void);
	inline void setCallBackHandler(Ref *obj,SEL_CallFuncO handler)
	{
		m_oTarget = obj;
		m_hCallBackHandler = handler;
	}

	void Shieldtouched();
	Color4B color;
	void showShake(Node* sp);
	void craeteLayer();
    //成就提示框
	void Achievementpromptbox(unsigned int id);

	void Achievementpromptbox_net(unsigned int id);

	//金币商店提示框
	void goldbox(std::function<void()> call_back = nullptr);
	void closeGame();
	void shopsW();
	//钻石提示框
	void jiewelbox();
	//关卡模式提示框
	void gkmodebox();

	void Swbox();
	//是否到
	void goShop(int id,unsigned int wupingId=0);  
	void goSop(Ref* sender);
	void Gameover(Ref* sender);
	//金钱不足提示
	void  money_no(int tag);
	//活跃框
	void risk_box();
	//活跃框奖励
	void active_warldbox(BagItem unit);
	void linqu_active(Ref* ref);
	ProgressTimer* progress;
	Label* b_l;
	void Conset();

	void drawSetmusic();

	void xinshouCallback(Ref* ref);
	void fillOuty(MenuItemImage* p,string imgfile);
public:
	float Actual_x;
	float Actual_y;

	Ref				*m_oTarget;
	SEL_CallFuncO	m_hCallBackHandler;

	void Gosd(Ref* sender);
	void returnPa(Ref* sender);

	void freep();
	void reward(Ref* sender);
	void reward_net(Ref* sender);

	void updateName(Ref* ref);



	void shopclose(Ref* sender);
	int getDelwithint(int number,int iswidth);

	void gouMai(Ref* sender);

	static BaseLayer* baselayer;//全局单例标志，以此来返回单例

	static BaseLayer* getBaseLayer();

	void Netcontact();

	void Regists();

	void Registf();

	void Tishistring(string str);

	void modify_nameBox();
	

	void daoju_reward2(Ref* sender);


	EditBoxTest *mody_name;
	void mody_nameYes(Ref* ref);

	void huoD_ward(int Mc,int type,int ward,string Zm);
	//ItInfo ifo
	void huoD_ward(int Mc,vector<ItInfo> ifos,string Zm);

	void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);
	int ward_type;
	int ward_num;
	void lingqu_netBack(Ref* ref);
public://网络返回消息处理

	void return_Msg(int tag);
	void returnmody_name(Ref* sender);

public:

	void MsgCenter()    //true为成功 false为失败
	{
		if(msg_c){
		  msg_c();
		}
	}
	void setMsg_Center(const Ward &func){
		msg_c=func;
	}

	bool  yinyue_state;
	bool  yinxiao_state;
	bool  texiao_state;

	void  setJiemian_box();
	Scale9Sprite* scal8;
	
	void Close_button(Ref* ref);
    void Yinyueset_button(Ref* ref);

	int getOparatetag();
	void setOparatetag(int tag);

	//功能
	bool isContactpoint(Vec2 p,Sprite* sp);
private:
	Ward msg_c;
	Scale9Sprite* ww;
	void daoju_reward(Ref* sender);

	BagItem bag_l;
	EventListenerTouchOneByOne* listen;

	int opratetag;
	vector<Sprite*>  touchsps;
	unsigned int state;
};
