//============================================================
//* 精灵定义
//* 游戏中所有精灵（玩家角色、怪物、宠物...）
//============================================================

#pragma once

#include "cocos2d.h"
#include "ID_Define.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

//unit一共9大类型
enum UnitType
{
	unittype_None,

	unittype_Special,  //游戏场景外的道具，金币砖石等。
	unittype_Actor,
	unittype_Pet,
	unittype_Magic,
	unittype_Land,
	unittype_Item,
	unittype_Obstacle,
	unittype_Tool,			//弹簧等。（不属于item不能被吸收， 不属于obstacle不能被破坏）
	unittype_Decorative,    //装饰物
	unittype_Skin,    //装饰物
};

enum ID_Range
{
	ID_Special_Start			= 1,
	ID_Special_End				= 100,

	ID_Actor_Start				= 101,
	ID_Actor_End				= 200,
	ID_Pet_Start				= 201,
	ID_Pet_End					= 300,
	ID_Magic_Start				= 301,
	ID_Magic_End				= 400,

	ID_Land_Start				= 401,
	ID_Land_End					= 500,
	ID_Item_Start				= 501,
	ID_Item_End					= 600,
	ID_Obstacle_Start			= 601,
	ID_Obstacle_End				= 700,
	ID_Tool_Start				= 1001,
	ID_Tool_End					= 1100,
	ID_Decorative_Start			= 1101,
	ID_Decorative_End			= 1200,

	ID_Achievement_Start		= 10001,
	ID_Achievement_End			= 10100,
};


//1.Unit属性列表.xls

//1. 上下碰撞
#define Attr_TreadAddScore		"TreadScoreAdd"
#define Attr_TreadApearItem		"TreadApearItem"
#define Attr_TreadDisapear		"TreadDisapear"
#define Attr_TreadBounce		"TreadBounce"
#define Attr_TreadNoneffect		"TreadNoneffect"
#define Attr_BounceFly			"BounceFly"
#define Attr_TreadDamage		"TreadDamage"
//2. 左右碰撞
#define Attr_Obstruct			"Obstruct"
#define Attr_LeftDamage			"LeftDamage"
//3. 右左碰撞 --- 无
//4. 下上碰撞
#define Attr_TopBumpAddScore	"TopBumpAddScore"
#define Attr_TopBumpApearItem	"TopBumpApearItem"
#define Attr_TopBumpDisapear	"TopBumpDisapear"
#define Attr_TopBumpBounce		"TopBumpBounce"
#define Attr_TopBumpDamage		"TopBumpDamage"
//5. 全方位
#define Attr_ScoreAdd			"ScoreAdd"
#define Attr_Invincible			"Invincible"
#define Attr_Protect			"Protect"
#define Attr_Magnet				"Magnet"
#define Attr_ChangeToGold		"ChangeToGold"
#define Attr_SpeedUp			"SpeedUp"
#define Attr_Rush				"Rush"
#define Attr_ElectricSwitch		"ElectricSwitch"
#define Attr_Poison				"Poison"
#define Attr_Detoxify			"Detoxify"
#define	Attr_Beat				"Beat"
#define Attr_PushbackFog		"PushbackFog"
//6. 特殊点
#define Attr_DownToUp			"DownToUp"
#define Attr_GameEndPos			"GameEndPos"
#define Attr_TeachJump			"teachJump"
#define Attr_TeachDoubJump		"teachDoubleJump"
#define Attr_TeachUp			"teachUp"
#define Attr_TeachDown			"teachDown"
#define Attr_RestoreOriginPos	"RestoreOriginPos"
#define Attr_Random				"Random"
#define Attr_BossState			"BossState"
#define Attr_BossCome			"BossCome"


