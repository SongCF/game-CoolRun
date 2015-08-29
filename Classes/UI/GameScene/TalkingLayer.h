//============================================================
//* 对话层
//* 功能 ：游戏中的对话层
//============================================================

#pragma once

#include "cocos2d.h"

using namespace cocos2d;
using std::string;
using std::vector;

class TalkingLayer : public Layer
{

public:
	enum Tag
	{
		Tag_Box_Start = 100,
	};

public:
	TalkingLayer();
	~TalkingLayer();
	
	CREATE_FUNC(TalkingLayer);

	virtual bool init();

	void setCallBack(const std::function<void(int)>& fun);
	void setTalkingStrings(const vector<string>& vTalkingStrings);
	
	void createTalkingMessageBox(int iIndex);
	void removeTalkingMessageBox(int iIndex);
	
	void setCanBeTouched(bool e){m_CanBeTouched = e;};

private:
	Sprite* createTalkingMessageBox(const string& messageStr);
protected:

	std::function<void(int)>	m_fFun;
	vector<string>				m_vTalkingStrings;
	Size						m_winSize;

	int							m_iTalkingIndex;

	bool						m_CanBeTouched;
};