
#include "TalkingLayer.h"
#include "Font.h"

#define TagName_DeadLayer				"DeadLayer"
#define TagName_PauseLayer				"PauseLayer"


TalkingLayer::TalkingLayer()
	: m_iTalkingIndex(-1)
	, m_fFun(nullptr)
	, m_CanBeTouched(true)
{
}

TalkingLayer::~TalkingLayer()
{

}

bool TalkingLayer::init()
{
	m_winSize = Director::getInstance()->getWinSize();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent)
	{
//		if (!m_CanBeTouched)
//		{
//			return false;
//		}
//		m_CanBeTouched = false;
		removeTalkingMessageBox(m_iTalkingIndex);
		if (m_fFun && m_iTalkingIndex+1 >= m_vTalkingStrings.size())
		{
			m_fFun(m_iTalkingIndex);
			this->removeFromParent();			return true;
		}

		createTalkingMessageBox(++m_iTalkingIndex);

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TalkingLayer::setCallBack(const std::function<void(int)>& fun)
{
	m_fFun = fun;
}

void TalkingLayer::setTalkingStrings(const vector<string>& vTalkingStrings)
{
	m_vTalkingStrings = vTalkingStrings;
}

void TalkingLayer::createTalkingMessageBox(int iIndex)
{
	m_iTalkingIndex = iIndex;
	if (m_iTalkingIndex >= m_vTalkingStrings.size())
	{
		CCLOG("%s----->error, index is out of max!!!!!!!!", __FUNCTION__);
		return;
	}
	Sprite* pBox = createTalkingMessageBox(m_vTalkingStrings.at(m_iTalkingIndex));
	pBox->setTag(Tag_Box_Start+iIndex);
}

void TalkingLayer::removeTalkingMessageBox(int iIndex)
{
	if (Node* pNode = this->getChildByTag(Tag_Box_Start+iIndex))
	{
		pNode->removeFromParent();
	}
}

Sprite* TalkingLayer::createTalkingMessageBox(const string& messageStr)
{
	m_CanBeTouched = true;

	Sprite* pBoxBg = Sprite::create("UI_tanchukuang.png");
	pBoxBg->setScale(0.7f);
	this->addChild(pBoxBg);

	Sprite* pNext = Sprite::create("UI_modian_xiayidengji.png");
	pBoxBg->addChild(pNext);
	pNext->setPosition(Vec2(pBoxBg->getContentSize().width - pNext->getContentSize().width, pNext->getContentSize().height/2 + 20));

	Sprite* pIconBg = Sprite::create("UI_jiaose_qiehuanrenwu_bg.png");
	pBoxBg->addChild(pIconBg);
	pIconBg->setScale(2.0f);
	pBoxBg->setPosition(Vec2(pIconBg->getContentSize().width/2*pIconBg->getScale()*pBoxBg->getScale() + pBoxBg->getContentSize().width/2*pBoxBg->getScale() + 50*pBoxBg->getScale(), pBoxBg->getContentSize().height/2));
	Point point = Vec2(-50,  pBoxBg->getContentSize().height/2);

	Sprite* pIcon = Sprite::create("UI_jiaose_touxiang_xiaoxiao.png");
	pBoxBg->addChild(pIcon);
	pIcon->setScale(pIconBg->getScale());

	int nameStartPos = messageStr.find_first_of("]");
	int nameEndPos = messageStr.find_first_of(":");
	
	string posStr = messageStr.substr(0, nameStartPos);
	string name = messageStr.substr(nameStartPos+1, nameEndPos-1);

	string info = messageStr.substr(nameEndPos+1, messageStr.size());
	if (posStr == "2")
	{
		pIcon->setTexture("UI_jiaose_qiehuanrenwu_bg_weikaifang.png");
		pBoxBg->setPositionX(m_winSize.width - pBoxBg->getPositionX());
		point = Vec2(pBoxBg->getContentSize().width - point.x,  pBoxBg->getContentSize().height/2);
	}
	pIconBg->setPosition(point);
	pIcon->setPosition(pIconBg->getPosition());

	int xOffset = 270;

	Label* pName = Label::createWithSystemFont(name, Font_TTF_KaTong, 40, Size(400, 0), TextHAlignment::LEFT);
	pBoxBg->addChild(pName);
	pName->setPosition(Vec2(xOffset, pBoxBg->getContentSize().height - pName->getContentSize().height/2 - 25));

	Label* pInfo = Label::createWithSystemFont(info, Font_TTF_KaTong, 30, Size(400, 0), TextHAlignment::LEFT);
	pBoxBg->addChild(pInfo);
	pInfo->setPosition(Vec2(xOffset, pName->getPositionY() - pName->getContentSize().height/2 - pInfo->getContentSize().height/2 - 20));

	return pBoxBg;
}

