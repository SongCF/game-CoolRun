#include "TollgateLayer.h"
#include "Font.h"
#include "GoodsHelper.h"
#include "StringsMap.h"
#include "GameDirector.h"
#include "RunningHelper.h"
#include "BasicLayer.h"
//DEBUG
#define DEBUG_OPEN_ALL_SECTION 0


//tag name
#define NAME_MainBackground "mian_background"
#define NAME_MapProgressIcon "mapProgressIcon"


//记录玩家当前选择的章节，下次进入该界面，就显示该章节
//玩家每次滑到下一章节，玩一个关卡，或者选择新的难度等操作都会改变
static int s_curSeason = -1;
static int s_curSection = -1;
static Section::Difficulty s_curLevel = Section::eNormal;


//用于结算界面跳转到关卡界面时
//设置是否一个章节结束，获得插画
bool TollgateLayer::m_bGotNewPicture = false;

TollgateLayer::TollgateLayer()
	:m_JewelNumLabel(nullptr)
	,m_changeLvItem(nullptr)
	,m_Progress(nullptr)
	,m_title(nullptr)
	,m_picture(nullptr)
{
	m_mapItemPos.clear();
	m_mapItemPos.push_back(vector<Vec2>(10, Vec2::ZERO));
	m_mapItemPos[0][0] = Vec2(100, 272);
	m_mapItemPos[0][1] = Vec2(208, 285);
	m_mapItemPos[0][2] = Vec2(248, 172);
	m_mapItemPos[0][3] = Vec2(166, 92);
	m_mapItemPos[0][4] = Vec2(346, 128);
	m_mapItemPos[0][5] = Vec2(360, 246);
	m_mapItemPos[0][6] = Vec2(482, 154);
	m_mapItemPos[0][7] = Vec2(624, 86);
	m_mapItemPos[0][8] = Vec2(712, 224);
	m_mapItemPos[0][9] = Vec2(584, 280);
	m_mapItemPos.push_back(vector<Vec2>(10, Vec2::ZERO));
	m_mapItemPos[1][0] = Vec2(60, 252);
	m_mapItemPos[1][1] = Vec2(160, 285);
	m_mapItemPos[1][2] = Vec2(248,252);
	m_mapItemPos[1][3] = Vec2(186, 132);
	m_mapItemPos[1][4] = Vec2(346, 108);
	m_mapItemPos[1][5] = Vec2(390, 246);
	m_mapItemPos[1][6] = Vec2(482, 266);
	m_mapItemPos[1][7] = Vec2(564, 190);
	m_mapItemPos[1][8] = Vec2(624, 280);
	m_mapItemPos[1][9] = Vec2(692, 204);
	m_mapItemPos.push_back(vector<Vec2>(10, Vec2::ZERO));
	m_mapItemPos[2][0] = Vec2(80, 252);
	m_mapItemPos[2][1] = Vec2(180, 200);
	m_mapItemPos[2][2] = Vec2(160, 100);
	m_mapItemPos[2][3] = Vec2(310, 130);
	m_mapItemPos[2][4] = Vec2(350, 230);
	m_mapItemPos[2][5] = Vec2(450, 190);
	m_mapItemPos[2][6] = Vec2(490, 110);
	m_mapItemPos[2][7] = Vec2(600, 100);
	m_mapItemPos[2][8] = Vec2(590, 220);
	m_mapItemPos[2][9] = Vec2(680, 280);
}

TollgateLayer::~TollgateLayer()
{
	m_mapIdxIcon.clear();
	m_mapList.clear();
}

