//============================================================
//* 人物类
//* 提供人物的展示、游戏进行时的状态变迁、逻辑数据处理
//============================================================

#pragma once

#include "UnitSprite.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

#define Armature_HuXi					"r_daiji"
#define Armature_Pao					"r_pao"
#define Armature_hua					"r_xiahua"
#define Armature_Tiao_1					"r_tiao"
#define Armature_Tiao_2					"r_tiao2"
#define Armature_Xialuo					"r_xialuo"
#define Armature_Tiao_Guodu_1			"r_tiao_g"
#define Armature_Tiao_Guodu_2			"r_tiao2_g"
#define Armature_ChongCi				"r_chongci"
#define Armature_ShouShang				"r_shoushang"
#define Armature_SiWang					"r_siwang"
#define Armature_HuaXiangStart			"r_qifei"
#define Armature_HuaXiangING			"r_feixing"
#define Armature_ZuoQi					"r_zuoqi"
#define Armature_FeiLeiLe				"r_feileile"
#define Armature_FanGun					"r_xialuog"  //高空下落翻滚
#define Armature_ZhuanQuan				"r_caitanhuang"  //踩弹簧转圈
#define Armature_Congratulation			"r_qingzhu"

//默认陆地位置  maxY - OFFSET_Land_Hero
#define Land_Default_PosY		192
// landMaxY - OFFSET_Land_Hero  才是人物刚好踩的位置，让他们的矩形有5个像素的交集
#define OFFSET_Land_Hero		5

struct HeroAttribute
{
	bool onGoing; //当前是否有改属性
	float curTime;//效果 出现后， 当前的累计时间，或距离 （刚出现时curTime = 0）
	float endTime;//效果 结束值，  （当 curTime >= endTime效果结束）
	HeroAttribute()
	{
		onGoing = false;
		curTime = 0;
		endTime = 0;
	}
	HeroAttribute(const HeroAttribute &_in)
	{
		this->onGoing = _in.onGoing;
		this->curTime = _in.curTime;
		this->endTime = _in.endTime;
	}
	const HeroAttribute& operator=(const HeroAttribute &_in)
	{
		this->onGoing = _in.onGoing;
		this->curTime = _in.curTime;
		this->endTime = _in.endTime;
		return *this;
	}
};
class HeroSprite : public UnitSprite
{
public:
	enum HeroState
	{
//		state_DownRun,
		state_Run,
		state_DownGlide,		//下滑
		state_Jump1,
		state_Fall1,
		state_Jump2,
		state_Fall2,
		state_Jump3,
		state_Fall3,
//		state_GunDong,
		state_ChongCi,
		state_Dead,
		state_Super,//超跑
		state_Bounce,
		state_BounceBack,
	};

	enum ActionTag
	{
		Tag_JumpAction_1 = 1000,
		Tag_JumpAction_2,
		Tag_JumpAction_3,
		Tag_JumpFallAction, //1003
		Tag_RunAction,
		Tag_BounceAction, //教学完结后的动画

		Tag_ChongCiUpAction,
		Tag_ChongCiFallAction,
		Tag_ZhongDuAction,
		Tag_TimerLabelAction,
		Tag_TimerIconlAction,
		Tag_InvincibleAction,
	};

	enum ChildTag
	{
		Tag_Protect = 1000,
		Tag_Invincible,
		Tag_Rush,
		Tag_Super,
		Tag_Shake,
	};

	enum HP_ChangType
	{
		Type_Minus,
		Type_Add,
	};


public:
	static HeroSprite* create(Unit* unit);
	HeroSprite();
	~HeroSprite();
	//init
	virtual bool init(Unit *unit) override;
	void resetUnit(Unit *unit); //重新更新unit，会重置globeAttribute，所以需要重新setPet、setMagicGoods、setItem
	virtual void onEnter() override;
	virtual void onExit() override;
	void setPet(const Unit *pet);
	void setMagicGoods(const Unit* magic);
	void setItem(const Unit* item);
	virtual Rect getCollisonRectInWorldSpace() override;
	Rect getRectInObsContainerSpace();


