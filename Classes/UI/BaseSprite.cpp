#include "BaseSprite.h"
//#include "ImageTypeController.h"

BaseSprite::BaseSprite(void)
{
//	mResMgr = ResourcePathManager::getManager();
	init();
}

BaseSprite* BaseSprite::create(string fileName)
{
	BaseSprite* s = new BaseSprite();
	s->initWithFile(fileName.c_str());
	s->autorelease();
	return s;
}
BaseSprite* BaseSprite::create()
{
	BaseSprite* s = new BaseSprite();
	s->init();
	s->autorelease();
	return s;
}
bool BaseSprite::initWithFile(const char *pszFilename)
{
//	mFileName = ImageTypeController::getControler()->getImageFile(pszFilename);
	mFileName = pszFilename;
	return Sprite::initWithFile(mFileName.c_str());
}
bool BaseSprite::initWithFile(string fileName)
{
	return initWithFile(fileName.c_str());
}
bool BaseSprite::initWithSpriteFrameFile(string file)
{
	return initWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(file));
}
bool BaseSprite::initWithFileDelay(string fileName,float delay)
{
	if (delay < 0)
	{
		return initWithFile(fileName);
	}else
	{
		mDelayFileName = fileName;
		runAction(Sequence::create(
			DelayTime::create(delay),
			CallFuncN::create(CC_CALLBACK_1(BaseSprite::timeToDelayInit, this)),
			nullptr
			));
	}
	return true;
}

void BaseSprite::timeToDelayInit(Node* mySelf)
{
	initWithFile(mDelayFileName);
	mDelayFileName = "";
	setScale(1.0f);
}

void BaseSprite::removeNode(Node* node)
{
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
}
BaseSprite::~BaseSprite(void)
{
}