bool TollgateLayer::init()
{
	if (!Layer::init()) return false;

	m_winSize = Director::getInstance()->getWinSize();
	if (PlayerInfoHelper::getHelper()->getOneInfo("isFirstInToll")==0)
	{
		PlayerInfoHelper::getHelper()->setOneInfo("isFirstInToll",1);
		initUI();
		Sprite* bigPicture;
		bigPicture = Sprite::create("chahua_00.png");
		LayerColor *pLayer = LayerColor::create(Color4B(0,0,0,160));
		EventListenerTouchOneByOne *touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan = [](Touch*, Event*){return true;};
		touchListener->onTouchEnded = [=](Touch*touch, Event*){
			pLayer->runAction(Sequence::create(
				MoveTo::create(0.5f,Vec2(740,497)),
				ScaleTo::create(0.01f,0.5f),
				FadeIn::create(0.5f),
				CallFunc::create(
				[&](){
					pLayer->removeFromParent();
			}),nullptr));	
				
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pLayer);

		
		pLayer->addChild(bigPicture);
		bigPicture->setPosition(pLayer->getContentSize()/2);
		/*		Menu* menu = Menu::create();
		bigPicture->addChild(menu, 10);
		menu->setPosition(0,0);
		MenuItemImage* closeItem = MenuItemImage::create("UI_jiaose_anniu_guanbi.png", "UI_jiaose_anniu_guanbi.png", 
		[pLayer](Ref* obj){
		pLayer->removeFromParent();
		});
		menu->addChild(closeItem);
		closeItem->setPosition(bigPicture->getContentSize());
		setMenuItemSelectedItem(closeItem, "UI_jiaose_anniu_guanbi.png")*/;

		//边
		float board = 8;
		LayerColor *pTopBar = LayerColor::create(Color4B(255,255,255,255), bigPicture->getContentSize().width, board);
		LayerColor *pDownBar = LayerColor::create(Color4B(255,255,255,255), bigPicture->getContentSize().width, board);
		LayerColor *pLeftBar = LayerColor::create(Color4B(255,255,255,255), board, bigPicture->getContentSize().height);
		LayerColor *pRightBar = LayerColor::create(Color4B(255,255,255,255), board, bigPicture->getContentSize().height);
		bigPicture->addChild(pTopBar, 1);
		pTopBar->setPosition(0, bigPicture->getContentSize().height-board);
		bigPicture->addChild(pDownBar, 1);
		pDownBar->setPosition(0, 0);
		bigPicture->addChild(pLeftBar, 1);
		pLeftBar->setPosition(0, 0);
		bigPicture->addChild(pRightBar, 1);
		pRightBar->setPosition(bigPicture->getContentSize().width-board, 0);

		bigPicture->setScale(MIN(m_winSize.width/1152*0.9, m_winSize.height/640*0.9));
		this->addChild(pLayer, 100);
	} 
	else
	{
		initUI();
	}


	return true;
}
void TollgateLayer::initUI()
{
	// 	if (s_curSeason == -1)
	// 	{
	// 		vector<int> tollgateProgress = TollgateHelper::getHelper()->getProgress(Section::eNormal);
	// 		s_curSeason = tollgateProgress.at(0);
	// 		s_curSection = tollgateProgress.at(1);
	// 		s_curLevel = Section::eNormal;
	// 	}

	//是否获得插画 (在AccountsLayer结算界面中设置是否通关获得插画)
	if (m_bGotNewPicture)
	{
		//获取到刚才跑完了关卡
		int seasonId = RunningHelper::getHelper()->getCurSeasonId();
		int sectionId = RunningHelper::getHelper()->getCurSectionId();
		TollgateHelper::getHelper()->setGotAward(seasonId, sectionId, Section::eNormal);//插画奖励固定在各章最后一关的normal
		//m_bGotNewPicture = false;//重置  (放在弹出之后)
		//直接弹出图片
		//在draw ui完成后，直接调用onPictureItem弹出图片 
	}else
	{
		vector<int> tollgateProgress = TollgateHelper::getHelper()->getProgress(Section::eNormal);
		s_curSeason = tollgateProgress.at(0);
		s_curSection = tollgateProgress.at(1);
		s_curLevel = Section::eNormal;
	}

	Sprite* bg = Sprite::create("UI_guanqia_kuang.png");
	this->addChild(bg, 0, NAME_MainBackground);
	bg->setPosition(m_winSize/2);

	//this->scheduleOnce(schedule_selector(TollgateLayer::timeToDrawUI), 0);
	timeToDrawUI(0);
}

