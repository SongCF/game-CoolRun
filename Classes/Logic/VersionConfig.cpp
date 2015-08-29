#include "VersionConfig.h"

#include <string>
#include "cocos2d.h"

using std::string;
USING_NS_CC;

#define FILE_Path "config/config.txt"
#define KEY_RootConfigInfo "ConfigInfo"
#define KEY_RootDebug "Debug"

#define KEY_key "key"
#define KEY_value "value"
#define KEY_describe "describe"


//*******************************************************************************************
//*     CONFIG
unsigned int g_uOneMeterWidthInPixel = 70;								//多少像素算 1 米
unsigned int g_uFightModeSwitchMeter = 1000;							//切换战斗模式需要的米数
unsigned int g_uSuperModeSwitchScore = 2800;							//切换超跑模式需要的分数
unsigned int g_uPreloadMapWidthInPixel = 300;							//游戏地图预加载屏幕之外的像素
float g_fGridWidth = 24.0;												//配置工具一个格子的像素大小
float g_fConfigOneScreenPixelWidth = 1440;								//配置工具规定一个屏幕的宽度60*GridWidth
float g_fMapScrollDelta = 7.7;											//初始滚动每秒16格 g_fGridWidth*16/60
float g_fHeroJump1Height = 132.0;										//角色一段跳高度(GridWidth*5.5f)
float g_fHeroJump2Height = 84.0;										//角色二段跳高度(GridWidth*3.5f)
float g_fHeroJump3Height = 84.0;										//角色三段跳高度(GridWidth*3.5f)
float g_fJump1NeedTime = 0.31;											//一段跳所需时间，二三段跳、弹跳等会与之关联
float g_fMaxZuanShiFuHuoTimes = 1;										//配置最大的钻石复活次数
float g_fMaxSiWangChongCiTimes = 1;										//配置最大的死亡冲刺次数
float g_fItemSpeedUpMultiple = 1.2;										//加速道具 当前速度增加的倍数
float g_fRushSpeedMulti = 3.5;											//配置冲刺速度的基础倍数
float g_fSuperSpeedMulti = 2.0;											//配置超跑速度的基础倍数
float g_fMaxGoldEffectCount = 5;										//配置金币效果的个数
unsigned int g_uGameInEffectSwitch = 1;									//游戏场景内特效开关
int g_iGameFuHuoCostDiamond = 10;										//游戏中复活花费的钻石数量
//*
//*******************************************************************************************
//*      DEBUG
unsigned int DEBUG_uMessageBox = 0;									//是否弹错误提示框
unsigned int DEBUG_uGameInAttrMenu = 0;								//是否显示游戏内道具使用按钮
unsigned int DEBUG_uGameInDebugInfo = 0;							//是否显示游戏内调试数据
unsigned int DEBUG_uGameInCollisionRect = 0;						//是否显示游戏内碰撞矩形
//*
//*******************************************************************************************



VersionConfig* VersionConfig::m_versionConfig = nullptr;
VersionConfig::VersionConfig()
{

}
VersionConfig::~VersionConfig()
{

}

VersionConfig* VersionConfig::getConfig()
{
	if (!m_versionConfig) {
		m_versionConfig = new VersionConfig();
		m_versionConfig->init();
	}
	return m_versionConfig;
}

bool VersionConfig::init()
{
	string filePath = FileUtils::getInstance()->fullPathForFilename(FILE_Path);
	Data dat = FileUtils::getInstance()->getDataFromFile(filePath);
	if (dat.getSize() <= 0)
	{
		CCLOG("%s ---> load %s error", __FUNCTION__, filePath.c_str());
		return false;
	}

	char *buf = new char[dat.getSize()+1];
	strncpy(buf, (char*)dat.getBytes(), dat.getSize());
	buf[dat.getSize()] = 0;
	CCLOG("%s ---> %s\n", __FUNCTION__, buf);

	rapidjson::Document d;
	d.Parse<0>(buf);
	delete [] buf;
	if (d.HasParseError())  //打印解析错误
	{
		CCASSERT(false,"");
		CCLOG("%s ---> GetParseError %s\n", __FUNCTION__, d.GetParseError());
		return false;
	}
	else if (d.IsObject()) 
	{
		bool ret1 = initConfigData(d);
		bool ret2 = initDebugData(d);
		return (ret1 && ret2);
	}
	else 
	{
		return false;
	}
	return false;
}

