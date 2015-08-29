//==================================
//关卡模式主ui界面
//==================================

#pragma once

#include "cocos2d.h"
#include "TollgateHelper.h"
#include "BaseSprite.h"
#include "BaseLayer.h"
#include <string>

using namespace cocos2d;
using std::string;

#define MSG_TollgateLayer_Close "MSG_TollgateLayer_Close"

class TollgateLayer : public Layer
{
public:
	CREATE_FUNC(TollgateLayer);
	TollgateLayer();
	~TollgateLayer();
	bool init();
	void timeToDrawUI(float dt);
	void refreshSectionProgress();
	void refreshMap(int seasonId);
	void initUI();
	/*获取章节解锁插画
	 * seasonId ：章节 （0-3  0表示开始的第一章插画， 1-3 为各章完成后的插画）
	*/
	string getIllustrationFileName(int seaonId);
	static void setAwardIllustration(){m_bGotNewPicture = true;}

protected:
	void setMenuItemSelectedItem(MenuItemImage* p,string imgfile);
	void onCloseItem(Ref* obj);
	void onAddJewelItem(Ref* obj);
	void onPictureItem(Ref* obj);
	void onLeftItem(Ref* obj);
	void onRightItem(Ref* obj);
	void onChangeDifficult(Ref* obj);
	void onMapItem(Ref* obj);
	void normalOrDiff(MenuItemImage *sectionItem,int sectionId,Vector<string> &tt,int seasonId);
	void shopCallBack();
protected:
	Size m_winSize;
	vector<vector<Vec2> > m_mapItemPos;

	Label* m_JewelNumLabel; //总的宝石数量
	Label* m_difficultLabel; //困难模式文字说明
	MenuItemImage* m_changeLvItem; //难度选择item
	ProgressTimer* m_Progress; //进度、普通关卡每过一关涨1/10， 困难关卡收集的星星比例
	BaseSprite* m_title;
	BaseSprite* m_picture;  //插画

	Vector<BaseSprite*> m_mapIdxIcon; //地图翻页指示
	Vector<BaseSprite*> m_mapList; //地图
	Vector<Menu*> m_mapMenu; // 关卡按钮


	static bool m_bGotNewPicture;
};