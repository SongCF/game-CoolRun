//============================================================
//* 场景配置管理类
//* 配置工具配置的所有场景信息由该类来解析
//* 包含 1：远景近景图， 2：场景中所有物件
//============================================================


#pragma once

#include "UnitDefine.h"
#include <vector>
#include <map>
#include <string>
#include "XReadJson.h"
#include "GameDirector.h"
#include "cocos2d.h"
using std::string;
using std::map;
using std::vector;
using namespace cocos2d;
struct SceneItem
{
	Unit *unit;
	float posX,posY;
	float rotation;
	float chance;
	unsigned int actionID;
	vector<unsigned int> triggerID;
	float scale;
	unsigned int zOrder;
	unsigned int alpha;
	Rect rect;
	SceneItem(){
		this->unit = nullptr;
		this->posX = 0;
		this->posY = 0;
		this->rotation = 0;
		this->chance = 0;
		this->actionID = 0;
		this->triggerID.clear();
		this->scale = 1.0f;
		this->alpha = 100;
		this->zOrder = 0;
	}
	SceneItem(const SceneItem& in){
		this->unit = in.unit;
		this->posX = in.posX;
		this->posY = in.posY;
		this->rotation = in.rotation;
		this->chance = in.chance;
		this->actionID = in.actionID;
		this->triggerID = in.triggerID;
		this->scale = in.scale;
		this->alpha = in.alpha;
		this->zOrder = in.zOrder;
		this->rect = in.rect;
	}
	const SceneItem& operator=(const SceneItem& in){
		this->unit = in.unit;
		this->posX = in.posX;
		this->posY = in.posY;
		this->rotation = in.rotation;
		this->chance = in.chance;
		this->actionID = in.actionID;
		this->triggerID = in.triggerID;
		this->scale = in.scale;
		this->alpha = in.alpha;
		this->zOrder = in.zOrder;
		this->rect = in.rect;
		return *this;
	}
	bool   operator < (const   SceneItem&   rhs)  const   //升序排序时必须写的函数
	{   
		return   posX   <   rhs.posX; 
	}
	bool   operator >  (const   SceneItem&   rhs)  const   //降序排序时必须写的函数
	{   
		return   posX   >   rhs.posX; 
	}
};
struct LayerBackground
{
	string fileName;
	string armatureName; // 有的漂浮物是直接用的cocostudio动画
	string animateName;
	unsigned int actionID;
	unsigned int tiggertID;
	int YOffset;
	LayerBackground()
	{
		actionID = 0;
		tiggertID = 0;
		YOffset = 0;
	}
	LayerBackground(const LayerBackground& _in){
		this->fileName = _in.fileName;
		this->armatureName = _in.armatureName;
		this->animateName = _in.animateName;
		this->actionID = _in.actionID;
		this->tiggertID = _in.tiggertID;
		this->YOffset = _in.YOffset;
	}
	const LayerBackground& operator=(const LayerBackground& _in){
		this->fileName = _in.fileName;
		this->armatureName = _in.armatureName;
		this->animateName = _in.animateName;
		this->actionID = _in.actionID;
		this->tiggertID = _in.tiggertID;
		this->YOffset = _in.YOffset;
		return *this;
	}
};
struct LayersBackground
{
	unsigned int minRandomWidth;
	unsigned int maxRandomWidth;
	vector<LayerBackground> layerList;
	int zOrder;
	float speed;
	float CameraMoveFactor_Y;
	float Scale;
	LayersBackground()
	{
		speed = 0;
		minRandomWidth = 0;
		maxRandomWidth = 0;
		layerList.clear();
		zOrder = 0;
		CameraMoveFactor_Y = 0;
		Scale = 1.0f;
	}
	LayersBackground(const LayersBackground& _in){
		this->speed = _in.speed;
		this->layerList = _in.layerList;
		this->zOrder = _in.zOrder;
		this->minRandomWidth = _in.minRandomWidth;
		this->maxRandomWidth = _in.maxRandomWidth;
		this->CameraMoveFactor_Y = _in.CameraMoveFactor_Y;
		this->Scale = _in.Scale;
	}
	const LayersBackground& operator=(const LayersBackground& _in){
		this->speed = _in.speed;
		this->layerList = _in.layerList;
		this->zOrder = _in.zOrder;
		this->minRandomWidth = _in.minRandomWidth;
		this->maxRandomWidth = _in.maxRandomWidth;
		this->CameraMoveFactor_Y = _in.CameraMoveFactor_Y;
		this->Scale = _in.Scale;
		return *this;
	}
};
struct SceneBackground 
{
	
	vector<LayersBackground> layersList;
	SceneBackground()
	{
		
		layersList.clear();
	}
	SceneBackground(const SceneBackground& _in){
		
		this->layersList = _in.layersList;
	}
	const SceneBackground& operator=(const SceneBackground& _in){

		this->layersList = _in.layersList;
		return *this;
	}
};
struct TableClothBackground 
{

	vector<LayersBackground> layersList;
	TableClothBackground()
	{

		layersList.clear();
	}
	TableClothBackground(const TableClothBackground& _in){

		this->layersList = _in.layersList;
	}
	const TableClothBackground& operator=(const TableClothBackground& _in){

		this->layersList = _in.layersList;
		return *this;
	}
};
struct Background
{
	float speed;
	int zOrder;
	int YOffset;
	float CameraMoveFactor_Y;
	float Scale;
	vector<string> mapList;

