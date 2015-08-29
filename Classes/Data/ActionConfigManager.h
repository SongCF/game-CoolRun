//============================================================
//* 动画配置类
//* 所有动画的配置，获得相应文件图片
//============================================================

#pragma once

#include "UnitDefine.h"
#include <string>
#include <vector>
#include "XReadJson.h"
using std::string;
using std::vector;

enum configActionID
{
	action_None = 0,

	action_FeiXingBi = 1,

	action_AiShuZhangChu = 2,
	action_GaoShuZhangChu = 3,

	action_YeZhuHuXi = 4,
	action_HouZiHuXi = 5,

	action_ShiTouFeiXing = 6,
	action_MiFengFeiXing = 7,

	action_JianGuoFanZhuan = 8,
	action_CaoMeiGuoFanZhuan = 9,
	action_LanMeiGuoFanZhuan = 10,

	action_HuoJianShanGuang = 11,
	action_HuoLongGuoShanGuang = 12,
	action_CiTieShanGuang = 13,
	action_MoFaBangShanGuang = 14,
	action_WuCaiQiPaoShanGuang = 15,

	action_ChongJi = 16, //冲击、飞出来：蜜蜂、导弹触发后飞出来，从右到左 
	action_Gun = 17, //石头自身原地滚动动画
	action_GunDong = 18, //石头往前滚动动画
	action_LuMianTaXian = 19, //地面塌陷
	action_BossMake = 20, //Boss制造道具
	action_LuMianShangsheng = 21,//地面上升
//	action_ZhongDu = 22, //中毒
	action_XiaoXiTiShi = 23, //消息提示
	
	action_XuanFuWu_Up = 25,	//悬浮物上升
	action_XuanFuWu_Down = 26,	//悬浮物下落
	
	action_XianDianLianUpDown = 27, //闪电链上下来回浮动
	action_ZhongDuJieYaoPingZhiShanSuo = 28,//中毒解药瓶上方箭头动画

	action_JiaoXue_LongHuXi = 29,//教学龙的呼吸动画
	action_PenDu = 30,//喷放毒气
	action_ScaleBy2 = 31,//变大一倍
	action_FadeOut = 32,//逐渐消失
};


struct ActionItem
{
	unsigned int ID;
	unsigned int type;
	float value;

	string name;
	string describe;

	string musicFile;
	string actionFile;

	ActionItem(){
		this->ID = 0;
		this->type = 0;
		this->value = 0;
	}
	ActionItem(const ActionItem& in){
		this->ID = in.ID;
		this->name = in.name;
		this->describe = in.describe;
		this->type = in.type;
		this->musicFile = in.musicFile;
		this->actionFile = in.actionFile;
		this->value = in.value;
	}
	const ActionItem& operator=(const ActionItem& in){
		this->ID = in.ID;
		this->name = in.name;
		this->describe = in.describe;
		this->type = in.type;
		this->musicFile = in.musicFile;
		this->actionFile = in.actionFile;
		this->value = in.value;
		return *this;
	}
};

class ActionConfigManager
{
private:
	static ActionConfigManager* s_mActionConfigManager;
	ActionConfigManager();
	virtual ~ActionConfigManager();
	void initData();
public:
	static ActionConfigManager* getManager();
	static configActionID getTypeById(unsigned int actionID);

	const vector<ActionItem>* getAllActionItem();
	const ActionItem* getActionItemByID(unsigned int actionID);

protected:
	vector<ActionItem> m_ActionList;
};