void TollgateLayer::timeToDrawUI(float dt)
{
	Sprite *bg = dynamic_cast<Sprite*>(this->getChildByName(NAME_MainBackground));
	if (!bg) return;
	//EventListenerTouchOneByOne *touchListener12 = EventListenerTouchOneByOne::create();
	//touchListener12->setSwallowTouches(true);
	//touchListener12->onTouchBegan = [](Touch*, Event*){return true;};
	//touchListener12->onTouchEnded = [](Touch*touch, Event*){
	//	CCLOG("Paddle::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	//};
	//Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener12, this);

	Menu* pMenu = Menu::create();
	bg->addChild(pMenu, 10);
	pMenu->setPosition(Vec2::ZERO);
	//close button
	{
		MenuItemImage* closeItem = MenuItemImage::create("UI_jiaose_anniu_guanbi.png", "UI_jiaose_anniu_guanbi.png", 
			std::bind(&TollgateLayer::onCloseItem, this, std::placeholders::_1));
		pMenu->addChild(closeItem);
		closeItem->setPosition(bg->getContentSize()-Size(16, 36));
		setMenuItemSelectedItem(closeItem, "UI_jiaose_anniu_guanbi.png");
	}
	//绿宝石
	{
		Sprite* lbsImg = Sprite::create("UI_guanqia_baoshilan_lvbaoshi.png");
		bg->addChild(lbsImg);
		lbsImg->setPosition(70,520);
		Sprite* lbsNumBg = Sprite::create("UI_guanqia_baoshilan_bg.png");
		bg->addChild(lbsNumBg);
		lbsNumBg->setAnchorPoint(Vec2(0,0.5));
		lbsNumBg->setPosition(lbsImg->getPositionX()+lbsImg->getContentSize().width/2, lbsImg->getPositionY());
		MenuItemImage* addLbs = MenuItemImage::create("UI_zhuye_jinbilan_anniu_jia.png", "UI_zhuye_jinbilan_anniu_jia.png",
			std::bind(&TollgateLayer::onAddJewelItem, this, std::placeholders::_1));
		pMenu->addChild(addLbs);
		addLbs->setPosition(lbsNumBg->getPositionX()+lbsNumBg->getContentSize().width+20, lbsNumBg->getPositionY());
		setMenuItemSelectedItem(addLbs, "UI_zhuye_jinbilan_anniu_jia.png");
		unsigned int jewelNum = GoodsHelper::getHelper()->getJewelNum();
		m_JewelNumLabel = Label::createWithSystemFont(String::createWithFormat("%u",jewelNum)->getCString(), Font_TTF_KaTong, 36);
		m_JewelNumLabel->setColor(Font_Color_Yellow);
		lbsNumBg->addChild(m_JewelNumLabel);
		m_JewelNumLabel->setPosition(lbsNumBg->getContentSize()/2+Size(0,4));
	}
	//进度(普通模式  困难模式)
	{
		Sprite* xxImg = Sprite::create("UI_guanqia_jindutiao_icon_xingxing.png");
		bg->addChild(xxImg);
		xxImg->setPosition(70, 465);
		Sprite* xxJingduBg = Sprite::create("UI_guanqia_jindutiao_bg.png");
		bg->addChild(xxJingduBg);
		xxJingduBg->setAnchorPoint(Vec2(0,0.5f));
		xxJingduBg->setPosition(xxImg->getPositionX()+xxImg->getContentSize().width/2, xxImg->getPositionY());
		m_Progress = ProgressTimer::create(Sprite::create("UI_guanqia_jindutiao.png"));
		xxJingduBg->addChild(m_Progress);
		m_Progress->setAnchorPoint(Vec2(0,0.5f));
		m_Progress->setMidpoint(Vec2(0, 0.5));
		m_Progress->setPosition(5,xxJingduBg->getContentSize().height/2);
		m_Progress->setType(ProgressTimer::Type::BAR);
		m_Progress->setBarChangeRate(Vec2(1,0));
		m_Progress->setPercentage(0);
	}
	//难度选择 button
	{
		Sprite *lvImg = Sprite::create("UI_guanqia_nandu_bg.png");
		bg->addChild(lvImg,1);
		lvImg->setPosition(bg->getContentSize().width-lvImg->getContentSize().width/2, lvImg->getContentSize().height/3);
		m_difficultLabel = Label::createWithSystemFont(FX::StringsMap::getValue("kunnanmoshi"), Font_TTF_KaTong, 36);
		lvImg->addChild(m_difficultLabel);
		m_difficultLabel->setPosition(lvImg->getContentSize()/2);
		m_changeLvItem = MenuItemImage::create("UI_guanqia_anniu_suoding.png", "UI_guanqia_anniu_suoding.png",
			std::bind(&TollgateLayer::onChangeDifficult, this, std::placeholders::_1));
		pMenu->addChild(m_changeLvItem);
		m_changeLvItem->setPosition(lvImg->getContentSize().width/2+lvImg->getPositionX(), lvImg->getPositionY());
		m_changeLvItem->setEnabled(true);
	}
	//tilte
	{
		m_title = BaseSprite::create(String::createWithFormat("UI_guanqia_biaoti_act_%d.png", s_curSeason)->getCString());
		bg->addChild(m_title);
		m_title->setPosition(bg->getContentSize().width/2, 465);
	}
	//picture
	{
		MenuItemImage *insetBg = MenuItemImage::create("UI_guanqia_chahua_bg.png", "UI_guanqia_chahua_bg.png",
			std::bind(&TollgateLayer::onPictureItem, this, std::placeholders::_1));
		pMenu->addChild(insetBg);
		insetBg->setPosition(740, 497);
		m_picture = BaseSprite::create();
		insetBg->addChild(m_picture);
		m_picture->setPosition(insetBg->getContentSize().width/2, 60);
	}
	//left right button
	{
		MenuItemImage *left = MenuItemImage::create("UI_zhuye_fanhui.png", "UI_zhuye_fanhui.png",
			std::bind(&TollgateLayer::onLeftItem, this, std::placeholders::_1));
		pMenu->addChild(left);
		left->setPosition(10, 245);
		setMenuItemSelectedItem(left, "UI_zhuye_fanhui.png");
		left->setScale(0.8f);
		MenuItemImage *right = MenuItemImage::create("UI_zhuye_fanhui.png", "UI_zhuye_fanhui.png",
			std::bind(&TollgateLayer::onRightItem, this, std::placeholders::_1));
		pMenu->addChild(right);
		right->setPosition(bg->getContentSize().width-10, left->getPositionY());
		setMenuItemSelectedItem(right, "UI_zhuye_fanhui.png");
		if ((Sprite*)right->getNormalImage()) ((Sprite*)right->getNormalImage())->setFlippedX(true);
		if ((Sprite*)right->getSelectedImage()) ((Sprite*)right->getSelectedImage())->setFlippedX(true);
		right->setScale(0.8f);
	}
	//map point
	{
		int count = TollgateHelper::getHelper()->getAllTollgate()->size();
		for (int i=0; i<count; ++i)
		{
			BaseSprite* temp = BaseSprite::create("UI_guanqia_fanye.png");
			bg->addChild(temp);
			m_mapIdxIcon.pushBack(temp);
			temp->setPosition(bg->getContentSize().width/2 + (i-(count-1)/2.f)*30, 29);
		}
	}
	//map
	{
		int count = TollgateHelper::getHelper()->getAllTollgate()->size();
		for (int i=0; i<count; ++i)
		{
			BaseSprite* tempMap = BaseSprite::create(String::createWithFormat("UI_guanqia_ditu_act_%d.png", i+1)->getCString());
			bg->addChild(tempMap);
			tempMap->setPosition(bg->getContentSize().width/2, 234);
			tempMap->setVisible(false);
			m_mapList.pushBack(tempMap);
			Menu* tempMenu = Menu::create();
			tempMap->addChild(tempMenu);
			tempMenu->setPosition(0,0);
			m_mapMenu.pushBack(tempMenu);
		}
	}

	refreshSectionProgress();
}