bool VersionConfig::initConfigData(rapidjson::Document& d)
{
	if (d.HasMember(KEY_RootConfigInfo) && d[KEY_RootConfigInfo].IsArray() && d[KEY_RootConfigInfo].Size() > 0)
	{
		const rapidjson::Value& arr = d[KEY_RootConfigInfo];
		for (int i=0,count=arr.Size(); i<count; ++i)
		{
			string key;
			if (arr[i].HasMember(KEY_key) && arr[i].HasMember(KEY_value)){
				key = arr[i][KEY_key].GetString();
			}else {
				CCASSERT(false, "");
				return false;
			}

			CCLOG("info: %s ---> %s : %.2f", __FUNCTION__, key.c_str(), arr[i][KEY_value].GetDouble());
			//多少像素算 1 米
			if ("OneMeterWidthInPixel" == key)
			{
				g_uOneMeterWidthInPixel = arr[i][KEY_value].GetUint();
			}
			//战斗模式需要多少米
			else if ("FightModeSwitchMeter" == key)
			{
				g_uFightModeSwitchMeter = arr[i][KEY_value].GetUint();
			}
			//超跑模式需要多少分
			else if ("SuperModeSwitchScore" == key)
			{
				g_uSuperModeSwitchScore = arr[i][KEY_value].GetUint();
			}
			//游戏地图预加载屏幕之外的像素
			else if ("PreloadMapPixelWidth" == key)
			{
				g_uPreloadMapWidthInPixel = arr[i][KEY_value].GetUint();
			}
			//配置工具一个格子的像素大小
			else if ("GridWidth" == key)
			{
				g_fGridWidth = arr[i][KEY_value].GetDouble();
			}
			//配置工具规定一个屏幕的宽度60*GridWidth
			else if ("ConfigOneScreenPixelWidth" == key)
			{
				g_fConfigOneScreenPixelWidth = arr[i][KEY_value].GetDouble();
			}
			//初始滚动每秒16格 g_fGridWidth*16/60
			else if ("MapScrollDelta" == key)
			{
				g_fMapScrollDelta = arr[i][KEY_value].GetDouble();
			}
			//角色一段跳高度(GridWidth*5.5f)
			else if ("HeroJump1Height" == key)
			{
				g_fHeroJump1Height = arr[i][KEY_value].GetDouble();
			}
			//角色二段跳高度(GridWidth*3.5f)
			else if ("HeroJump2Height" == key)
			{
				g_fHeroJump2Height = arr[i][KEY_value].GetDouble();
			}
			//角色三段跳高度(GridWidth*3.5f)
			else if ("HeroJump3Height" == key)
			{
				g_fHeroJump3Height = arr[i][KEY_value].GetDouble();
			}
			//角色二段跳高度(GridWidth*3.5f)
			else if ("Jump1NeedTime" == key)
			{
				g_fJump1NeedTime = arr[i][KEY_value].GetDouble();
			}
			//配置最大的钻石复活次数
			else if ("MaxZuanShiFuHuoTimes" == key)
			{
				g_fMaxZuanShiFuHuoTimes = arr[i][KEY_value].GetDouble();
			}
			//配置最大的死亡冲刺次数
			else if ("MaxSiWangChongCiTimes" == key)
			{
				g_fMaxSiWangChongCiTimes = arr[i][KEY_value].GetDouble();
			}
			//加速道具 增加的倍数
			else if ("ItemSpeedUpMultiple" == key)
			{
				g_fItemSpeedUpMultiple = arr[i][KEY_value].GetDouble();
			}
			//配置冲刺速度的倍数
			else if ("RushSpeedMulti" == key)
			{
				g_fRushSpeedMulti = arr[i][KEY_value].GetDouble();
			}
			//加速道具 增加的倍数
			else if ("SuperSpeedMulti" == key)
			{
				g_fSuperSpeedMulti = arr[i][KEY_value].GetDouble();
			}
			//配置最大的金币效果数
			else if ("MaxGoldEffectCount" == key)
			{
				g_fMaxGoldEffectCount = arr[i][KEY_value].GetDouble();
			}
			//游戏场景内特效开关
			else if ("GameInEffectSwitch" == key)
			{
				g_uGameInEffectSwitch = arr[i][KEY_value].GetDouble();
			}
			else 
			{
				CCASSERT(false, "error branch");
			}
		}
		return true;
	}
	return false;
}


bool VersionConfig::initDebugData(rapidjson::Document& d)
{
	if (d.HasMember(KEY_RootDebug) && d[KEY_RootDebug].IsArray() && d[KEY_RootDebug].Size() > 0)
	{
		const rapidjson::Value& arr = d[KEY_RootDebug];
		for (int i=0,count=arr.Size(); i<count; ++i)
		{
			string key;
			if (arr[i].HasMember(KEY_key) && arr[i].HasMember(KEY_value)){
				key = arr[i][KEY_key].GetString();
			}else {
				CCASSERT(false, "");
				return false;
			}

			CCLOG("debug: %s ---> %s : %.2f", __FUNCTION__, key.c_str(), arr[i][KEY_value].GetDouble());
			//解析unitID错误时，是否显示提示框（1显示 0隐藏）
			if ("InfoMessageBox" == key)
			{
				DEBUG_uMessageBox = arr[i][KEY_value].GetUint();
			}
			//游戏内右侧使用道具的按钮（1显示 0隐藏）
			else if ("GameInAttributeMenu" == key)
			{
				DEBUG_uGameInAttrMenu = arr[i][KEY_value].GetUint();
			}
			//游戏内调试信息---帧率、场景速度、道具个数（1显示 0隐藏）
			else if ("GameInDebugInfo" == key)
			{
				DEBUG_uGameInDebugInfo = arr[i][KEY_value].GetUint();
			}
			//游戏内碰撞矩形的显示与否（1显示 0隐藏）
			else if ("GameInCollisionRect" == key)
			{
				DEBUG_uGameInCollisionRect = arr[i][KEY_value].GetUint();
			}
			else 
			{
				CCASSERT(false, "error branch");
			}
		}
		return true;
	}
	return false;
}