	//call back
	virtual void onCollision(UnitSprite* pSprite = nullptr, enumCollisionPos board=collideLEFT) override;
	void noLandCollision();// 本次没有检测到陆地的碰撞，如果m_ijump是 0 和 -1 则播放下落动画，往屏幕下方掉下去。
	virtual void update(float fDelta) override;

public:
	void begin();// only call once 
	void beginAttributeEffect(float dt); //开场属性
	void arrivalTerminal(); //关卡模式：到达终点
	void playCongratulationAnimate();
	void reborn(bool isRocket);
	int getRebornTimes(){return m_iFuHuoTimes;}
protected:
	int							m_iFuHuoTimes;



	//hero位置
protected:
	float						m_fLandPosY;
	float						m_fLandHeight;
public:
	void willResetX();
	void stopResetX();
	void resetX(float dt);
	float getHeroYOffset();

	void correctCamera();
	void correctChongCiHero(float dt);



	// armature
protected:
	Armature*		m_pArmature;
	string			m_armatureFileName;
public:
	void addArmatureNode();
	Node* getArmatureNode(){return m_pArmature;};
	static pair<string, string> getArmatureInfo(unsigned int typeId);
	void pauseArmature();					//暂停ArmatureAnimation
	void resumeArmature();					//恢复ArmatureAnimation
	void playAnimation(const char* animationname);
	void playAnimationByState(HeroState state);
	string getCurPlayAnimationName();




	//Action
public:
	void resumeAction();
	void startRun();
	void resetRun();
	void stopRun();
	void jump(); //跳
	void DownGlide(); //下滑
	void jumpHighestPoint();//调到最高点了
	void fall();//下落（跳完之后的下落动画）参数fMoreYoffset：需要在原来的基础上增加的Y方向的下落距离
	void fallDone(); //下落完成
	void fight();//打击
	void beatBoss(UnitSprite *pSprite, float value); //战斗模式： 吃到攻击性道具

	void injured(int subHp);//触发受伤（撞到触发受伤的物品调用，然后再在这里面判断是否有无敌状态，如果没有才会触发真正的死亡）
	void die();//真的死亡
protected:
	//用于检测下落时穿透路面
	//开始下落时开启   落地时关闭
	void recordFallInfoEnable(bool b);
	//检测穿透陆地
	bool m_bFallNow;
	Vec2 m_startFallPos;


public:
	//模式
	void super(int y);//超跑
	void willStartSuperPlay(int status=0); // 播放进入超跑云下来的过度动画，status=0表明真的是准备进入超跑，=1表示战斗结束模拟进入超跑，但状态还是fightmode
	void startSuperPlay();
	void startCommonPlay();
	void startFightPlay();

	//状态
	bool isNotInjured(); //是否为不可受伤状态
	void changeState(HeroState state);
	void setWillChangeToState(HeroState state);
	HeroState getCurState(){return m_curState;};
	HeroState getPreState(){return m_previouState;};
	void setCurState(HeroState state){ setPreState(m_curState); m_curState = state;};
	void setPreState(HeroState state){m_previouState = m_curState;}
protected: 
	HeroState m_curState;							//-1(下翻状态) 0(跑) 1 2 3 
	int m_willChangetoState;
	HeroState m_previouState;							//-1(下翻状态) 0(跑) 1 2 3 
	bool m_bWillSuperRun;
	bool m_bArrivalTerminal; //是否到达终点了
//	bool m_bWillBeChongCi;
public:
// 	void setWillBeChongCi(bool e){m_bWillBeChongCi = e;this->setIsShowCaiHongQiao(e);};
// 	bool getWillBeChongCi(){return m_bWillBeChongCi;};
	bool getWillSuperRun(){return m_bWillSuperRun;}
	void setWillSuperRun(bool b);

	//控制彩虹桥显示
 	CC_SYNTHESIZE(bool, m_bIsShowCaiHongQiao, IsShowCaiHongQiao);
// 	//设置电闸是否开启，开启则能通过，否则碰撞即死
// 	CC_SYNTHESIZE(bool, m_bDianZhaOpened, DianZhaOpened);
	//设置是否为中毒状态
	void addZhongDuState(string &attr, float value);
	void removeZhongDuState();
	void removeDianZha();
	//部分数值
protected:
	float m_fDefaultFightPower;
	int	  m_iHp;
	int	  m_iSiWangChongCiTimes;
public:
	float getFightPower();//总的战斗力（包含宠物 魔法物加成）
	bool changeHp(HP_ChangType changType, int iNum);
	int getCurHP(){return m_iHp;};
	void setSiWangChongCiTimes(int times){m_iSiWangChongCiTimes = times;};
	int getSiWangChongCiTimes(){return m_iSiWangChongCiTimes;};





