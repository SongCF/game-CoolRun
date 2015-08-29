//============================================================
//* 游戏场景内（一共五层结构）
//* 第五层 ：玩家输入层（触摸屏事件）
//* 功能 ：玩家输入（滑动、点击按钮）之后的逻辑响应
//============================================================


#pragma once

#include "cocos2d.h"
#include "BaseSprite.h"

using namespace cocos2d;


class GameInTouchLayer : public Layer
{

public:
	enum _iTag
	{
		jump_btn_tag = 1,
		down_btn_tag,
		up_btn_tag,
	};

	enum Btn_State
	{
		State_Normal,
		State_Select,
		State_Disable,
	};

	enum Btn_Type
	{
		Type_JumpDown,		//下跳
		Type_JumpUp,		//正跑，上跳
	};

	enum Btn_Pos_Type
	{
		Btn_Pos_None,
		Btn_Pos_Left,
		Btn_Pos_Right,
	};

public:
	GameInTouchLayer();
	~GameInTouchLayer();

	virtual bool init();


	void setRightBtnType(Btn_Type btntype);

	CREATE_FUNC(GameInTouchLayer);

public:
	void initUI();

	void StartSuper(bool showHand=true);
	void startCommon();

	void hideMenu();
	void teachJump();
	void teachDoubleJump();
	void teachDown();
	void teachUp();

	void showSuperTouch(Node* pParent);

	void setCanBeTouched(bool e){m_bCanTouched = e;};
	bool getCanBeTouched(){return m_bCanTouched;};

	inline void setIsTouchingLeft(bool e){m_bIsTouchingLeft = e;};
	inline bool getIsTouchingLeft(){return m_bIsTouchingLeft;};

	void removecheckIsTouchingLeft();
private:
	void setBtnState(Btn_Pos_Type posType, Btn_State state);
	void showHand(Node* pParent);
	void checkIsTouchingLeft(float dt);
private:
	BaseSprite*						m_pJump_Btn;
	BaseSprite*						m_pDown_Btn;
//	BaseSprite*						m_pUp_Btn;
	bool							m_bIsSuper;
//	bool							m_bIsTeachUpDone;

	bool							m_bCanTouched;
	bool							m_bIsTouchingLeft;
	Btn_Pos_Type					m_curTouchBtnPosType;
};