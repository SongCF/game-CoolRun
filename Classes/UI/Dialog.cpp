#include "Dialog.h"
#include "extensions/cocos-ext.h"
#include "Font.h"
#include "RunningHelper.h"
using namespace cocos2d::extension;


#define BG_NAME				"BackGround"
#define Touche_Layer		"Touche_Layer"

void MessageDialog::showDialog(string msg, pfuncMessageDialogCallback callback)
{
	MessageDialog* layer = new MessageDialog;
	if (layer && layer->init(msg, callback))
	{
		Director::getInstance()->getRunningScene()->addChild(layer, DIALOG_ZORDER, DIALOG_NAME);
		layer->release();
	}
	else
	{
		CC_SAFE_DELETE(layer);
	}
}
bool MessageDialog::init(string& msg, pfuncMessageDialogCallback callback)
{
	if (! LayerColor::initWithColor(Color4B(0,0,0,60))) 
		return false;

	m_callback = callback;
	Size winSize = Director::getInstance()->getWinSize();

	//touch
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	listener->onTouchEnded = [&](Touch* pTouch, Event* pEvent)
	{
		if (m_callback)
		{
			m_callback();
		}
		this->removeFromParent();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//bg
	Scale9Sprite *pDlgBg = Scale9Sprite::create("UI_tanchukuang_shuoming.png");
	pDlgBg->setContentSize(Size(460, 226));
	this->addChild(pDlgBg, 1, BG_NAME);
	pDlgBg->setPosition(Director::getInstance()->getWinSize()/2);
	//描述
	Label *pMiaoshu = Label::createWithSystemFont(msg.c_str(), Font_TTF_KaTong, 30, Size(winSize.width*0.6f, 0), TextHAlignment::CENTER);
	pDlgBg->setContentSize(pMiaoshu->getContentSize() + Size(100,90));
	pDlgBg->addChild(pMiaoshu, 10);
	pMiaoshu->setColor(Font_Color_BaseColor);
	pMiaoshu->setAnchorPoint(Vec2(0.5f,0.5f));
	pMiaoshu->setPosition(pDlgBg->getContentSize()/2);

	return true;
}







GuideDialog* GuideDialog::showDialog(string text, string imply, string menuString, std::function<void()> callback)
{
	GuideDialog* layer = new GuideDialog;
	if (layer && layer->init(text, imply, menuString, callback))
	{
		Director::getInstance()->getRunningScene()->addChild(layer, DIALOG_ZORDER, DIALOG_NAME);
		layer->release();
	}
	else
	{
		CC_SAFE_DELETE(layer);
	}
	return layer;
}
bool GuideDialog::init(string text, string imply, string menuString, std::function<void()> callback)
{
	if (! LayerColor::initWithColor(Color4B(0,0,0,60))) 
		return false;
	m_bTouchedClose = false;
	m_callback = callback;

	Sprite *pDlgBg = Sprite::create("UI_tanchukuang_jiaoxue.png");
	this->addChild(pDlgBg, 1, BG_NAME);
	pDlgBg->setPosition(Director::getInstance()->getWinSize()/2);

	Sprite* pIcongBg = Sprite::create("UI_jiaose_qiehuanrenwu_bg.png");
	pDlgBg->addChild(pIcongBg);
	pIcongBg->setScale(1.2f);
	pIcongBg->setPosition(Vec2(20, pDlgBg->getContentSize().height - 20));

	Sprite* pIcon = Sprite::create("UI_touxiang_long.png");
//	pIcon->setScale(0.8f);
	pIcongBg->addChild(pIcon);
	pIcon->setPosition(Vec2(pIcongBg->getContentSize().width/2 + 10, pIcongBg->getContentSize().height/2+5));

	//描述
	Label *pMiaoShu = Label::createWithSystemFont(text.c_str(), Font_TTF_KaTong, 30, Size(470, 0), TextHAlignment::LEFT);
	pDlgBg->addChild(pMiaoShu, 10);
	pMiaoShu->setColor(Font_Color_BaseColor);
//	pMiaoShu->setAnchorPoint(Vec2(0.5f,1));
	pMiaoShu->setPosition(pDlgBg->getContentSize().width/2, pDlgBg->getContentSize().height/2 + 20);

	//提示
	Label *pTishi = Label::createWithSystemFont(imply.c_str(), Font_TTF_KaTong, 36, Size(470, 0), TextHAlignment::CENTER);
	pDlgBg->addChild(pTishi, 10);
//	pTishi->setColor(Color3B::RED);
//	pTishi->setAnchorPoint(Vec2(0,0));
	pTishi->setPosition(pDlgBg->getContentSize().width/2, pTishi->getContentSize().height/2 + 40);

	//按钮
	if (! menuString.empty() && callback != nullptr)
	{
		Menu* menu = Menu::create();
		pDlgBg->addChild(menu, 10);
		menu->setPosition(Vec2::ZERO);

		{//swllow item
			Layer *layer = Layer::create();
			pDlgBg->addChild(layer, 1);
			auto touchListener = EventListenerTouchOneByOne::create();
			touchListener->setSwallowTouches(true);
			touchListener->onTouchBegan = [&](Touch* pTouch, Event* pEvent){return true;};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, layer);
		}

		Sprite *qxS1 = Sprite::create("UI_youxizhong_fuhuo_anniu_bg.png");
		Label *resLa1 = Label::createWithSystemFont(menuString.c_str(), Font_TTF_KaTong, 24);
		qxS1->addChild(resLa1);
		resLa1->setPosition(qxS1->getContentSize()/2);
		//
		Sprite *qxS2 = Sprite::create("UI_youxizhong_fuhuo_anniu_bg.png");
		Label *resLa2 = Label::createWithSystemFont(menuString.c_str(), Font_TTF_KaTong, 30);
		qxS2->addChild(resLa2);
		resLa2->setPosition(qxS2->getContentSize()/2);
		float sX=1, sY=1;
		sX = (resLa1->getContentSize().width+20)/qxS1->getContentSize().width;
		sY = (resLa1->getContentSize().height+20)/qxS1->getContentSize().height;
		qxS1->setScale(sX,sY);
		qxS2->setScale(sX,sY);
		resLa1->setScaleX(1/sX);resLa1->setScaleY(1/sY);
		resLa2->setScaleX(1/sX);resLa2->setScaleY(1/sY);
		//
		MenuItemSprite *item = MenuItemSprite::create(qxS1, qxS2, [&](Ref *pSender){
			m_callback();
		});
		item->setScale(1.5f);
		menu->addChild(item, 1);
		item->setPosition(pDlgBg->getContentSize().width/2 + 20, qxS1->getContentSize().height/2+30);
	}


	return true;
}

