//============================================================
//* 游戏场景导演类
//* 所有场景层之间的跳转均通过该管理进行
//* 为了减少各场景之间的耦合度
//============================================================

#pragma once

#include <vector>
using std::vector;

#define TranslationTime 0.5f


enum class RunningMode
{
	teach = 0,		//教学
	endless = 1,	//无尽模式
	tollgate,		//关卡模式

};
//玩家当前模式
enum Scene_PlayMode
{
	mode_Common,
	mode_Super,
	mode_Fight,
};

class GameDirector
{
private:
	static GameDirector* s_mGameDirector;
	GameDirector();
public:
	static GameDirector* getDirector();

	// = RunningMode::endless 主页人物界面
	// = RunningMode::tollgate 关卡模式界面
	void loadingToUIScene(RunningMode posIdx=RunningMode::endless);
	void gotoUIScene(RunningMode posIdx=RunningMode::endless);
	void loadingToRunningScene(RunningMode mode, unsigned int seasonId=1,unsigned int sectionId=1,unsigned int difficutLevel=0);
	void gotoRunningScene(RunningMode mode, vector<int> sectionInfo);

	//bShowWaitingUI 是否显示转圈图片
	void showWaiting(bool bShowWaitingUI = true);
	void hideWaiting();
};