void TollgateLayer::refreshSectionProgress()
{
	Sprite *bg = dynamic_cast<Sprite*>(this->getChildByName(NAME_MainBackground));
	if (!bg) return;
	//season    difficult
	//标题
	{
		if (s_curSeason >= 1 && s_curSeason <= TollgateHelper::getHelper()->getAllTollgate()->size()){
			m_title->initWithFile(String::createWithFormat("UI_guanqia_biaoti_act_%d.png", s_curSeason)->getCString());
		}else{
			CCASSERT(false, "");
		}
	}
	//进度条
	{
		if (s_curLevel == Section::eNormal){
			const Season* pSeason = TollgateHelper::getHelper()->getOneTollgateSeason(s_curSeason);
			float rate = TollgateHelper::getHelper()->getSeasonProgress(s_curSeason, s_curLevel) / (float)pSeason->sectionList.size();
			m_Progress->setPercentage(rate * 100);
		}
		else if(s_curLevel == Section::eDifficult){
			int count = 0;
			const Season* pSeason = TollgateHelper::getHelper()->getOneTollgateSeason(s_curSeason);
			for (int i=0; i<pSeason->sectionList.size(); ++i)
			{//0  1  2  3 
				count += TollgateHelper::getHelper()->getOneSectionPassLevel(s_curSeason, pSeason->sectionList[i].sectionId, Section::eDifficult);
			}
			float rate = count / (float)(2*pSeason->sectionList.size());
			m_Progress->setPercentage(rate * 100);
		}
		else {
			CCASSERT(false, "");
		}
	}
	//插画
	{
		string fileName= "";
		for (int i=0;i<4;i++)
		{
			if (TollgateHelper::getHelper()->isGotAward(i+1, TollgateHelper::getHelper()->getOneTollgateSeason(s_curSeason)->sectionList.size(), Section::eNormal))
			{
				fileName = getIllustrationFileName(i+1);
			}
		}
		if (fileName!="")
		{
			m_picture->initWithFile(fileName);
			m_picture->setScale(0.10417, 0.12656);
			if (m_bGotNewPicture){
				m_bGotNewPicture = false;
				onPictureItem(nullptr);
			}
		}
		else
		{
			m_picture->initWithFile("chahua_00.png");
			m_picture->setScale(0.10417, 0.12656);
		}
	}
	//难度选择
	{
		if (s_curLevel == Section::eNormal){
			m_difficultLabel->setString(FX::StringsMap::getValue("putongmoshi"));
			if (TollgateHelper::getHelper()->isCanStart(s_curSeason, 1, Section::eDifficult)){
				m_changeLvItem->setNormalImage(Sprite::create("UI_guanqia_anniu_qiehuan.png"));
				setMenuItemSelectedItem(m_changeLvItem, "UI_guanqia_anniu_qiehuan.png");
				m_changeLvItem->setEnabled(true);
			}else {
#if DEBUG_OPEN_ALL_SECTION == 1
				m_changeLvItem->setNormalImage(Sprite::create("UI_guanqia_anniu_qiehuan.png"));
				setMenuItemSelectedItem(m_changeLvItem, "UI_guanqia_anniu_qiehuan.png");
				m_changeLvItem->setEnabled(true);
#else
				//锁定的时候 显示困难
				m_difficultLabel->setString(FX::StringsMap::getValue("kunnanmoshi"));
				m_changeLvItem->setNormalImage(Sprite::create("UI_guanqia_anniu_suoding.png"));
				m_changeLvItem->setEnabled(true);
#endif
			}
		}
		else if(s_curLevel == Section::eDifficult){
			m_difficultLabel->setString(FX::StringsMap::getValue("kunnanmoshi"));
			m_changeLvItem->setNormalImage(Sprite::create("UI_guanqia_anniu_qiehuan.png"));
			setMenuItemSelectedItem(m_changeLvItem, "UI_guanqia_anniu_qiehuan.png");
			m_changeLvItem->setEnabled(true);
		}
		else {
			CCASSERT(false, "");
		}
	}
	//地图标示点
	{
		for (int i=0,count=m_mapIdxIcon.size(); i<count; ++i)
		{
			if (i+1 == s_curSeason){
				m_mapIdxIcon.at(i)->initWithFile("UI_guanqia_fanye_select.png");
			}else {
				m_mapIdxIcon.at(i)->initWithFile("UI_guanqia_fanye.png");
			}
		}
	}
	//地图
	{
		for (int i=0,count=m_mapList.size(); i<count; ++i)
		{
			if (i+1 == s_curSeason){
				m_mapList.at(i)->setVisible(true);
				this->refreshMap(s_curSeason);
			}else {
				m_mapList.at(i)->setVisible(false);
			}
		}
	}
}

