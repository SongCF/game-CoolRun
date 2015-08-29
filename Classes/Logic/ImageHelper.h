//============================================================
//* 图片资源管理类
//* 所有游戏场景需要通过关键字获取的 游戏资源都从这里获取
//============================================================

#pragma once

#include "BaseSprite.h"
#include <string>

using std::string;

//全是静态函数
class ImageHelper
{
public:
// 	static string getUnitSpriteFrameFileById(unsigned int unitId);
// 	//pref: use ---> static string getUnitSpriteFrameFileById(unsigned int unitId);
// 	__declspec(deprecated)  static string getGameInItemFile(unsigned int backgroundId, unsigned int unitId);
// 
// 	//static string getActorSpriteFrameFileById(unsigned int unitId);
// 	//pref: use ---> static string getAttrSpriteFrameFile(string attr);
// 	/*__declspec(deprecated)*/ static string getGameInActorFile(unsigned int unitId);
// 
// 	static string getAttrSpriteFrameFile(string attr);
// 	//pref: use ---> static string getAttrSpriteFrameFile(string attr);
// 	__declspec(deprecated)  static string getAttrImageFile(string attr);
// 


	static BaseSprite* getUnitSpriteUIScene(unsigned int unitId, int level = 0);
	static BaseSprite* getUnitSpriteRunningScene(unsigned int unitId);

	static BaseSprite* getAttrSpriteUIScene(string attr);

	static BaseSprite* getBackgroundRunningScene(string str);
};