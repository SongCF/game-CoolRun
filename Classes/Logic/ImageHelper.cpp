#include "ImageHelper.h"
#include "UnitConfigManager.h"
#include "LevelDataConfigManager.h"
#include "cocos2d.h"
using namespace cocos2d;


BaseSprite* ImageHelper::getUnitSpriteUIScene(unsigned int unitId, int level/* = 0*/)
{
	Unit unit = *UnitConfigManager::getManager()->getOneUnit(unitId);
	if (level != 0)
	{
		const OneLevelInfo* info = LevelDataConfigManager::getManager()->getOneUnitLevelInfo(unitId,level);
		unit.imageFile = info->imgFile;
	}

	BaseSprite *ret = nullptr;

	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(unit.imageFile);
	if (texture){
		ret = BaseSprite::create(unit.imageFile);
	}else {
		ret = BaseSprite::create();
		ret->initWithSpriteFrameFile(unit.imageFile);
	}

	return ret;
}
BaseSprite* ImageHelper::getUnitSpriteRunningScene(unsigned int unitId)
{
	const Unit *unit = UnitConfigManager::getManager()->getOneUnit(unitId);

	BaseSprite *ret = nullptr;

	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(unit->imageFile);
	if (texture){
		ret = BaseSprite::create(unit->imageFile);
	}else {
		ret = BaseSprite::create();
		ret->initWithSpriteFrameFile(unit->imageFile);
	}

	return ret;
}

BaseSprite* ImageHelper::getAttrSpriteUIScene(string attr)
{
	string temp;
	if (attr == Attr_AddMagnet)
	{
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_CiTie)->imageFile;
	}
	else if (attr == Attr_AddProtect)
	{
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_HuDun)->imageFile;
	}
	else if (attr == Attr_AddSpeedUp)
	{
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_ChongCi)->imageFile;
	}
	else if (attr == Attr_AddChangeGold)
	{
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_DianJin)->imageFile;
	}
	else if(attr == Attr_AddInvincible){
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_WuDi)->imageFile;
	}
	else if (attr == Attr_Reborn){
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_FuHuoDaoJu)->imageFile;
	}
	else{
		CCLOG("not find!");
		//return "";
		temp = UnitConfigManager::getManager()->getOneUnit(unitID_DianJin)->imageFile;
	}


	BaseSprite *ret = nullptr;

	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(temp);
	if (texture){
		ret = BaseSprite::create(temp);
	}else {
		ret = BaseSprite::create();
		ret->initWithSpriteFrameFile(temp);
	}

	return ret;
}

BaseSprite* ImageHelper::getBackgroundRunningScene(string str)
{
	return BaseSprite::create(str);
}