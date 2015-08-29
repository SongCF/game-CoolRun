
#include "GameInTouchLayer.h"
#include "MsgDefine.h"
#include "RunningHelper.h"
#include "StringsMap.h"
#include "HeroSprite.h"
#include "Dialog.h"

GameInTouchLayer::GameInTouchLayer()
	: m_pJump_Btn(nullptr)
	, m_pDown_Btn(nullptr)
	, m_bIsSuper(false)
	, m_bCanTouched(true)
	, m_curTouchBtnPosType(Btn_Pos_None)
//	, m_bIsTeachUpDone(false)
	, m_bIsTouchingLeft(false)
{
	
}

GameInTouchLayer::~GameInTouchLayer()
{

}

bool GameInTouchLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initUI();

	return true;
}

void GameInTouchLayer::initUI()
{
	m_pJump_Btn = BaseSprite::create("UI_youxizhong_tiaoyue_select.png");
	this->addChild(m_pJump_Btn);
//	m_pJump_Btn->setScale(2.0f);
	m_pJump_Btn->setOpacity(125);
	m_pJump_Btn->setPosition(Vec2(this->getContentSize().width - m_pJump_Btn->getContentSize().width/2*m_pJump_Btn->getScaleX() - 10, 
		 		m_pJump_Btn->getContentSize().height/2*m_pJump_Btn->getScaleY()+10));

	m_pDown_Btn = BaseSprite::create("UI_youxizhong_cehua_select.png");
	m_pDown_Btn->setOpacity(125);
	this->addChild(m_pDown_Btn);
	m_pDown_Btn->setScale(m_pJump_Btn->getScale());
	m_pDown_Btn->setPosition(Vec2(m_pDown_Btn->getContentSize().width/2*m_pJump_Btn->getScaleX() + 10, 
		 		m_pJump_Btn->getPositionY()));
	
	if (RunningHelper::getHelper()->isTeachScene())
	{
		m_pDown_Btn->setVisible(false);
		m_pJump_Btn->setVisible(false);
		m_pJump_Btn->setOpacity(255);
		m_pDown_Btn->setOpacity(255);
	}
	
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [&](Touch* touch, Event* event)
	{
//		CCLOG("Touch Begin");
		Vec2 tuochPoint = touch->getLocation();
		if (!this->isVisible() || !this->getBoundingBox().containsPoint(tuochPoint) || !m_bCanTouched)
		{
			return false;
		}

		if (!m_bIsSuper)
		{
			if (tuochPoint.x < this->getContentSize().width/2)
			{
				if (!m_pDown_Btn->isVisible())
				{
					return false;
				}
				if (RunningHelper::getHelper()->isTeachScene())
				{
					if (!m_pDown_Btn->getBoundingBox().containsPoint(tuochPoint))
					{
						return false;
					}
					RunningHelper::getHelper()->gameResume();
					m_pDown_Btn->removeAllChildren();
					this->hideMenu();
					Director::getInstance()->getRunningScene()->removeChildByName(DIALOG_NAME);
				}

				this->setIsTouchingLeft(true);
				m_curTouchBtnPosType = Btn_Pos_Left;
				this->setBtnState(Btn_Pos_Left, State_Select);

				this->unschedule(schedule_selector(GameInTouchLayer::checkIsTouchingLeft));
				this->schedule(schedule_selector(GameInTouchLayer::checkIsTouchingLeft), 0.01f);

				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom Customevent(CustomButton_Down);
				eventDspc->dispatchEvent(&Customevent);
//				CCLOG("Dispatch Down");
			}
			else
			{

				if (!m_pJump_Btn->isVisible() || !m_bCanTouched)
				{
					return false;
				}

				if (RunningHelper::getHelper()->isTeachScene())
				{
					if (!m_pJump_Btn->getBoundingBox().containsPoint(tuochPoint))
					{
						return false;
					}
					RunningHelper::getHelper()->gameResume();
					m_pJump_Btn->removeAllChildren();
					this->hideMenu();
					Director::getInstance()->getRunningScene()->removeChildByName(DIALOG_NAME);
				}
				
				m_curTouchBtnPosType = Btn_Pos_Right;
				this->setBtnState(Btn_Pos_Right, State_Select);
				this->removecheckIsTouchingLeft();

				EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
				EventCustom Customevent(CustomButton_Jump);
				eventDspc->dispatchEvent(&Customevent);
//				CCLOG("Dispatch Jump");
			}
		}
		
		return true;
	};

	listener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (!this->isVisible() || !this->getBoundingBox().containsPoint(touch->getLocation()) || !m_bCanTouched)
		{
			return;
		}

		if (!m_bIsSuper)
		{
			return;
		}

		int y = touch->getLocation().y - touch->getPreviousLocation().y;
		EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		EventCustom Customevent(CustomButton_Super);
		Customevent.setUserData(&y);
		eventDspc->dispatchEvent(&Customevent);
	};

	listener->onTouchCancelled = [&](Touch* touch, Event* event)
	{
		listener->onTouchEnded(touch, event);
	};

	listener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		if (!this->isVisible() || !this->getBoundingBox().containsPoint(touch->getLocation()) || !m_bCanTouched)
		{
			return;
		}
		if (!m_bIsSuper)
		{
			if (m_curTouchBtnPosType == Btn_Pos_Left)
			{
				this->removecheckIsTouchingLeft();
				if (!RunningHelper::getHelper()->isTeachScene())
				{
					RunningHelper::getHelper()->getRunnerHero()->resetRun();
				}
				this->setBtnState(Btn_Pos_Left, State_Normal);
			}
			else if (m_curTouchBtnPosType == Btn_Pos_Right)
			{
				this->setBtnState(Btn_Pos_Right, State_Normal);
			}
		}
		
		m_curTouchBtnPosType = Btn_Pos_None;
	};
	listener->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameInTouchLayer::checkIsTouchingLeft(float dt)
{
	if (HeroSprite* pHero = RunningHelper::getHelper()->getRunnerHero())
	{
		if (pHero->getCurState() == HeroSprite::state_Run)
		{
			pHero->DownGlide();
		}
	}
}