	Background(){
		this->speed = 0;
		this->zOrder = 0;
		this->YOffset = 0;
		this->CameraMoveFactor_Y = 0;
		this->Scale = 1.0f;
	}
	Background(const Background& in){
		this->speed = in.speed;
		this->mapList = in.mapList;
		this->zOrder = in.zOrder;
		this->YOffset = in.YOffset;
		this->CameraMoveFactor_Y = in.CameraMoveFactor_Y;
		this->Scale = in.Scale;
	}
	const Background& operator=(const Background& in){
		this->speed = in.speed;
		this->mapList = in.mapList;
		this->zOrder = in.zOrder;
		this->YOffset = in.YOffset;
		this->CameraMoveFactor_Y = in.CameraMoveFactor_Y;
		this->Scale = in.Scale;
		return *this;
	}
};
struct BackgroundList
{
	vector<Background> backList;
	SceneBackground scene;
	TableClothBackground table;
	BackgroundList(){
		this->backList.clear();

	}
	BackgroundList(const BackgroundList& in){
		this->backList = in.backList;
		this->scene = in.scene;
		this->table = in.table;
	}
	const BackgroundList& operator=(const BackgroundList& in){
		this->backList = in.backList;
		this->scene = in.scene;
		this->table = in.table;
		return *this;
	}
};

struct MapInfoList //一个部分的信息。 如：第一场战斗，第一个1000米。
{
	unsigned int meters;
	int count;   //该部分有多少个 cpp_map，  比如：第一个战斗关卡可能有多个cpp_map
	int startIndex; //某类型(如fight)的地图全放在一个list中，那么第一次战斗的startIndex是1，第二场战斗的startIndex就是第一场战斗cpp_map的个数+1
	MapInfoList(){
		this->meters = 0;
		this->startIndex = 0;
		this->count = 0;
	}
	MapInfoList(const MapInfoList& in){
		this->meters = in.meters;
		this->startIndex = in.startIndex;
		this->count = in.count;
	}
	const MapInfoList& operator=(const MapInfoList& in){
		this->meters = in.meters;
		this->startIndex = in.startIndex;
		this->count = in.count;
		return *this;
	}
};

struct SceneItemInfo
{
	vector<SceneItem> list;
	vector<int> backList;
	SceneItemInfo()
	{
		list.clear();
		backList.clear();
	}
	SceneItemInfo(const SceneItemInfo&_in)
	{
		this->list = _in.list;
		this->backList = _in.backList;
	}
	const SceneItemInfo& operator=(const SceneItemInfo& _in)
	{
		this->list = _in.list;
		this->backList = _in.backList;
		return *this;
	}
};

class GameSceneConfigManager
{
private:
	static GameSceneConfigManager* s_mSceneConfigManager;
	GameSceneConfigManager();
	void initData();
	void initSuperData();
	void initFightData();
	void initBackData();
	bool readData(int index,Document &root);
public:
	static GameSceneConfigManager* getManager();

	BackgroundList * getBackground(unsigned int bgID);

#if 0
	const SceneItemInfo* getSceneItems(unsigned int sceneID,int num,Scene_PlayMode PlayMode,int &screenCount,RunningMode mode,int count = 1);
#endif
	//关卡模式 （大关id， 小关id， 难度）
	const SceneItemInfo* getSceneItems(unsigned int seasonId, unsigned int sectionId, int difficultLevel, int &screenCount);
	//无尽模式  
	//1. mode 模式(超跑、战斗、正常)
	//2. phaseId 阶段id(第一场，第二场)
	//3. mapIdx 阶段地图索引(cpp_map?_1，cpp_map?_2)
	//4. screencount _输出_ 屏数
	const SceneItemInfo* getSceneItems(Scene_PlayMode mode, unsigned int phaseId, unsigned int mapIdx, int &screenCount);
	const MapInfoList* getOnePhaseMapInfo(unsigned int phaseId);

	unsigned int getBackgroundCount();
	unsigned int getSceneItemCount(Scene_PlayMode mode);

	unsigned int getSceneCount(unsigned int id,Scene_PlayMode mode,int count = 0);

	//获取指定模式 的 地图最大多少个,这里表示的一个是值一个模块，比如第一场战斗，其中可能包含多个cpp_map
	unsigned int getMaxMeter(Scene_PlayMode mode);

	const SceneItemInfo *getTollgateSceneItems(string name,int &screenCount);

	void getScenItemFromFile(const string &file,int &screenCount,vector<SceneItem> &list,vector<int> &backList);

	void getSceneItemFromJson(const rapidjson::Value& root,int &screenCount,vector<SceneItem> &list,vector<int> &backList);
	//unsigned int getScreenCount(unsigned int id,bool isSuper = false);

	int getMeterByFightCount(unsigned int fightNum,int count = 1);
protected:
	map<unsigned int , MapInfoList> m_lMapList;
	map<unsigned int, SceneItemInfo > m_sceneItemsConfigPool;
	map<unsigned int, unsigned int > m_nScreenCount;
	map<unsigned int, BackgroundList> m_bgConfigPool;

	map<unsigned int, unsigned int> m_sceneMaps;    // 背景id - 配置表id

	unsigned int					m_nMaxMeter;

	map<unsigned int , MapInfoList> m_lSuperMapList;
	map<unsigned int, SceneItemInfo > m_SupersceneItemsConfigPool;
	map<unsigned int, unsigned int > m_nSuperScreenCount;

	map<unsigned int , MapInfoList> m_lFightMapList;
	map<unsigned int, SceneItemInfo > m_FightsceneItemsConfigPool;
	map<unsigned int, unsigned int > m_nFightScreenCount;

	map<unsigned int, unsigned int> m_SuperSceneMaps;    // 背景id - 配置表id

	unsigned int					m_nSuperMaxMeter;

	SceneItemInfo					m_lTollgateSceneItemList;

	map<Scene_PlayMode,unsigned int> m_lMaxMeterList;

	map<int,int>					 m_lFightCountList;
};