void GuideDialog::setBossGudie(bool e)
{
	m_bTouchedClose = e;
	if (e)
	{
		this->removeChildByName(Touche_Layer);
		{//swllow item
			Layer *layer = Layer::create();
			this->getChildByName(BG_NAME)->addChild(layer, 1, Touche_Layer);
			auto touchListener = EventListenerTouchOneByOne::create();
			touchListener->setSwallowTouches(true);
			touchListener->onTouchBegan = [&](Touch* pTouch, Event* pEvent){ 
				if (m_bTouchedClose)
				{
					if (RunningHelper::getHelper()->getCurrentScenePlayMode() == mode_Fight)
						//&& !this->getChildByName(BG_NAME)->getBoundingBox().containsPoint(pTouch->getLocation()))
					{
						this->removeFromParent();
						RunningHelper::getHelper()->gameResume();
					}
				}
				return true;
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, layer);
		}
	}
	else
	{
		this->removeChildByName(Touche_Layer);
	}
}


//对话框
SelectDialog::SelectDialog()
{
}

SelectDialog::~SelectDialog()
{
}

SelectDialog* SelectDialog::create(string title, string context, string iconName)
{
	SelectDialog* pMessageDialog = new SelectDialog();
	
	if (pMessageDialog && pMessageDialog->initWithInfo(title, context, iconName))
	{
		pMessageDialog->autorelease();
		return pMessageDialog;
	}
	CC_SAFE_DELETE(pMessageDialog);
	return pMessageDialog;

}

