#ifndef __XReadJson_H__
#define __XReadJson_H__
#include <iostream>
#include "document.h"
#include <vector>
using namespace std;
using namespace rapidjson;
class XReadJson
{
public:
	XReadJson();
	static XReadJson* getIntanse();
	virtual ~XReadJson(void);
	bool getJsonRootValue(string fileName,Document& doc);
	bool getJsonRootValueByJsonData(const char *jsonData,rapidjson::Document& doc);

	string getString_json(const rapidjson::Value& root,const char* key);
	int getInt_json(const rapidjson::Value& root,const char* key,int def);
	int getArrayCount_json(const rapidjson::Value& root, const char* key,int def);
	float getFloatValue_json(const rapidjson::Value& root,const char* key, float def);
	bool getBooleanValue_json(const rapidjson::Value& root,const char* key, bool def);

	long getLong_json(const rapidjson::Value& root,const char* key,int def);

	string getString_json(const rapidjson::Value& root);
	int getInt_json(const rapidjson::Value& root,int def);
	float getFloatValue_json(const rapidjson::Value& root, float def);
	bool getBooleanValue_json(const rapidjson::Value& root,bool def);
	long getLong_json(const rapidjson::Value& root,int def);


	int getIntValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, int def);
	string getStringValueFromArray_json(const rapidjson::Value& root,const char* key,int idx);
	int getFloatValueFromArray_json(const rapidjson::Value& root,const char* key,int idx, float def);
	bool getBoolValueFromArray_json(const rapidjson::Value& root,const char* key,int idx,bool def);

	const rapidjson::Value &getSubValue_json(const rapidjson::Value& root,const char* arrayKey);
	const rapidjson::Value &getSubValue_json(const rapidjson::Value& arrayRoot,int index);

	bool isHave(const rapidjson::Value& root,const char* key);

	void getArrayString_json(const rapidjson::Value& arrayRoot,vector<string> &list);
	void getArrayInt_json(const rapidjson::Value& arrayRoot,vector<int> &list);
private:
};
#endif