	//碰撞 与 属性
protected:
	bool m_bTreadLandRoll; //高空落地要播放翻滚动画
	void treadLand(UnitSprite* pSprite);
	void addScore(int score, UnitSprite* pSprite); //加分逻辑  也会是减分
	void displayAddScoreLabel(int score);//加分浮现字体动画  也有可能是减分哦
public:
	void ObstuctHero();
	void obstructTheAdvance();  //阻挡前进
	CC_SYNTHESIZE(UnitSprite*, m_pCurObstructSprite, CurObstuctSprite);//记录当前阻挡的物品
// 	UnitSprite*	getCurObstuctSprite(){return m_pCurObstructSprite;}
// 	void setCurObstuctSprite(UnitSprite* pSpr){m_pCurObstructSprite = pSpr;}
// 	UnitSprite*				m_pCurObstructSprite;		
protected:
	void invincible(string attr, float fTime);			//进入无敌状态
	void protect(float fTime);				//进入护盾状态
	void magnet(string attr, float fTime);				//进入磁铁状态
	void changeGold(float fTime);			//进入点金状态
	void speedUp(string attr, float fValue);
	void speedDown(string attr, float fTime);	//进入减少状态 type(1距离加速、2时间加速)
	void rush(string attr, float fValue);	//进入加速状态 type(1距离加速、2时间加速)  (冲刺 自带 磁铁效果)
	void BounceFly(float height, Vec2 startWorldPos, UnitSprite* pObsSprite=nullptr);         //踩踏 反弹动画
	void topBump(); //顶撞反弹
public:
	const vector<Attribute>* getGlobleAddAttribute() { return &m_globleAddAttribute; }
	void startAttribute(Attribute attr);
	void endAttribute(const string& attr);
	void deathRush(string attr,float fValue); //死亡冲刺
	void treadBounce(float height, UnitSprite* pObsSprite=nullptr);         //踩踏 反弹动画
	
	bool isHaveAttrOnGoing(string attr); //当前是否拥有某属性
protected:
	void updateAttribute(float dt);

	vector<Attribute> m_globleAddAttribute; //全局附加属性（宠物、魔法物附加的属性、包含人物自己的属性）：整场游戏都有的加成属性
	map<string,HeroAttribute>	m_lHeroAttributeList;
	int							m_iZhongDuDaoJiShi;
	bool						m_bIsDieRockets;
	CC_SYNTHESIZE(bool, m_bIsSpringBounce, IsSpringBounce);  //是否是弹簧





	// 特效
public:
	void willShock();
	void skill_screenShake();
	void showChongJiBoAnimation(bool bBig = true);
	void showDeadLayer();
	void showDieDlg();

	void chongCiUp(UnitSprite* pObstacleSprite);
	void chongCiFall(UnitSprite* pObstacleSprite);
	void chongCiUpDone();
	void chongCiFallDone();
	void showChongCiAnimation();
	void removeChongCiAnimation();
	void updateChongCiAnimation(float dt);

	void ShowInvincibleAni();

	void showTreadBounceAnimation(float dt);

	void showZhongDuAnimaiton(bool e);
	void showZhongDuDaoJiShi(bool e);
	void hiedeCaiHongQiao(float dt);

protected:
	void movementCallback(Node* pNode);
	void playAnimationEnd(Armature *armature, MovementEventType type, const std::string& movementID);






	//debug draw
public:
	CustomCommand m_customCommand;
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDrawDebugCollisonRect(const Mat4 &transform, uint32_t flags);

protected:
	Unit m_pet;
	Unit m_magicGoods;
	vector<Unit> m_useItems;

	Vec2 m_startJumpPos;


	vector<EventListenerCustom*>				m_lListenerList;
};