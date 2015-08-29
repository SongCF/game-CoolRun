#include "BossSprite.h"
#include "RunningHelper.h"
#include "StateBar.h"
#include "AchievementHelper.h"
#include "HeroSprite.h"
#include "Font.h"
#include "RunningHelper.h"
#include "GameUnitLayer.h"


#define Armature_BOSS_FeiXing			"b_fx"
#define Armature_BOSS_XiuXi				"b_jq"
#define Armature_BOSS_Gongji			"b_sf"
#define Armature_BOSS_BaoFa				"b_bf"
#define Armature_BOSS_XiuXi_ShouShang	"b_jqss"
#define Armature_BOSS_FeiXing_ShouShang	"b_fxss"
#define Armature_BOSS_SiWang			"b_sw"

#define Armature_BOSS_ComeOut			"wushibeijitui"


BossSprite* BossSprite::create(Unit* unit)
{
	BossSprite* pSprite = new BossSprite;
	if (pSprite && pSprite->init(unit)){
		pSprite->autorelease();
	}else {
		CC_SAFE_DELETE(pSprite);
		pSprite = nullptr;
	}
	return pSprite;
}

bool BossSprite::init(Unit* unit)
{
	if (! UnitSprite::init(unit)) return false;

	this->setOpacity(0);
	//1. hp
	m_bossState = BS_Normal;
	m_iDefaultHp = m_fCurHp = m_fPreHp = unit->getAttributeValue(Attr_HP);
	m_controlSprite = nullptr;
	m_pArmature = nullptr;
	m_bWillLeave = false;

	//2. bloodbar
	initBloodBar();

	addArmatureInfo(unit->ID);
	playBossAnimation(Armature_BOSS_FeiXing);

	return true;
}

void BossSprite::changeState(unsigned int num)
{
	if (m_fCurHp <= 0)
		return;

	if (1 == num){
		m_bossState = BS_Normal;
		playBossAnimation(Armature_BOSS_FeiXing);
	}
	else if (2 == num){
		m_bossState = BS_Rest;
		playBossAnimation(Armature_BOSS_XiuXi);
	}
	else if (3 == num){
		m_bossState = BS_Anger;
		playBossAnimation(Armature_BOSS_BaoFa);
	}
	else 
		CCASSERT(false, "error config boss type");

}