void TollgateLayer::refreshMap(int seasonId)
{
	if (m_mapList.size() < seasonId || m_mapMenu.size() < seasonId || m_mapItemPos.size() < seasonId){
		CCASSERT(false, "");
		return;
	}

	char buf[32];
	BaseSprite* pMap = m_mapList.at(seasonId-1);
	Menu* pMenu = m_mapMenu.at(seasonId-1);
	pMap->removeChildByName(NAME_MapProgressIcon);
	pMenu->removeAllChildren();

	string normal_1 ;
	string normal_2 ;
	string normal_3 ;
	string suoding_1;
	string suoding_2;
	string suoding_3;
	string tongguan_1;
	string tongguan_2;
	string tongguan_3;
	string headImag = String::createWithFormat("UI_guanqia_boss_touxiang_%d.png", seasonId)->getCString();

	if (s_curLevel == Section::eNormal)
	{
		normal_1 = "UI_normal_jiesuo.png";
		normal_2 = "UI_normal_5_jiesuo.png";
		normal_3 = "UI_normal_10_jiesuo.png";
		suoding_1 =  "UI_normal_suoding.png";
		suoding_2 =  "UI_normal_5_suoding.png";
		suoding_3 =  "UI_normal_10_suoding.png";
		tongguan_1="UI_normal_tongguan.png";
		tongguan_2="UI_normal_5_tongguan.png";
		tongguan_3="UI_normal_10_tongguan.png";
	}else{
		normal_1 = "UI_kunnan_jiesuo.png";
		normal_2 = "UI_kunnan_5_jiesuo.png";
		normal_3 = "UI_kunnan_10_jiesuo.png";
		suoding_1 =  "UI_kunnan_suoding.png";
		suoding_2 =  "UI_kunnan_5_suoding.png";
		suoding_3 =  "UI_kunnan_10_suoding.png";
		tongguan_1="UI_kunnan_tongguan.png";
		tongguan_2="UI_kunnan_5_tongguan.png";
		tongguan_3="UI_kunnan_10_tongguan.png";
	}

	//获取当前章节打过的最后一关
	int lastSeasonId = TollgateHelper::getHelper()->getProgress(s_curLevel).at(0);
	int curSeasonLastSection = TollgateHelper::getHelper()->getSeasonProgress(seasonId, s_curLevel);
	int countStar = 0; //上一章星数
	int isPassSimple = false;
	int lastSection = 0;
	if (seasonId>1)
	{
		lastSection =  TollgateHelper::getHelper()->getSeasonProgress(seasonId-1, s_curLevel);
		const Season* pSeason = TollgateHelper::getHelper()->getOneTollgateSeason(seasonId-1);
		for (int i=0; i<pSeason->sectionList.size(); ++i)
		{//0  1  2  3 
			countStar += TollgateHelper::getHelper()->getOneSectionPassLevel(seasonId-1, pSeason->sectionList[i].sectionId, Section::eDifficult);
		}
		if ( TollgateHelper::getHelper()->getSeasonProgress(seasonId, Section::eNormal) ==10)
		{
			isPassSimple = true;
		}
	}
	for (int i=0,count=TollgateHelper::getHelper()->getOneTollgateSeason(seasonId)->sectionList.size(); i<count; ++i)
	{
		bool isPass = false;
		bool isLock = false;
		int sectionId = i+1;
		MenuItemImage *sectionItem = MenuItemImage::create("","","",std::bind(&TollgateLayer::onMapItem, this, std::placeholders::_1));
		pMenu->addChild(sectionItem, 1, sectionId);
		sectionItem->setPosition(m_mapItemPos[seasonId-1][i]);
		if (s_curLevel == Section::eNormal)
		{
			if (sectionId <= curSeasonLastSection && lastSeasonId >= seasonId)
			{
				isPass = true;
			} 
			else if ((sectionId-1 == curSeasonLastSection && seasonId == lastSeasonId)||((sectionId-1) == 0 && lastSection ==10))
			{
				isLock = true;
			}
		} 
		else
		{
			if (sectionId <= curSeasonLastSection && lastSeasonId >= seasonId)
			{
				isPass = true;
			} 
			else if ((sectionId-1 == curSeasonLastSection && seasonId == lastSeasonId))
			{
				isLock = true;
			}
			else if (((sectionId-1) == 0 && (countStar>=20) && isPassSimple ))
			{
				isLock = true;
			}

		}
		//普通与困难只有打过的不一样
		if (isPass){ //已通关
			Sprite* pNorImg = nullptr;
			Sprite* pSelImg = nullptr;
			if (sectionId != 5 && sectionId != 10)
			{
				pNorImg = Sprite::create(tongguan_1);
				sectionItem->setNormalImage(pNorImg);
				setMenuItemSelectedItem(sectionItem,tongguan_1);
				pSelImg = (Sprite*)sectionItem->getSelectedImage();
			}else if (sectionId == 10)
			{
				pNorImg = Sprite::create(tongguan_3);
				sectionItem->setNormalImage(pNorImg);
				setMenuItemSelectedItem(sectionItem, tongguan_3);
				pSelImg = (Sprite*)sectionItem->getSelectedImage();
				auto bossHead = Sprite::create(headImag);
				bossHead->setScale(0.6);
				bossHead->setPosition(Vec2(sectionItem->getContentSize().width/2,sectionItem->getContentSize().height/2));
				sectionItem->addChild(bossHead);
			}else{
				pNorImg = Sprite::create(tongguan_2);
				sectionItem->setNormalImage(pNorImg);
				setMenuItemSelectedItem(sectionItem, tongguan_2);
				pSelImg = (Sprite*)sectionItem->getSelectedImage();
			}

			//num label
			sprintf(buf, "%d", sectionId);
			{
				LabelAtlas* sectionNumLabel = LabelAtlas::create(buf, "UI_guanqia_shuzi.png", 26, 38, '0');
				sectionNumLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
				sectionNumLabel->setPosition(sectionItem->getContentSize()/2 + Size(-4,12));
				pNorImg->addChild(sectionNumLabel);
			}
			{
				LabelAtlas* sectionNumLabel = LabelAtlas::create(buf, "UI_guanqia_shuzi.png", 26, 38, '0');
				sectionNumLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
				sectionNumLabel->setPosition(sectionItem->getContentSize()/2 + Size(-4,12));
				pSelImg->addChild(sectionNumLabel);
			}
			if (s_curLevel == Section::eNormal)
			{
				TollgateInfor::TollgatePassLevel passLv = TollgateHelper::getHelper()->getOneSectionPassLevel(seasonId, sectionId, s_curLevel);
				{
					Sprite *lvImg = Sprite::create(String::createWithFormat("UI_guanqia_biaoshi_star_%d.png", passLv)->getCString());
					lvImg->setPosition(sectionItem->getContentSize().width/2-4, sectionItem->getContentSize().height+10);
					pNorImg->addChild(lvImg);
				}
				{
					Sprite *lvImg = Sprite::create(String::createWithFormat("UI_guanqia_biaoshi_star_%d.png", passLv)->getCString());
					lvImg->setPosition(sectionItem->getContentSize().width/2-4, sectionItem->getContentSize().height+10);
					pSelImg->addChild(lvImg);
				}
			}
			else if (s_curLevel == Section::eDifficult)
			{
				TollgateInfor::TollgatePassLevel passLv = TollgateHelper::getHelper()->getOneSectionPassLevel(seasonId, sectionId, s_curLevel);
				{
					Sprite *lvImg = Sprite::create(String::createWithFormat("UI_guanqia_kunnan_biaoshi_star_%d.png", passLv)->getCString());
					lvImg->setPosition(sectionItem->getContentSize().width/2-4, sectionItem->getContentSize().height+10);
					pNorImg->addChild(lvImg);
				}
				{
					Sprite *lvImg = Sprite::create(String::createWithFormat("UI_guanqia_kunnan_biaoshi_star_%d.png", passLv)->getCString());
					lvImg->setPosition(sectionItem->getContentSize().width/2-4, sectionItem->getContentSize().height+10);
					pSelImg->addChild(lvImg);
				}
			}
			else {
				CCASSERT(false, "");
			}
		}
		else if (isLock){//已解锁
			if (sectionId != 10 && sectionId!= 5)
			{
				sectionItem->setNormalImage(Sprite::create(normal_1));
				setMenuItemSelectedItem(sectionItem, normal_1);
			}else if (sectionId == 5 )
			{
				sectionItem->setNormalImage(Sprite::create(normal_2));
				setMenuItemSelectedItem(sectionItem, normal_2);
			}else{
				sectionItem->setNormalImage(Sprite::create(normal_3));
				setMenuItemSelectedItem(sectionItem, normal_3);
				auto bossHead = Sprite::create(headImag);
				bossHead->setPosition(Vec2(sectionItem->getContentSize().width/2,sectionItem->getContentSize().height/2));
				sectionItem->addChild(bossHead);
			}
			//sectionItem->setNormalImage(Sprite::create("UI_guanqia_biaoshi_xinjiesuo.png"));
			//setMenuItemSelectedItem(sectionItem, "UI_guanqia_biaoshi_xinjiesuo.png");
			//进度标示
			Sprite* progressImg = Sprite::create("UI_guanqia_dingwei.png");
			pMap->addChild(progressImg, 1, NAME_MapProgressIcon);
			progressImg->setPosition(sectionItem->getPosition() + Vec2(0, 90));
			progressImg->runAction(RepeatForever::create(Sequence::create(
				MoveBy::create(0.5f, Vec2(0, -20)),
				MoveBy::create(0.5f, Vec2(0, 20)),
				nullptr)));
		}
		else { //未解锁

#if DEBUG_OPEN_ALL_SECTION == 1
			if (sectionId != 5 && sectionId != 10)
			{
				sectionItem->setNormalImage(Sprite::create(normal_1));
				setMenuItemSelectedItem(sectionItem,normal_1);
			}else if (sectionId == 10)
			{
				auto nomalImag = Sprite::create(normal_3);
				sectionItem->setNormalImage(nomalImag);
				setMenuItemSelectedItem(sectionItem, normal_3);
				auto bossHead = Sprite::create(headImag);
				bossHead->setScale(0.52);
				bossHead->setPosition(Vec2(sectionItem->getContentSize().width/2,sectionItem->getContentSize().height*0.8));
				sectionItem->addChild(bossHead);

			}else{
				sectionItem->setNormalImage(Sprite::create(normal_2));
				setMenuItemSelectedItem(sectionItem, normal_2);
			}

#else
			//sectionItem->setNormalImage(Sprite::create("UI_guanqia_biaoshi_weijiesuo.png"));
			//setMenuItemSelectedItem(sectionItem, "UI_guanqia_biaoshi_weijiesuo.png");
			if (sectionId != 5 && sectionId != 10)
			{
				sectionItem->setNormalImage(Sprite::create("UI_normal_suoding.png"));
				setMenuItemSelectedItem(sectionItem, "UI_normal_suoding.png");
			}else if (sectionId == 10)
			{
				sectionItem->setNormalImage(Sprite::create("UI_normal_10_suoding.png"));
				setMenuItemSelectedItem(sectionItem, "UI_normal_10_suoding.png");
				auto bossHead = Sprite::create("UI_guanqia_boss_touxiang_1.png");
				bossHead->setScale(0.52);
				bossHead->setPosition(Vec2(sectionItem->getContentSize().width/2,sectionItem->getContentSize().height*0.8));
				sectionItem->addChild(bossHead);
			}else{
				sectionItem->setNormalImage(Sprite::create("UI_normal_5_suoding.png"));
				setMenuItemSelectedItem(sectionItem, "UI_normal_5_suoding.png");
			}
			sectionItem->setEnabled(false);
#endif
		}
	}
}
void TollgateLayer::normalOrDiff(MenuItemImage *sectionItem,int sectionId,Vector<string> &tt,int seasonId)
{
	
}
void TollgateLayer::setMenuItemSelectedItem(MenuItemImage* p,string imgfile)
{
	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);
}