bool SelectDialog::initWithInfo(const string& title, const string& context, const string& iconName)
{
	if (!Layer::init())
	{
		return false;
	}

	LayerColor* pLayerColor = LayerColor::create(Color4B(0, 0, 0, 125));
	this->addChild(pLayerColor, -1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	m_DialogType = DiglogType_TwoBtn;

	m_pBg = Sprite::create("UI_tanchukuang.png");
	this->addChild(m_pBg, 1, iTag_Bg);
	m_pBg->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2 + 50));

	m_pCloseBtn = MenuItemImage::create(
		"UI_jiaose_anniu_guanbi.png", 
		"UI_jiaose_anniu_guanbi.png",
		std::bind(&SelectDialog::close, this, std::placeholders::_1));
	Menu* pMenu = Menu::create(m_pCloseBtn, nullptr);
	pMenu->setPosition(Vec2(0, 0));
	m_pBg->addChild(pMenu, 2);

	m_pCloseBtn->setPosition(Vec2(m_pBg->getContentSize().width - 20, m_pBg->getContentSize().height - 20));
	int Yoffset = 30;

	m_pTitle = Label::createWithSystemFont(title, Font_TTF_KaTong, 40);
	m_pTitle->setColor(Font_Color_BaseColor);
	m_pBg->addChild(m_pTitle, 1, iTag_Title_Str);
	m_pTitle->setPosition(Vec2(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height - m_pTitle->getContentSize().height/2 - Yoffset));

	m_pBg_Info_Str = Label::createWithSystemFont(context, Font_TTF_KaTong, 26, Size(420, 90), TextHAlignment::LEFT);
	m_pBg->addChild(m_pBg_Info_Str, 1, iTag_Bg_Str);
	m_pBg_Info_Str->setPosition(Vec2(m_pBg->getContentSize().width/2, m_pBg->getContentSize().height/2-Yoffset));

	m_pBg_Icon = nullptr;
	if (iconName.size() > 0)
	{
		m_pBg_Icon = Sprite::create(iconName.c_str());
		Sprite* pBg = Sprite::create("UI_jiaose_qiehuanrenwu_bg.png");
		m_pBg_Icon->addChild(pBg, -1);
		pBg->setPosition(m_pBg_Icon->getContentSize()/2);
	}
	else
	{
		m_pBg_Icon = Sprite::create();
	}

	if (m_pBg_Icon)
	{
		m_pBg->addChild(m_pBg_Icon, 1, iTag_Bg_Icon);
		
		m_pBg_Icon->setPosition(Vec2(m_pBg_Icon->getContentSize().width/2 + 20, m_pBg->getContentSize().height/2 - Yoffset));
		if (iconName.size() > 0)
		{
			m_pBg_Info_Str->setPosition(Vec2(m_pBg_Icon->getPositionX()+m_pBg_Icon->getContentSize().width/2 + m_pBg_Info_Str->getContentSize().width/2, m_pBg_Icon->getPositionY()));
		}
		else
		{
			m_pBg_Info_Str->setPosition(Vec2(m_pBg->getContentSize().width/2, m_pBg_Icon->getPositionY()));
		}
	}

	int xOffset = 80;

	m_pLeftBtn = createBtn("OK", "", std::bind(&SelectDialog::close, this, std::placeholders::_1));
	pMenu->addChild(m_pLeftBtn, 1);
	m_pLeftBtn->setPosition(Vec2(m_pBg->getContentSize().width/2 - m_pLeftBtn->getContentSize().width/2 - xOffset, 
		m_pBg->getPositionY() - m_pBg->getContentSize().height - m_pLeftBtn->getContentSize().height - 100));

	m_pRightBtn = createBtn("Cancel", "", std::bind(&SelectDialog::close, this, std::placeholders::_1));
	pMenu->addChild(m_pRightBtn, 1);
	m_pRightBtn->setPosition(Vec2(m_pBg->getContentSize().width/2 + m_pRightBtn->getContentSize().width/2 + xOffset, 
		m_pLeftBtn->getPositionY()));

	m_pMiddleBtn = createBtn("OK", "", std::bind(&SelectDialog::close, this, std::placeholders::_1));
	pMenu->addChild(m_pMiddleBtn, 1);
	m_pMiddleBtn->setPosition(Vec2(m_pBg->getContentSize().width/2, 
		m_pLeftBtn->getPositionY()));

	setDialogType(m_DialogType);

	return true;
}