std::pair<string, string> BossSprite::addArmatureInfo(unsigned int bossId)
{
	pair<string, string> info;
	float scale = 1.f;

	if (bossId == unitID_Boss1)
	{
		info.first = "bossmao.ExportJson";
		info.second = "bossmao";
		scale = 2.f;
	}
	else if (bossId == unitID_Boss2)
	{
		info.first = "bosslong.ExportJson";
		info.second = "bosslong";
		scale = 1.f;
	}
	else 
	{
		CCASSERT(false, "error branch");
	}

//	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(info.first);

	m_pArmature = Armature::create(info.second);
	m_pArmature->setPositionX(this->getContentSize().width/2);
	this->addChild(m_pArmature);
	m_pArmature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(BossSprite::playAnimationEnd, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_pArmature->setScale(scale);

	return info;
}

void BossSprite::come()
{
	playBossAnimation(Armature_BOSS_FeiXing);
}

bool BossSprite::hit()
{
	if (m_bossState == BS_Normal){
		playBossAnimation(Armature_BOSS_Gongji);
		return true;
	}
	else {
		return false;
	}
}

void BossSprite::beBeaten(float subBlood)
{
	int addScore = 100;
	if (m_fCurHp <= 0) 
		return;
	m_fPreHp = m_fCurHp;
	m_fCurHp -= (int)subBlood;

	if(m_fCurHp < 0)
		m_fCurHp = 0;
	updateBloodBar();

	LabelAtlas* pLabel = LabelAtlas::create("0", "UI_shuzi_04.png", 35, 55, '0');
	pLabel->setScale(0.0f);
	pLabel->setOpacity(0.0f);
	pLabel->runAction(Sequence::create(
		Spawn::create(ScaleTo::create(0.08f,1.0f), FadeIn::create(0.08f), nullptr),
		DelayTime::create(0.2f),
		MoveBy::create(0.192f, Vec2(0, 10)),
		Spawn::create(ScaleTo::create(0.1f, 0.0f), FadeOut::create(0.1f), MoveBy::create(0.1f, Vec2(0, 120)), nullptr),
		CallFunc::create([=](){
			RunningHelper::getHelper()->addScore(addScore, false);
	}),
		RemoveSelf::create(), 
		nullptr));
	pLabel->setPosition(Vec2(this->getParent()->getContentSize().width/2,
		this->getPositionY()));
	this->getParent()->addChild(pLabel, 200);
	pLabel->setString(__String::createWithFormat("%d", addScore)->getCString());

	Sprite* pAdd = Sprite::create("plus.png");
 	pLabel->addChild(pAdd);
 	pAdd->setPosition(Vec2(-pAdd->getContentSize().width, pLabel->getContentSize().height/2));

	//effect
	Armature *ar = Armature::create("effect");
	ar->getAnimation()->setMovementEventCallFunc([=](Armature *armature, MovementEventType movementType, const std::string& movementID){
		if (movementType == COMPLETE && movementID == "e_Bossshouji"){
			ar->removeFromParent();
		}
	});
	this->addChild(ar);
	ar->setPosition(this->getContentSize()/2);
	ar->getAnimation()->play("e_Bossshouji");

	if (m_bossState == BS_Rest)
		playBossAnimation(Armature_BOSS_XiuXi_ShouShang);
	else 
		playBossAnimation(Armature_BOSS_FeiXing_ShouShang);

	if (m_fCurHp <= 0) 
		die();

	CCLOG("current boss hp = %.1f", m_fCurHp);
}

void BossSprite::playBossAnimation(string animateName)
{
// 	if (m_fCurHp <= 0 && animateName != Armature_BOSS_SiWang)
// 	{
// 		return;
// 	}

	m_pArmature->getAnimation()->play(animateName);
}

void BossSprite::playAnimationEnd(Armature *armature, MovementEventType type, const std::string& movementID)
{
	if (type == COMPLETE)
	{
		//die armature
		if (movementID == Armature_BOSS_SiWang){
			return;
		}
		else if ((movementID == Armature_BOSS_XiuXi_ShouShang || movementID == Armature_BOSS_FeiXing_ShouShang) && m_fCurHp <= 0){
			playBossAnimation(Armature_BOSS_SiWang);
			return;
		}

		//
		if (m_bossState == BS_Anger)
		{
			if (Armature_BOSS_BaoFa == movementID){
				//爆发完了就走了。
				playBossAnimation(Armature_BOSS_FeiXing);
				leave();
			}
			else {
				playBossAnimation(Armature_BOSS_BaoFa);
			}
		}
		else if (m_bossState == BS_Rest)
		{
			m_pArmature->getAnimation()->play(Armature_BOSS_XiuXi);
		}
		else if (m_bossState == BS_Normal)
		{
			m_pArmature->getAnimation()->play(Armature_BOSS_FeiXing);
		}
		else 
		{
			CCASSERT(false, "Error State");
		}
	}
}


void BossSprite::leave()
{
	m_bWillLeave = true;
	Size winSize = Director::getInstance()->getWinSize();
	this->runAction(Sequence::create(
		MoveTo::create(2, Vec2(winSize.width + 200, this->getPositionY())),
		RemoveSelf::create(true),
		nullptr));

	this->removeBloodBar();
}
void BossSprite::die()
{
	//RunningHelper::getHelper()->startCommonPlay();
	RunningHelper::getHelper()->endFightPlay();

	int deadAdd = 500;
//	this->removeAllChildrenWithCleanup(true);
	this->removeBloodBar();
//	RunningHelper::getHelper()->getRunnerHero()->startAttribute(Attribute(Attr_Rush, 1.5f));
 	this->runAction(Sequence::create(
		DelayTime::create(0.25f),
 		CallFunc::create([=](){
			Armature* pComeout = Armature::create("Run_UI_youxizhong_zhandou");
			pComeout->getAnimation()->play(Armature_BOSS_ComeOut);
			pComeout->setPosition(Vec2(this->getParent()->getContentSize().width/2, this->getParent()->getContentSize().height - pComeout->getContentSize().height - 50));
			Director::getInstance()->getRunningScene()->addChild(pComeout, 100);
			pComeout->getAnimation()->setMovementEventCallFunc([pComeout](Armature *armature, MovementEventType movementType, const std::string& movementID){
				if (movementType == COMPLETE || movementType == LOOP_COMPLETE) pComeout->removeFromParent();
			});


			float x = 5;
			float time = 0.015f;
			Director::getInstance()->getRunningScene()->runAction(
				Sequence::create(
					DelayTime::create(0.1f),
					MoveBy::create(time,Vec2(x,0)),
					MoveBy::create(time,Vec2(-x,0)),
					MoveBy::create(time,Vec2(x,0)),
					MoveBy::create(time,Vec2(x,0)),
					MoveBy::create(time,Vec2(-x/2,0)),
					MoveTo::create(0.01f,Vec2(0,0)),

					DelayTime::create(0.2f),

					CallFunc::create([=](){
						Node* pNode = RunningHelper::getHelper()->getGameUnitLayer();
						LabelAtlas* pLabel = LabelAtlas::create("0", "UI_shuzi_04.png", 35, 55, '0');
						pLabel->setScale(0.0f);
						pLabel->setOpacity(0.0f);
						pLabel->runAction(Sequence::create(
							Spawn::create(ScaleTo::create(0.08f,1.0f), FadeIn::create(0.08f), nullptr),
							DelayTime::create(0.2f),
							MoveBy::create(0.192f, Vec2(0, 10)),
							Spawn::create(ScaleTo::create(0.1f, 0.0f), FadeOut::create(0.1f), MoveBy::create(0.1f, Vec2(0, 220)), nullptr),
							CallFunc::create([=](){
								RunningHelper::getHelper()->addScore(deadAdd, false);
						}),
							RemoveSelf::create(), 
							nullptr));
						pLabel->setPosition(Vec2(pNode->getContentSize().width/2,
							pNode->getContentSize().height/2-pLabel->getContentSize().width/2));
						pNode->addChild(pLabel);
						pLabel->setString(__String::createWithFormat("%d", deadAdd)->getCString());
						Sprite* pAdd = Sprite::create("plus.png");
						pLabel->addChild(pAdd);
						pAdd->setPosition(Vec2(-pAdd->getContentSize().width, pLabel->getContentSize().height/2));
				}), 
					nullptr));
 		}),
		DelayTime::create(1.0f),
		RemoveSelf::create(),
 		nullptr));

	Achieve* info = nullptr;
	//检测杀手相关成就
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShaShouI);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShaShouI, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShaShouII);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShaShouII, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShaShouIII);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShaShouIII, 1);
	}
	info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShaShouIV);
	if (info && !info->isOver)
	{
		AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShaShouIV, 1);
	}
}