//人物、宠物等特有属性
#define Attr_TreadDoubleScore	"TreadDoubleScore"
#define Attr_FiveTimesScore		"FiveTimesScore"
#define Attr_ThreeJump			"ThreeJump"
#define Attr_Reborn				"Reborn"
#define	Attr_HP					"HP"
#define	Attr_Power				"Power"
//
#define	Attr_AddPower			"AddPower"
#define Attr_AddMagnet			"AddMagnet"
#define Attr_AddInvincible		"AddInvincible"
#define Attr_AddProtect			"AddProtect"
#define Attr_AddRush			"AddRush"
#define Attr_AddSpeedUp			"AddSpeedUp"
#define Attr_AddChangeGold		"AddChangeGold"
#define Attr_AddMultScore		"AddMultScore"
#define Attr_AddMultGold		"AddMultGold"



struct Attribute
{
	string attr;
	float value;

	Attribute(){
		value = 0;
	}
	Attribute(string attr, float value){
		this->attr = attr;
		this->value = value;
	}
	Attribute(const Attribute& _in){
		this->attr = _in.attr;
		this->value = _in.value;
	}
	const Attribute& operator = (const Attribute &_in){
		this->attr = _in.attr;
		this->value = _in.value;
		return *this;
	}
};


struct Unit
{
	unsigned int ID;
	UnitType type;

	string name;
	string descrigbe;
	string imageFile;
	vector<vector<int>> skinIDList;//用作替换imgFile的皮肤列表，不同米数用的皮肤不一样  (关卡模式 与 无尽模式皮肤顺序不同)
	string musicFile;
	vector<Attribute> attribute;
	vector<cocos2d::Rect> rect;
	Unit(){
		this->ID = 0;
		this->type = unittype_None;
	}
	Unit(const Unit& _in){
		this->ID = _in.ID;
		this->type = _in.type;
		this->name = _in.name;
		this->descrigbe = _in.descrigbe;
		this->imageFile = _in.imageFile;
		this->skinIDList = _in.skinIDList;
		this->attribute = _in.attribute;
		this->musicFile = _in.musicFile;
		this->rect = _in.rect;
	}
	const Unit& operator= (const Unit& _in){
		this->ID = _in.ID;
		this->type = _in.type;
		this->name = _in.name;
		this->descrigbe = _in.descrigbe;
		this->imageFile = _in.imageFile;
		this->skinIDList = _in.skinIDList;
		this->attribute = _in.attribute;
		this->musicFile = _in.musicFile;
		this->rect = _in.rect;
		return *this;
	}
	bool operator > (const Unit& _in){
		return this->ID > _in.ID;
	}
	bool operator < (const Unit& _in){
		return this->ID < _in.ID;
	}

	float getAttributeValue(string attr) const
	{
		for (int i=0,count=attribute.size(); i<count; ++i)
		{
			if (attribute[i].attr == attr)
				return attribute[i].value;
		}
		CCASSERT(false, "has not this attribute in the unit!");
		return 0;
	}
	bool haveAttribute(string attr) const
	{
		for (int i=0,count=attribute.size(); i<count; ++i)
		{
			if (attribute[i].attr == attr)
				return true;
		}
		return false;
	}

	bool haveAttrTread() const
	{
		for (int i=0,count=attribute.size(); i<count; ++i)
		{
			if (attribute[i].attr == Attr_TreadAddScore
				|| attribute[i].attr == Attr_TreadApearItem
				|| attribute[i].attr == Attr_TreadDisapear
				|| attribute[i].attr == Attr_TreadBounce
				|| attribute[i].attr == Attr_TreadNoneffect
				|| attribute[i].attr == Attr_BounceFly
				|| attribute[i].attr == Attr_TreadDamage)
				return true;
		}
		return false;
	}	
	bool haveAttrTopBump() const
	{
		for (int i=0,count=attribute.size(); i<count; ++i)
		{
			if (attribute[i].attr == Attr_TopBumpAddScore
				|| attribute[i].attr == Attr_TopBumpApearItem
				|| attribute[i].attr == Attr_TopBumpDisapear
				|| attribute[i].attr == Attr_TopBumpBounce
				|| attribute[i].attr == Attr_TopBumpDamage)
				return true;
		}
		return false;
	}
};