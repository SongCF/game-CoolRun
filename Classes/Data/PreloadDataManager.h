//============================================================
//添加资源
//============================================================

#pragma once

#include "UnitDefine.h"
#include "XReadJson.h"
class PreloadDataManager
{
private:
	static PreloadDataManager* s_mPreloadManager;
	PreloadDataManager();
	void initData();
	
public:
	static PreloadDataManager* getManager();

	//获取数据
	const vector<string>* getPreloadUIScene();
	const vector<string>* getPreloadRunScene();
	const vector<string>* getPreloadMusic();
private:
	vector<string> m_UISceneStringList;
	vector<string> m_RunSceneStringList;
	vector<string> m_MusicStringList;
};