void BossSprite::initBloodBar()
{
	StateBar* pStateBar = RunningHelper::getHelper()->getCurrentStateBar();
	if (!pStateBar)
	{
		CCASSERT(false, "pStateBar is should not nullptr");
		return;
	}
	pStateBar->showScoreBar(false);

	Sprite *controlBgSpr = Sprite::create("UI_youxizhong_bossxuetiao_bg.png");
	controlBgSpr->setPosition(pStateBar->getContentSize().width / 2 - 30,45);
	pStateBar->addChild(controlBgSpr, 1, Tag_BossControlBgSpr);

// 	Label* pHPString = Label::createWithSystemFont(__String::createWithFormat("%0.0f/%0.0f", m_fCurHp, m_iDefaultHp)->getCString(), "", 26);
// 	controlBgSpr->addChild(pHPString, 3, Tag_BossHPString);
// 	pHPString->setPosition(controlBgSpr->getContentSize() / 2);

	ProgressTimer * hpTimer = ProgressTimer::create(Sprite::create("UI_youxizhong_bossxuetiao.png"));
	hpTimer->setType(ProgressTimer::Type::BAR);
	hpTimer->setMidpoint(Vec2(0, 0.5f));
	hpTimer->setBarChangeRate(Vec2(1,0));
	hpTimer->setPercentage(100.f);
	hpTimer->setAnchorPoint(Vec2(0,0.5f));
	hpTimer->setPosition(77, hpTimer->getContentSize().height+12);
	controlBgSpr->addChild(hpTimer, 1);
	m_controlSprite = hpTimer;

// 	Sprite *xingSpr = Sprite::create("ui_sence_chaojijiangli_jindutiao_star.png");
// 	xingSpr->setPosition(controlBgSpr->getPositionX() - controlBgSpr->getContentSize().width / 2 + 35,45);
// 	pStateBar->addChild(xingSpr,2,Tag_BossXingSpr);
// 	controlBgSpr->setPositionX(controlBgSpr->getPositionX() + 63);
}

void BossSprite::updateBloodBar()
{
	if (m_controlSprite)
	{
// 		if (Label* pHPString = (Label*)m_controlSprite->getParent()->getChildByTag(Tag_BossHPString))
// 		{
// 			pHPString->setString(__String::createWithFormat("%0.0f/%0.0f", m_fCurHp, m_iDefaultHp)->getCString());
// 		}
		m_controlSprite->runAction(ProgressFromTo::create(0.25f, m_fPreHp/m_iDefaultHp*100.f, m_fCurHp/m_iDefaultHp*100.f));
	}
}

void BossSprite::removeBloodBar()
{
	StateBar* pStateBar = RunningHelper::getHelper()->getCurrentStateBar();
	if (!pStateBar)
	{
		CCASSERT(false, "pStateBar is should not nullptr");
		return;
	}
	pStateBar->showScoreBar(true);

	RunningHelper::getHelper()->getCurrentStateBar()->removeChildByTag(Tag_BossControlBgSpr);
	RunningHelper::getHelper()->getCurrentStateBar()->removeChildByTag(Tag_BossXingSpr);
}