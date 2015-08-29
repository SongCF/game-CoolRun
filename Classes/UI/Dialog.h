//============================================================
//* 消息框
//============================================================

#pragma once

#include "cocos2d.h"

#include <string>
using std::string;

USING_NS_CC;

#define DIALOG_ZORDER 3096
#define DIALOG_NAME "_MESSAGE_DIALOG_"

class MessageDialog : public cocos2d::LayerColor
{
public:
typedef std::function<void()> pfuncMessageDialogCallback;
	static void showDialog(string msg, pfuncMessageDialogCallback callback=nullptr);


protected:
	MessageDialog():m_callback(nullptr){}
	bool init(string& msg, pfuncMessageDialogCallback callback);


private:
	pfuncMessageDialogCallback m_callback;

};







class GuideDialog : public cocos2d::LayerColor
{
public:
	static GuideDialog* showDialog(string text, string imply, string menuString="", std::function<void()> callback=nullptr);
			
	void setBossGudie(bool e);
protected:
	GuideDialog(){}
	bool init(string text, string imply, string menuString, std::function<void()> callback);


private:
	std::function<void()>	m_callback;
	bool					m_bTouchedClose;
};






class SelectDialog : public cocos2d::Layer
{
public:
	SelectDialog();
	~SelectDialog();

	static SelectDialog* create(string title, string context, string iconName);

	enum Tag
	{
		iTag_Bg = 1,
		iTag_Title_Str,
		iTag_Bg_Str,
		iTag_Bg_Icon,
		iTag_Btn_Str,
		iTag_Btn_Icon,
	};

	enum DigLogType
	{
		DiglogType_OneBtn = 1,
		DiglogType_TwoBtn,
	};

	enum BtnType
	{
		Type_Left = 0,
		Type_Middle,
		Type_Right,
		Type_Close,
	};

public:
	void setCloseCallBack(const ccMenuCallback& callback);
	void setLeftBtnCallBack(const ccMenuCallback& callback);
	void setRightBtnCallBack(const ccMenuCallback& callback);
	void setMiddleBtnCallBack(const ccMenuCallback& callback);

	void setDialogType(DigLogType type);
	DigLogType getDialogType(){return m_DialogType;};

	void setBtnInfo(string str, string iconName, const ccMenuCallback& callback, BtnType btntype);

	void setBgInfo(string context, string iconName);
	void setBgInfo(string title, string context, string iconName);
	MenuItemImage* getBtn(BtnType type)
	{
		MenuItemImage* pRet = nullptr;
		if (type == Type_Left)
		{
			pRet = m_pLeftBtn;
		}
		else if (type == Type_Middle)
		{
			pRet = m_pMiddleBtn;
		}
		else if (type == Type_Right)
		{
			pRet = m_pRightBtn;
		}
		else if (type == Type_Close)
		{
			pRet = m_pCloseBtn;
		}
		return pRet;
	}

	Label* getTitle()
	{
		return m_pTitle;
	}

	Sprite* getBg()
	{
		return m_pBg;
	}

	Label* getBgInfo()
	{
		return m_pBg_Info_Str;
	}

protected:
	MenuItemImage* createBtn(string str, string iconName, const ccMenuCallback& callback);
	bool initWithInfo(const string& title, const string& context, const string& iconName);
	void close(Ref* pObj);
	
protected:

	Sprite*				m_pBg;
	Sprite*				m_pBg_Icon;
	Label*				m_pBg_Info_Str;
	Label*				m_pTitle;

	MenuItemImage*		m_pLeftBtn;
//	Sprite*				m_pLeftBtn_Icon;
//	Label*				m_pLeftBtn_Info_Str;

 	MenuItemImage*		m_pRightBtn;
// 	Sprite*				m_pRightBtn_Icon;
// 	Label*				m_pRightBtn_Info_Str;

	MenuItemImage*		m_pMiddleBtn;

	MenuItemImage*		m_pCloseBtn;

	DigLogType			m_DialogType;

	
};
