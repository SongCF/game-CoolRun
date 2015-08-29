//============================================================
//* 游戏场景内（一共五层结构）
//* 第三层 ：游戏精灵单元层
//* 功能 ：所有配置的单元都通过该层显示，主要通过配置信息 正确显示各个单元
//* 依赖 ：GameSceneFactory
//============================================================


#pragma once

#include "cocos2d.h"
#include "RunningHelper.h"
#include "GameDirector.h"

using namespace cocos2d;

class HeroSprite;
class PetSprite;
class ObstacleSprite;
class BossSprite;


//#define GridWidth 24.f   //配置工具一个格子表示24像素
//#define ScrollDelta (GridWidth*16/60) //初始滚动每秒16格
//#define ConfigOneScreenWidth (60*GridWidth)

class GameUnitLayer : public Layer
{
public:
	enum _iTag
	{
		Tag_DaNiao = 2000,
	};

public:
	static GameUnitLayer* create(RunningMode mode, vector<int> sectionInfor);
	bool init(RunningMode mode, vector<int> sectionInfor);
	GameUnitLayer();
	~GameUnitLayer();
	void start(float dt);
	void start();

	void startScroll();
	void stopScroll();
	float getScrollWidth();
	float getScrollMulti();
	void setScrollMultitypeWidth(float based_multi);

	//每1000米为一个场景
	CC_SYNTHESIZE(unsigned int, m_uCommonSceneCount, CommonSceneCount);
	//战斗场景
	CC_SYNTHESIZE(unsigned int, m_uFightSceneCount, FightSceneCount);


	static float getHeroDefaultPosX(){ return (Director::getInstance()->getWinSize().width-768);}
	//获取当前人物踩到的路面的位置, true 表示hero正踩到的， false表示hero默认x处的路面
	//返回 找到陆地否
	std::pair<bool,Sprite*> getRecentLandRect(Rect& rect, bool herotread);
	Vec2 getLastLandWorldPos();//获取最后加载的一个陆地 中上位置

	HeroSprite* getHeroSprite() {return m_hero;}
	PetSprite* getPetSprite() {return m_pet;}
	Node* getMagicSprite() {return m_magic;}
	Layer* getObstacleContainerLayer() {return m_obstacleContainer;}
	void cleanTempObstacleLayer();
	cocos2d::Vector<ObstacleSprite*>* getObstacle() {return &m_obstacles;}

	void getPreloadSceneItemQueue(vector<SceneItem>& vec);
	Rect CalculateSceneItemRectInWorldSpace(const SceneItem& item);

	void cleanSceneItemQueue();

	void resetScrollLayer(bool bLoadMap = false, Scene_PlayMode preMode = mode_Common);//Scene_PlayMode不用关心，只有超跑转普通的时候会用
	void startCommonMode(Scene_PlayMode preMode);
	void startSuperMode();
	void startFightMode();
	void superModeEnd(float dt);

	void addMaskScale(float fscale = 0.5f);
	void removeMaskLayer();
	void addMaskLayer(const Vec2& pos);
	virtual void update(float dt) override;

	void correctLandWillSuperRun();

	void addRanklistHero();
	void removeRanklistHero();

	void addFeiLong(Point pos);

public:
	void moveLayer_byMoveCamera(float time, float delta);
	float getCurMeterShouldSpeed(/*long meter*/);//获取当前默认应该的速度,现在跟meter无关了
	float getCurModeBaseSpeed();//获取当前模式的基础数据

protected:
	void tick(float dt);
	void scrollLayer();
	void preloadSceneItems(bool loadAtOnce = false);
	const vector<SceneItem>* loadConfigSceneInfo();

	void updateRunMeter();

	void clearCollisionVector();
	void recoveryHeroPos(ObstacleSprite* pObs); //英雄被主档后，如果被阻挡的木桩移触，则英雄开始位置恢复 by jj

	void updateMaskPos(float dt);
	void setMaskPos(const Vec2& pos);
	void scaleSmallMask(float dt);
	void scaleBigMask(float dt);
public:
	//fight  ---> endless
	void bossComeIn();
	void bossLeave();
	BossSprite *getBoss();

	// ---> tollgate
	void bossComeIn(unsigned int bossUnitID);
private:
	Size mWinSize;

	HeroSprite * m_hero;
	PetSprite  * m_pet;
	Node * m_magic;

	//  m_curScrollWidth = m_curScrollBaseWidth * m_curSpeedMulti
	float m_curScrollBaseWidth;//当前模式滚动的基础宽度
	float m_curScrollWidth;//当前滚动宽度  （已计算倍数）
	float m_curSpeedMulti;//当前滚动倍数 （基础速度的倍数）

	RenderTexture*		m_pBlackLayer;
	Sprite* m_pMask;
	float m_fCurScale;
	float m_fWillScaleTo;

//	Layer *m_obstacleContainer1,*m_obstacleContainer2;//两个层交替滚动，一个配置信息保证在2屏宽度
	Layer *m_obstacleContainer;
	Layer *m_tempObstacleContainer;
	cocos2d::Vector<ObstacleSprite*> m_obstacles; //主要用于碰撞检测


	//场景物品
	//很多地方需要访问中间节点、每次转换为vector，损失效率
//	queue<SceneItem> m_sceneItemsQueue;
	vector<SceneItem> m_sceneItemsVec;
	long m_sceneItemsVecIdx;

	unsigned int m_alreadyLoadSceneCount; //每种模式从该模式开始算起，加载了多少个场景了（超跑完10个场景就结束）
	float m_fSceneConfigTotalWidth;
	float m_fCurSceneConfigWidth;

	//
	RunningMode m_runningMode;
// 	unsigned int m_runningSeasonId;
// 	unsigned int m_runningSectionId;
// 	unsigned int m_diffictLevel;
	vector<int> m_runningSecionInfor; // seasonid,sectionid,diffictlevel

	EventListenerCustom* m_lvUpListener;

	bool m_bChangeToSuper;   //在将要切换至super模式时，obsLayer不能上下移动
};