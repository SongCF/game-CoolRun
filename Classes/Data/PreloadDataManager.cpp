#include "PreloadDataManager.h"
#include "cocos2d.h"
using namespace cocos2d;
static string DataName = "config/PreloadResources.txt";
PreloadDataManager* PreloadDataManager::s_mPreloadManager = nullptr;


PreloadDataManager::PreloadDataManager()
{
}

PreloadDataManager* PreloadDataManager::getManager()
{
	if (! s_mPreloadManager) {
		s_mPreloadManager = new PreloadDataManager;
		s_mPreloadManager->initData();
	}
	return s_mPreloadManager;
}
void PreloadDataManager::initData()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	if (!json->getJsonRootValue(FileUtils::getInstance()->fullPathForFilename(DataName),root))
	{
		CCLOG("openFile %s : faild\n",DataName.c_str());
		return;
	}
	CCLOG("openFile %s : success\n",DataName.c_str());
	{//item的
		
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"preloadUIScene");
		json->getArrayString_json(arrayValue,m_UISceneStringList);
	}
	{
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"preloadRunScene");
		json->getArrayString_json(arrayValue,m_RunSceneStringList);
	}
	{
		const rapidjson::Value &arrayValue = json->getSubValue_json(root,"music");
		json->getArrayString_json(arrayValue,m_MusicStringList);
	}
}
const vector<string>* PreloadDataManager::getPreloadUIScene()
{
	return &m_UISceneStringList;
}
const vector<string>* PreloadDataManager::getPreloadRunScene()
{
	return &m_RunSceneStringList;
}
const vector<string>* PreloadDataManager::getPreloadMusic()
{
	return &m_MusicStringList;
}