string TollgateLayer::getIllustrationFileName(int seaonId)
{
	char buf[48];
	sprintf(buf, "chahua_%02d.png", seaonId);
	return buf;
}


void TollgateLayer::onCloseItem(Ref* obj)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(MSG_TollgateLayer_Close);
}

void TollgateLayer::onAddJewelItem(Ref* obj)
{
	FX::Music::getInstance()->buttonClicked();
	auto shop=BaseLayer::create();
	shop->goldbox([&](){
		this->setVisible(true);
	});
	Director::getInstance()->getRunningScene()->addChild(shop,2000);
}

void TollgateLayer::shopCallBack()
{
	this->setVisible(true);
}

void TollgateLayer::onPictureItem(Ref* obj){

	//picture size = 1152,640
	//if (!TollgateHelper::getHelper()->isGotAward(s_curSeason, TollgateHelper::getHelper()->getOneTollgateSeason(s_curSeason)->sectionList.size(), 
	//	Section::eNormal))
	//	return;

	LayerColor *pLayer = LayerColor::create(Color4B(0,0,0,160));
	EventListenerTouchOneByOne *touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch*, Event*){return true;};
	touchListener->onTouchEnded = [=](Touch*touch, Event*){
		//pLayer->removeFromParent();
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pLayer);

	Sprite* bgSprite = Sprite::create("UI_shangcheng_bg.png");
	bgSprite->setPosition(Vec2(pLayer->getContentSize().width/2,pLayer->getContentSize().height/2));
	pLayer->addChild(bgSprite);
	int aa[4];
	aa[0] = 3;
	aa[1] = 7;
	aa[2] = 3;
	aa[3] = 7;
	for (int i=0;i<4;i++)
	{
		string s_image = String::createWithFormat("chahua_0%d.png", i)->getCString();
		Sprite* pic = Sprite::create(s_image);
		pic->setScale(0.3);
		pic->setAnchorPoint(Vec2(0.5,0.5));
		bgSprite->addChild(pic);
		if(i==0)
		{
		}
		else if (!TollgateHelper::getHelper()->isGotAward(i, TollgateHelper::getHelper()->getOneTollgateSeason(s_curSeason)->sectionList.size(), 
			Section::eNormal))
		{
			pic->setColor(Color3B(96,96,96));
		}
		
		int py;
		if (i<2)
		{
			py = 6;
		} 
		else
		{
			py=3;
		}
		pic->setPosition(bgSprite->getContentSize().width/10*aa[i],bgSprite->getContentSize().height/8*py);
	}
	
	Sprite* bigPicture = Sprite::create("chahua_00.png");
	pLayer->addChild(bigPicture);
	bigPicture->setVisible(false);

	Menu* menu = Menu::create();
	bgSprite->addChild(menu, 10);
	menu->setPosition(0,0);
	MenuItemImage* closeItem = MenuItemImage::create("UI_jiaose_anniu_guanbi.png", "UI_jiaose_anniu_guanbi.png", 
		[pLayer,this](Ref* obj){
			pLayer->removeFromParent();
	});
	menu->addChild(closeItem);
	closeItem->setPosition(bgSprite->getContentSize().width,bgSprite->getContentSize().height);
	setMenuItemSelectedItem(closeItem, "UI_jiaose_anniu_guanbi.png");

	//边
	float board = 8;
	LayerColor *pTopBar = LayerColor::create(Color4B(255,255,255,255), bigPicture->getContentSize().width, board);
	LayerColor *pDownBar = LayerColor::create(Color4B(255,255,255,255), bigPicture->getContentSize().width, board);
	LayerColor *pLeftBar = LayerColor::create(Color4B(255,255,255,255), board, bigPicture->getContentSize().height);
	LayerColor *pRightBar = LayerColor::create(Color4B(255,255,255,255), board, bigPicture->getContentSize().height);
	bigPicture->addChild(pTopBar, 1);
	pTopBar->setPosition(0, bigPicture->getContentSize().height-board);
	bigPicture->addChild(pDownBar, 1);
	pDownBar->setPosition(0, 0);
	bigPicture->addChild(pLeftBar, 1);
	pLeftBar->setPosition(0, 0);
	bigPicture->addChild(pRightBar, 1);
	pRightBar->setPosition(bigPicture->getContentSize().width-board, 0);

	bigPicture->setScale(MIN(m_winSize.width/1152*0.9, m_winSize.height/640*0.9));
	this->addChild(pLayer, 100);
}

