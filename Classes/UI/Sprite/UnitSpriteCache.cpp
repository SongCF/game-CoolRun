#include "UnitSpriteCache.h"
#include "UnitConfigManager.h"

#define MAX_CACHE_NUM 200

UnitSpriteCache* UnitSpriteCache::_instance = nullptr;


UnitSpriteCache* UnitSpriteCache::getInstance()
{
	if (! _instance) _instance = new UnitSpriteCache;
	return _instance;
}
UnitSpriteCache::UnitSpriteCache()
	:m_index(0)
{
	initSpriteCache();
}
UnitSpriteCache::~UnitSpriteCache()
{
	for (int i=0,count=s_obsCache.size(); i<count; ++i)
	{
		s_obsCache.at(i)->release();
	}
	s_obsCache.clear();
}

void UnitSpriteCache::initSpriteCache()
{
	for (int i=0,count=s_obsCache.size(); i<count; ++i)
	{
		s_obsCache.at(i)->release();
	}
	s_obsCache.clear();

	m_index = 0;
	for (int i=0; i<MAX_CACHE_NUM; ++i)
	{
		ObstacleSprite *p = new ObstacleSprite;
		s_obsCache.push_back(p);
	}
}

ObstacleSprite * UnitSpriteCache::getOneCache()
{
	//从上次的结束点开始取
	int count = s_obsCache.size();
	int start = m_index;

	for (; m_index<count; ++m_index)
	{
		if (s_obsCache[m_index]->getReferenceCount() == 1)
		{
//			CCLOG("%s ---> %d", __FUNCTION__, m_index);
			return s_obsCache[m_index++];
		}
	}
	for (m_index=0; m_index<start; ++m_index)
	{
		if (s_obsCache[m_index]->getReferenceCount() == 1)
		{
//			CCLOG("%s ---> %d", __FUNCTION__, m_index);
			return s_obsCache[m_index++];
		}
	}
	return nullptr;
}
ObstacleSprite * UnitSpriteCache::getSpriteByCache(Unit *unit)
{
	ObstacleSprite *p = getOneCache();
	if (p) {
		p->removeAllChildren();
		p->reinitialize();
//		CCLOG("1. in cache");
	}else {
		p = new ObstacleSprite;
		p->autorelease();
		CCLOG("2. no cache");
	}
	return p;
}