void GameInTouchLayer::removecheckIsTouchingLeft()
{
	this->setIsTouchingLeft(false);
	this->unschedule(schedule_selector(GameInTouchLayer::checkIsTouchingLeft));
}

void GameInTouchLayer::StartSuper(bool showHand)
{
	m_bIsSuper = true;
	m_pDown_Btn->setVisible(false);
	m_pJump_Btn->setVisible(false);
	this->setBtnState(Btn_Pos_Left, State_Normal);
	this->setBtnState(Btn_Pos_Right, State_Normal);
	this->removecheckIsTouchingLeft();
	if (showHand)
	{
		showSuperTouch((Node *)RunningHelper::getHelper()->getGameUnitLayer());
	}
	
}
void GameInTouchLayer::startCommon()
{
	m_bIsSuper = false;
	m_pDown_Btn->setVisible(true);
	m_pJump_Btn->setVisible(true);
}

void GameInTouchLayer::setBtnState(Btn_Pos_Type posType, Btn_State state)
{
	if (m_bIsSuper)
	{
		return;
	}
	if (posType == Btn_Pos_Left)
	{
		if (state == State_Normal)
		{
//			m_pDown_Btn->setTexture("ui_sence_anniu_xiafan.png");
			//m_pDown_Btn->setOpacity(125);
			//m_pDown_Btn->setScale(1.0f);
			if (RunningHelper::getHelper()->isTeachScene())
			{
				m_pDown_Btn->setOpacity(255);
			}
			else
			{
				m_pDown_Btn->setOpacity(125);
			}
		}
		else if (state == State_Select)
		{
//			m_pDown_Btn->setTexture("ui_sence_anniu_xiafan_select.png");
			//m_pDown_Btn->setOpacity(255);
			//m_pDown_Btn->setScale(0.9f);
			if (RunningHelper::getHelper()->isTeachScene())
			{
				m_pDown_Btn->setOpacity(125);
			}
			else
			{
				m_pDown_Btn->setOpacity(225);
			}
		}
		else if (state == State_Disable)
		{
//			m_pDown_Btn->setTexture("");
			m_pDown_Btn->setOpacity(0);
			//m_pDown_Btn->setScale(1.0f);
		}
	}
	else if (posType == Btn_Pos_Right)
	{
		{
			if (state == State_Normal)
			{
//				m_pJump_Btn->setTexture("ui_sence_anniu_tiao.png");
				//m_pJump_Btn->setOpacity(125);
				//m_pJump_Btn->setScale(1.0f);
				if (RunningHelper::getHelper()->isTeachScene())
				{
					m_pJump_Btn->setOpacity(255);
				}
				else
				{
					m_pJump_Btn->setOpacity(125);
				}
			}
			else if (state == State_Select)
			{
//				m_pJump_Btn->setTexture("ui_sence_anniu_tiao_select.png");
				//m_pJump_Btn->setOpacity(255);
				//m_pJump_Btn->setScale(0.9f);
				if (RunningHelper::getHelper()->isTeachScene())
				{
					m_pJump_Btn->setOpacity(125);
				}
				else
				{
					m_pJump_Btn->setOpacity(225);
				}
			}
			else if (state == State_Disable)
			{
//				m_pJump_Btn->setTexture("");
				m_pJump_Btn->setOpacity(0);
				//m_pJump_Btn->setScale(1.0f);
			}
		}
	}
}

