//============================================================
//* unit cache
//* 游戏场景中，obstacleSprite有大量的删除创建，为提高效率，使用缓存池实现
//============================================================

#pragma once

//#include <queue>
#include <vector>
#include "ObstacleSprite.h"

class UnitSpriteCache
{
public:
	static UnitSpriteCache* getInstance();
	ObstacleSprite * getSpriteByCache(Unit *unit);
	~UnitSpriteCache();
protected:
	UnitSpriteCache();
	void initSpriteCache();
	ObstacleSprite * getOneCache();
private:
	static UnitSpriteCache* _instance;
	std::vector<ObstacleSprite*> s_obsCache;

	int m_index;
};