void TollgateLayer::onLeftItem(Ref* obj)
{
	if (s_curSeason <= 1) return;
	s_curSeason -= 1;
	refreshSectionProgress();
}
void TollgateLayer::onRightItem(Ref* obj)
{
	int maxSeasonId = TollgateHelper::getHelper()->getAllTollgate()->size();
	if (s_curSeason >= maxSeasonId) return;
	s_curSeason += 1;
	refreshSectionProgress();
}
void TollgateLayer::onChangeDifficult(Ref* obj)
{
	if (s_curLevel == Section::eDifficult)
	{
		s_curLevel = Section::eNormal;
		refreshSectionProgress();
	}
	else if (s_curLevel == Section::eNormal)
	{
		if (TollgateHelper::getHelper()->isCanStart(s_curSeason, 1, Section::eDifficult)){
			s_curLevel = Section::eDifficult;
			refreshSectionProgress();
		}else {
			auto basic=BasicLayer::create();
			basic->Cannokaifang("tishi","guanqiatishi");
			Director::getInstance()->getRunningScene()->addChild(basic);
//#if DEBUG_OPEN_ALL_SECTION == 1
//			s_curLevel = Section::eDifficult;
//			refreshSectionProgress();
//#else
//
//			CCASSERT(false, "不允许的情况按钮应该不能点击！");
//#endif
		}
	}
	else 
	{
		CCASSERT(false, "");
	}
}

void TollgateLayer::onMapItem(Ref* obj)
{
	int sectionId = ((MenuItem*)obj)->getTag();
	CCLOG("===========================================");
	CCLOG("tollgate section begin : seasonId[%d], sectionId[%d], level[%s]", 
		s_curSeason, sectionId, s_curLevel==Section::eNormal?"normal":"difficult");
	CCLOG("===========================================");
	GameDirector::getDirector()->loadingToRunningScene(RunningMode::tollgate, s_curSeason, sectionId, s_curLevel);
}