void GameInTouchLayer::setRightBtnType(Btn_Type btntype)
{
	if (m_bIsSuper || RunningHelper::getHelper()->isTeachScene())
	{
		return;
	}
	if (btntype == Type_JumpUp)
	{
		m_pJump_Btn->setVisible(true);
		this->setBtnState(Btn_Pos_Right, State_Normal);
	}
}

void GameInTouchLayer::teachJump()
{
	m_pDown_Btn->setVisible(false);
	m_pJump_Btn->setVisible(true);
	showHand(m_pJump_Btn);

	m_pDown_Btn->setGlobalZOrder(100);
	m_pJump_Btn->setGlobalZOrder(100);

	static bool has_jump1 = false;
	if (has_jump1){
		has_jump1 = false;
		GuideDialog::showDialog(FX::StringsMap::getValue("Guide_jump2_1"), FX::StringsMap::getValue("Guide_imply_jump"));
	}
	else{ 
		has_jump1 = true;
		GuideDialog::showDialog(FX::StringsMap::getValue("Guide_jump1"), FX::StringsMap::getValue("Guide_imply_jump"));
	}

// 	Label* pLabel = Label::createWithSystemFont(FX::StringsMap::getValue("teachlayer_yiduantiao01").c_str(), "", 24);
//  	Size size = Director::getInstance()->getWinSize()/2;
//  	pLabel->setPosition(m_pJump_Btn->convertToNodeSpace(Vec2(size.width, size.height+200)));
// 	m_pJump_Btn->addChild(pLabel);
// 	pLabel->setColor(Color3B(0, 0 ,0));
// 
// 	Label* pLabel1 = Label::createWithSystemFont(FX::StringsMap::getValue("teachlayer_yiduantiao02").c_str(), "", 30);
// 	m_pJump_Btn->addChild(pLabel1);
// 	pLabel1->setColor(Color3B(0, 0 ,0));
// 	pLabel1->setPosition(Vec2(pLabel->getPositionX(), pLabel->getPositionY()-50));
}
void GameInTouchLayer::teachDoubleJump()
{
	m_pDown_Btn->setVisible(false);
	m_pJump_Btn->setVisible(true);
	showHand(m_pJump_Btn);

	GuideDialog::showDialog(FX::StringsMap::getValue("Guide_jump2_2"), FX::StringsMap::getValue("Guide_imply_jump"));

// 	Label* pLabel = Label::createWithSystemFont(FX::StringsMap::getValue("teachlayer_erduantiao01").c_str(), "", 24);
// 	Size size = Director::getInstance()->getWinSize()/2;
// 	pLabel->setPosition(m_pJump_Btn->convertToNodeSpace(Vec2(size.width, size.height+200)));
// 	m_pJump_Btn->addChild(pLabel);
// 	pLabel->setColor(Color3B(0, 0 ,0));
// 
// 	Label* pLabel1 = Label::createWithSystemFont(FX::StringsMap::getValue("teachlayer_erduantiao02").c_str(), "", 30);
// 	m_pJump_Btn->addChild(pLabel1);
// 	pLabel1->setColor(Color3B(0, 0 ,0));
// 	pLabel1->setPosition(Vec2(pLabel->getPositionX(), pLabel->getPositionY()-50));
}
void GameInTouchLayer::teachDown()
{
	m_pJump_Btn->setVisible(false);
	m_pDown_Btn->setVisible(true);
	showHand(m_pDown_Btn);

	GuideDialog::showDialog(FX::StringsMap::getValue("Guide_down"), FX::StringsMap::getValue("Guide_imply_dowm"));

// 	Label* pLabel = Label::createWithSystemFont(FX::StringsMap::getValue("teachlayer_xiafan01").c_str(), "", 24);
// 	Size size = Director::getInstance()->getWinSize()/2;
// 	pLabel->setPosition(m_pDown_Btn->convertToNodeSpace(Vec2(size.width, size.height+200)));
// 	m_pDown_Btn->addChild(pLabel);
// 	pLabel->setColor(Color3B(0, 0 ,0));
// 
// 	Label* pLabel1 = Label::createWithSystemFont(FX::StringsMap::getValue("teachlayer_xiafan02").c_str(), "", 30);
// 	m_pDown_Btn->addChild(pLabel1);
// 	pLabel1->setPosition(Vec2(pLabel->getPositionX(), pLabel->getPositionY()-50));
// 	pLabel1->setColor(Color3B(0, 0 ,0));

}
void GameInTouchLayer::teachUp()
{
	m_pDown_Btn->setVisible(false);
	m_pJump_Btn->setVisible(true);
	showHand(m_pJump_Btn);

	GuideDialog::showDialog(FX::StringsMap::getValue("Guide_bounce"), FX::StringsMap::getValue("Guide_imply_jump"));
}

