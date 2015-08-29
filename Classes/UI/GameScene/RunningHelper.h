//============================================================
//* 奔跑场景的帮助类
//* 各层之间的交互通过它完成
//============================================================


#pragma once

#include "cocos2d.h"
#include "GameSceneFactory.h"
#include "TollgateHelper.h"
#include "GameDirector.h"
#include <queue>
#include <map>
#include <string>
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

using std::queue;
using std::map;
using std::string;

class HeroSprite;
class ObstacleSprite;
class GameUnitLayer;
class GameInTouchLayer;
class UnitCollisionDetection;
class StateBar;
class SimpleBackgroundLayer;
class SimpleFrontgroundLayer;


using namespace cocos2d;

#define MSG_RefreshBackground "refresh_background"

#define MSG_ShockWave "hero_shock_wave"   //英雄极速飞行完了之后发出冲击波、前方固定范围内的障碍物摧毁，陷进填上彩虹桥
#define MSG_Absorbe   "hero_absorbe"      //英雄吃到磁铁 每帧发送一个该消息，obstacle每次收到该消息像英雄方向移动固定距离
#define MSG_ChangeGold   "change_gold"      //英雄吃到点金 每帧发送一个该消息，obstacle每次收到该消息把一定范围内的其他铜币银币变成金币


class RunningHelper : public Layer
{
private:
	static RunningHelper* m_runningHelper;
	RunningHelper();
	map<string,string> m_oneGameLog;//一场游戏的信息，用于analytics
public:
	static RunningHelper* getHelper();
	map<string,string>* getGameLog(){return &m_oneGameLog;}

	void initStartGame(Scene* pRunningScene, RunningMode mode, vector<int> sectionInfor);
	void gameOver();
	void gameReborn(bool bRocket = false);
	void gameWin(); //关卡模式到达终点
	void gamePause();//教学才使用
	void gameResume();//教学才使用

	void teach(string attr);

	void MoveCamera(float time, float deltaY, bool castSameTimeBack=false);
	void scaleCamera(float time, float scale, float backTime=0);// backtime ==0 时 缩放后不会回去
	float getCameraScale();

	//endlessMODE---> score       tollgateMODE---> 红宝石
	void addScore(float num, bool bGold = false);
	unsigned int getScore();
	void addJewel(float num){m_fTotalJewel += num;}//该接口用于计算该关卡一共多少红宝石
	unsigned int getTotalJewel(){return (unsigned int)m_fTotalJewel;}
	void addMeter(float num);
	unsigned int getMeter();
	float getHeroAttributeAddMultNum(string attr); //结算金币加成

	//获取当前场景配置信息 gameUnitLayer每次调用都换更换当前sceneInfo
	//vector 需要传入    seasonid  sectionid  difficultlevel
	SceneInfo getOneSceneInfo(RunningMode mode, vector<int> sectionInforVec);
	//获取当前背景信息
	const BackgroundList* getFirstBackground();
	const BackgroundList* getCurrentBackground();

	//获取当前跑的人物
	HeroSprite* getRunnerHero();
	//获取当前跑酷场景的所有障碍物和道具
	cocos2d::Vector<ObstacleSprite*>* getObstacles();
	GameUnitLayer* getGameUnitLayer();
	UnitCollisionDetection *getUnitCollsionDetection();
	GameInTouchLayer *getGameInTouchLayer();
	StateBar *getCurrentStateBar();
	SimpleBackgroundLayer* getBackgroundLayer();
	SimpleFrontgroundLayer* getFrontgroundLayer();

	CC_SYNTHESIZE(RunningMode, m_curRunningMode, CurRunningMode);//关卡模式、无尽模式
	CC_SYNTHESIZE(Scene_PlayMode, m_curScenePlayMode, CurrentScenePlayMode);//超跑、战斗、正常
	int getAlreadyFightPlayCount(){return m_uFightPlayCount;}//已经fightPlay的次数
	vector<int> m_curSectionInfor; // seasonid,sectionid,level
	unsigned int getCurSeasonId(){if (m_curSectionInfor.size()>=1) return m_curSectionInfor[0]; else return 0;}
	unsigned int getCurSectionId(){if (m_curSectionInfor.size()>=2) return m_curSectionInfor[1]; else return 0;}
	Section::Difficulty getCurDifficutLevel(){if (m_curSectionInfor.size()>=3) return (Section::Difficulty)m_curSectionInfor[2]; else return Section::eNormal;}

	bool m_bIsGameIn;//当前是否在游戏场景  
	bool isGameIn(){return m_bIsGameIn;}
	void setGameIn(bool in){m_bIsGameIn = in;}

	//获取当前跑酷场景
	float getRunnerMultitypeSpeed();//场景滚动的速度 基础速度的倍数
	void setRunnerMultitypeSpeed(float based_multi); //设置跑酷速度  基础速度的倍数
	void superPlayStorageOldScene(vector<SceneItem>& vec, float curWidthInPixel); //进入超跑要保存上次的场景跑到哪里了  超跑出来接着跑
	void startSuperPlay();
	void startFightPlay();
	void endFightPlay();
	CC_SYNTHESIZE(bool, m_bFightEnd, FightEnd);
	void fightEndToCommon();
	void startCommonPlay();

	bool isTeachScene();

	//当前吃金币的 效果 正在播放的个数
public:	void setGoldEffectCount(int iGoldEffectCount){m_iGoldEffectCount = iGoldEffectCount;};
public:	int getGoldEffectCount(){ return m_iGoldEffectCount;};
protected:	int				m_iGoldEffectCount;

	//是否计算 meter
public:	void enableCalculateMeter(bool enable){m_bEnableCalculateMeter = enable;}
public:	bool isCalculateMeter(){ return m_bEnableCalculateMeter;}
protected:	bool				m_bEnableCalculateMeter;

public:
	void postRefreshBgMsg(int time_ms);//切换背景渐变的毫秒数  ms ,  必须是整数

protected:
	void playAnimationEnd(Armature *armature, MovementEventType type, const std::string& movementID);
protected:
	Scene* m_pRunningScene;

	queue<SceneInfo> m_curScene;
	SceneInfo m_preSuperScene; //超跑前的common 场景， 超跑完后 要接着上次的
	bool m_bUserOldCommonScene; // 超跑完后 是否使用上次的scene

	float m_displayScore;//endlessMODE---> score       tollgateMODE---> 红宝石
	float m_fTotalJewel;
	float m_displayMeter;

	//1
	unsigned int			m_uCommonMapCount;
	unsigned int			m_uOtherMapCount;

	unsigned int			m_uSuperPlayCount;
	unsigned int			m_uFightPlayCount;
};