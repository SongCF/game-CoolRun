#include "StringsMap.h"
//#include "FileTools.h"
#include "XReadJson.h"
#include "cocos2d.h"

using namespace cocos2d;

namespace FX{

#define ROOT_PATH	"config/"
#define FILE_MAIN	"strings.txt"

// 所支持的标签
#define LABLE_KEY		"key"
#define LABLE_VALUE		"value"
#define LABLE_INCLUDE	"include"

map<string,string> StringsMap::s_dataMap;

// 用于缓存已经解析了的文件
static map<string,int> s_paresedFile;
bool StringsMap::m_bHasInit = false;

bool StringsMap::init()
{
	if (m_bHasInit) return true;
	else m_bHasInit = true;

	s_dataMap.clear();
	s_paresedFile.clear();
	return parseFile(FILE_MAIN);
}

bool StringsMap::parseFile(string relativePath)
{
	// 看是否已经解析过了
	if (s_paresedFile.find(relativePath) != s_paresedFile.end())
	{
		CCLOG("Waring : %s --> file[%s] is pared over before or is in parsing now ,do you include it again???",__FUNCTION__,relativePath.c_str());
		return true;
	}
	s_paresedFile.insert(pair<string, int>(relativePath,0));


	string path = string(ROOT_PATH)+relativePath;
	CCLOG("%s parsing file [%s] ....",__FUNCTION__,path.c_str());


	////
	//parse file
	Document root;
	XReadJson *json = XReadJson::getIntanse();
	json->getJsonRootValue(path,root);

	if (!root.IsObject())
	{
		return false;
	}
	const rapidjson::Value &arrayValue = json->getSubValue_json(root,"strings");
	int size = arrayValue.Size();
	for (int i = 0 ; i < size ; i++)
	{
		const rapidjson::Value &onePair = json->getSubValue_json(arrayValue,i);

		// 校验
		if(!json->isHave(onePair, LABLE_KEY) && json->isHave(onePair, LABLE_VALUE))
		{
			CCLOG("Error : %s there is no %s for %s [%s]",__FUNCTION__,LABLE_KEY,LABLE_VALUE,json->getString_json(onePair, LABLE_VALUE).c_str());
			CCASSERT(false,"key 和 value 没有配对");
			continue;
		}
		else if(json->isHave(onePair, LABLE_KEY) && ! json->isHave(onePair, LABLE_VALUE))
		{
			CCLOG("Error : %s there is no %s for %s [%s]",__FUNCTION__,LABLE_VALUE,LABLE_KEY,json->getString_json(onePair, LABLE_KEY).c_str());
			CCASSERT(false,"key 和 value 没有配对");
			continue;
		}

		//有include
		if (json->isHave(onePair, LABLE_INCLUDE))
		{
			string file = json->getString_json(onePair, LABLE_INCLUDE);
			if (!parseFile(file))
			{
				CCLOG("Error : %s parse file %s faild",__FUNCTION__,file.c_str());
				return false;
			}
		}
		// 解析键值对
		if (json->isHave(onePair, LABLE_KEY) && json->isHave(onePair, LABLE_VALUE))
		{
			string _key = json->getString_json(onePair, LABLE_KEY);
			string _value = json->getString_json(onePair, LABLE_VALUE);

			if (haveValue(_key))
			{
				CCLOG("Waring : %s --> the key [%s] in file[%s] is already used before,we will ignore this key and value[%s]!",__FUNCTION__,_key.c_str(),path.c_str(),_value.c_str());
				continue;
			}
			s_dataMap.insert(pair<string, string>(_key,_value));
		}
	}

	return true;
}

string StringsMap::getValue(string _key)
{
	CCASSERT(m_bHasInit, "has not init!");

	map<string, string>::iterator iter;
	//如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器;
	iter = s_dataMap.find(_key);
	if(iter != s_dataMap.end())
	{
		return iter->second;
	}
	CCLOG("Waring : StringsMap::getValue --> _key[%s] is not found!",_key.c_str());
	return "";
}

bool StringsMap::haveValue(string _key)
{
	map<string, string>::iterator iter;
	//如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器;
	iter = s_dataMap.find(_key);
	return iter != s_dataMap.end();
}

}//namespace