void GameInTouchLayer::showHand(Node* pParent)
{
// 	LayerColor* pLayer = LayerColor::create(Color4B(0, 0, 0, 125));
// 	Size size = Director::getInstance()->getWinSize()/2;
// 	pLayer->setPosition(pParent->convertToNodeSpace(Vec2(size.width, size.height)));
// 	pParent->addChild(pLayer);

	Sprite* pHand = Sprite::create("ui_tubiao_jiaoxue_xiaoshou.png");
	pParent->addChild(pHand);

	pHand->setGlobalZOrder(100);
	pHand->setPosition(Vec2(pParent->getContentSize().width/2, pParent->getContentSize().height+pHand->getContentSize().height/2));
	pHand->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.6f, Vec2(-10, -10)), MoveBy::create(0.6f, Vec2(10, 10)), nullptr)));
}
void GameInTouchLayer::showSuperTouch(Node* pParent)
{
	Sprite *spr = Sprite::create("shouzhi.png");
	Size size = Director::getInstance()->getWinSize();
	spr->setPosition(size.width * 0.75f,size.height / 2);
	pParent->addChild(spr);
	spr->setAnchorPoint(Vec2(0.5f,0.5f));


	spr->runAction(Sequence::create(MoveBy::create(1.0f,Vec2(0,size.height / 4)),MoveBy::create(2.0f,Vec2(0, - size.height / 2)),MoveBy::create(2.0f,Vec2(0, size.height / 2)),Spawn::create(MoveBy::create(2.0f,Vec2(0, - size.height / 2)),FadeOut::create(2.0f),nullptr),RemoveSelf::create(),nullptr));
}

void GameInTouchLayer::hideMenu()
{
	m_pDown_Btn->setVisible(false);
	m_pJump_Btn->setVisible(false);
}