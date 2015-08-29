//============================================================
//* 控制类
//* 逻辑层
//* 版本控制，（模式切换控制）
//============================================================

#pragma once

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#define CONFIG_ON 1
#define CONFIG_OFF 0


//*******************************************************************************************
//*     CONFIG
extern unsigned int g_uOneMeterWidthInPixel;
extern unsigned int g_uFightModeSwitchMeter;
extern unsigned int g_uSuperModeSwitchScore;
extern unsigned int g_uPreloadMapWidthInPixel;
extern float g_fGridWidth;
extern float g_fConfigOneScreenPixelWidth;
extern float g_fMapScrollDelta;
extern float g_fHeroJump1Height;									//角色一段跳高度(GridWidth*5.5f)
extern float g_fHeroJump2Height;									//角色二段跳高度(GridWidth*3.5f)
extern float g_fHeroJump3Height;
extern float g_fJump1NeedTime;										//一段跳所需时间，二三段跳、弹跳等会与之关联
extern float g_fMaxZuanShiFuHuoTimes; 
extern float g_fMaxSiWangChongCiTimes;
extern float g_fItemSpeedUpMultiple;										//加速道具 当前速度增加的倍数
extern float g_fRushSpeedMulti;												//配置冲刺速度的基础倍数
extern float g_fSuperSpeedMulti;											//配置超跑速度的基础倍数
extern float g_fMaxGoldEffectCount;
extern unsigned int g_uGameInEffectSwitch;									//游戏场景内特效开关
extern int g_iGameFuHuoCostDiamond;											//游戏中复活花费的钻石数量
//*
//*******************************************************************************************
//*      DEBUG
extern unsigned int DEBUG_uMessageBox;									//是否弹错误提示框
extern unsigned int DEBUG_uGameInAttrMenu;								//是否显示游戏内道具使用按钮
extern unsigned int DEBUG_uGameInDebugInfo;							//是否显示游戏内调试数据
extern unsigned int DEBUG_uGameInCollisionRect;						//是否显示游戏内碰撞矩形
//*
//*******************************************************************************************


class VersionConfig
{
private:
	VersionConfig();
	static VersionConfig* m_versionConfig;
	bool init();
	bool initConfigData(rapidjson::Document& d);
	bool initDebugData(rapidjson::Document& d);
public:
	~VersionConfig();
	static VersionConfig* getConfig();

};