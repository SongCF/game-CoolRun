//============================================================
//* 精灵类  Sprite的封装
//* 所有需要显示出来的精灵，请用该类代替Sprite（方便图片特殊处理）
//============================================================


#pragma once

#include "cocos2d.h"
#include <string>
//#include "ResourcePathManager.h"

using namespace std;
using namespace cocos2d;

class BaseSprite : public Sprite
{
private:
	string mFileName;
	string mDelayFileName;
	void timeToDelayInit(Node* mySelf);
protected:
//	ResourcePathManager* mResMgr;
public:
	BaseSprite(void);
	
	static BaseSprite* create(string fileName);
	static BaseSprite* create();

	string getFileName(){return mFileName;};
	virtual bool initWithFile(const char *pszFilename);
	virtual bool initWithFile(string fileName);
	virtual bool initWithSpriteFrameFile(string file);
	virtual bool initWithFileDelay(string fileName,float delay);
	virtual void removeNode(Node* node);
	~BaseSprite(void);
};