MenuItemImage* SelectDialog::createBtn(string str, string iconName, const ccMenuCallback& callback)
{
	MenuItemImage* pBtn = MenuItemImage::create("UI_youxizhong_fuhuo_anniu_bg.png", "UI_youxizhong_fuhuo_anniu_bg.png", callback);

	Label* pInfo_Str = Label::createWithSystemFont(str, Font_TTF_KaTong, 30);
	pBtn->addChild(pInfo_Str, 1, iTag_Btn_Str);
	pInfo_Str->setPosition(pBtn->getContentSize()/2);
	Sprite* pIcon = nullptr;
	if (iconName.size() > 0)
	{
		pIcon = Sprite::create(iconName.c_str());
	}
	else
	{
		pIcon = Sprite::create();
	}
	if (pIcon)
	{
		pBtn->addChild(pIcon, 1, iTag_Btn_Icon);
		pInfo_Str->setPosition(Vec2(pBtn->getContentSize().width/2 - pIcon->getContentSize().width/2, pBtn->getContentSize().height/2));
		pIcon->setPosition(Vec2(pInfo_Str->getPositionX()+pIcon->getContentSize().width/2+pInfo_Str->getContentSize().width/2, pInfo_Str->getPositionY()));
	}

	return pBtn;
}

void SelectDialog::close(Ref* pObj)
{
	this->removeFromParent();
}

void SelectDialog::setCloseCallBack(const ccMenuCallback& callback)
{
	if (callback)
	{
		m_pCloseBtn->setCallback(callback);
	}
}

void SelectDialog::setLeftBtnCallBack(const ccMenuCallback& callback)
{
	if (callback)
	{
		m_pLeftBtn->setCallback(callback);
	}
}

void SelectDialog::setMiddleBtnCallBack(const ccMenuCallback& callback)
{
	if (callback)
	{
		m_pMiddleBtn->setCallback(callback);
	}
}

void SelectDialog::setRightBtnCallBack(const ccMenuCallback& callback)
{
	if (callback)
	{
		m_pRightBtn->setCallback(callback);
	}
}

void SelectDialog::setBtnInfo(string str, string iconName, const ccMenuCallback& callback, BtnType btntype)
{
	MenuItemImage* pBtn = nullptr;

	if (btntype == Type_Left)
	{
		pBtn = m_pLeftBtn;
		setLeftBtnCallBack(callback);
	}
	else if (btntype == Type_Middle)
	{
		pBtn = m_pMiddleBtn;
		setMiddleBtnCallBack(callback);
	}
	else if (btntype == Type_Right)
	{
		pBtn = m_pRightBtn;
		setRightBtnCallBack(callback);	
	}
	if (!pBtn)
	{
		return;
	}

	Label* pStr = (Label*)pBtn->getChildByTag(iTag_Btn_Str);
	Sprite* pIcon = (Sprite*)pBtn->getChildByTag(iTag_Btn_Icon);
	pStr->setString(str.c_str());
	if (iconName.size() > 0)
	{
		pIcon->setTexture(iconName.c_str());
		pStr->setPosition(Vec2(pBtn->getContentSize().width/2 - pIcon->getContentSize().width/2, pBtn->getContentSize().height/2));
		pIcon->setPosition(Vec2(pStr->getPositionX()+pIcon->getContentSize().width/2+pStr->getContentSize().width/2, pStr->getPositionY()));
	}
}

void SelectDialog::setDialogType(DigLogType type)
{
	m_DialogType = type;
	if (m_DialogType == DiglogType_OneBtn)
	{
		m_pLeftBtn->setVisible(false);
		m_pRightBtn->setVisible(false);
		m_pMiddleBtn->setVisible(true);
	}
	else if (m_DialogType == DiglogType_TwoBtn)
	{
		m_pLeftBtn->setVisible(true);
		m_pRightBtn->setVisible(true);
		m_pMiddleBtn->setVisible(false);
	}
}

void SelectDialog::setBgInfo(string context, string iconName)
{
	m_pBg_Info_Str->setString(context.c_str());

	if (iconName.size() > 0)
	{
		m_pBg_Icon->setTexture(iconName.c_str());
		m_pBg_Icon->setPosition(Vec2(m_pBg_Icon->getContentSize().width/2 + 20, m_pBg->getContentSize().height/2 - 25));
		m_pBg_Info_Str->setPosition(Vec2(m_pBg_Icon->getPositionX()+m_pBg_Icon->getContentSize().width/2 + m_pBg_Info_Str->getContentSize().width/2, m_pBg_Icon->getPositionY()));
	}
}

void SelectDialog::setBgInfo(string title, string context, string iconName)
{
	m_pTitle->setString(title);
	setBgInfo(context, iconName);
}