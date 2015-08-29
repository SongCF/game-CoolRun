#include "XReadJson.h"
#include "cocos2d.h"

using namespace rapidjson;
static XReadJson *read_json = NULL;
XReadJson *XReadJson::getIntanse()
{
	if (!read_json)
	{
		read_json = new XReadJson();
	}
	return read_json;
}
XReadJson::XReadJson()
{
}
XReadJson::~XReadJson(void)
{

}
bool XReadJson::getJsonRootValue(string fileName,rapidjson::Document& doc)
{
    fileName = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName.c_str());
	CCLOG("full file name = %s", fileName.c_str());

	cocos2d::Data dat = cocos2d::FileUtils::getInstance()->getDataFromFile(fileName);
	if (dat.getSize() <= 0){
		CCLOG("no file: %s", fileName.c_str());
		return false;
	}

	long size = dat.getSize();
	char *pTempData = (char*)malloc(size + 1);
	memcpy(pTempData, dat.getBytes(), size);
	pTempData[size] = 0;
//	CCLOG("file data: %s", pTempData);  //比较大的文件会在打印的时候栈溢出  level.text。
	doc.Parse<kParseDefaultFlags>(pTempData);
	free(pTempData);

	if (doc.HasParseError())  //解析错误
	{  
		CCASSERT(false, "parse error");
		CCLOG("GetParseError %s\n",doc.GetParseError()); 
		return false; 
	}
	if (doc.IsObject())//这个时候要保证d.IsObject类型
	{
		return true;
	}
	return false;
	
}
bool XReadJson::getJsonRootValueByJsonData(const char *jsonData,rapidjson::Document& doc)
{
	doc.Parse<kParseDefaultFlags>(jsonData);

	if (doc.HasParseError())  //解析错误
	{  
		return false;
		//CCLOG("GetParseError %s\n",d.GetParseError());  
	}
	if (doc.IsObject())//这个时候要保证d.IsObject类型
	{
		return true;
	}
	//rapidjson::Value& value = doc;
	return false;
}
int XReadJson::getInt_json(const rapidjson::Value& root,const char* key,int def)
{
	int bRet = def;
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		const Value &a = root[key];  
		if (a.IsArray())  
		{  
		}  
		else
		{
			bRet = a.GetInt();
		}
	} 
	return bRet; //没查找到任何对象则返回空字符串
}
long XReadJson::getLong_json(const rapidjson::Value& root,const char* key,int def)
{
	long bRet = def;
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		const Value &a = root[key];  
		if (a.IsArray())  
		{  
		}  
		else
		{
			bRet = a.GetInt64();
		}
	} 
	return bRet; //没查找到任何对象则返回空字符串
}
float XReadJson::getFloatValue_json(const rapidjson::Value& root,const char* key, float def)
{
	float fRet = def;
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		const Value &a = root[key];  
		if (a.IsArray())  
		{  
		}  
		else
		{
			fRet = a.GetDouble();
		}
	} 

	return fRet;
}
string XReadJson::getString_json(const rapidjson::Value& root,const char* key)
{
	string fRet = "";
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		const Value &a = root[key];  
		if (a.IsArray())  
		{  
		}  
		else
		{
			fRet = a.GetString();
		}
	} 

	return fRet;
}

bool XReadJson::getBooleanValue_json(const rapidjson::Value& root,const char* key, bool def)
{
	bool bRet = def;
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		const Value &a = root[key];  
		if (a.IsArray())  
		{  
		}  
		else
		{
			bRet = a.GetBool();
		}
	} 

	return bRet;
}
int XReadJson::getArrayCount_json(const rapidjson::Value& root, const char* key,int def)
{
	int nRet = def;
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		const Value &a = root[key];  
		if (a.IsArray())  
		{  
			nRet = a.Size();
		}  
	} 
	return nRet;
}
int XReadJson::getIntValueFromArray_json(const rapidjson::Value& arrayRoot,const char* key,int idx, int def)
{
	int nRet = def;
	if (arrayRoot.IsArray())
	{
		if (idx < arrayRoot.Size())
		{
			const Value &value = arrayRoot[idx];
			nRet = value[key].GetInt();
		}
	}
	return nRet;
}
string XReadJson::getStringValueFromArray_json(const rapidjson::Value& arrayRoot,const char* key,int idx)
{
	string nRet = "";
	if (arrayRoot.IsArray())
	{
		if (idx < arrayRoot.Size())
		{
			const Value &value = arrayRoot[idx];
			nRet = value[key].GetString();
		}
	}
	return nRet;
}

int XReadJson::getFloatValueFromArray_json(const rapidjson::Value& arrayRoot,const char* key,int idx, float def)
{
	float nRet = def;
	if (arrayRoot.IsArray())
	{
		if (idx < arrayRoot.Size())
		{
			const Value &value = arrayRoot[idx];
			nRet = value[key].GetDouble();
		}
	}
	return nRet;
}

bool XReadJson::getBoolValueFromArray_json(const rapidjson::Value& arrayRoot,const char* key,int idx,bool def)
{
	bool nRet = def;
	if (arrayRoot.IsArray())
	{
		if (idx < arrayRoot.Size())
		{
			const Value &value = arrayRoot[idx];
			nRet = value[key].GetBool();
		}
	}
	return nRet;
}
const Value &XReadJson::getSubValue_json(const rapidjson::Value& root,const char* key)
{
	if (root.IsObject() && root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
//		CCLOG("has key");
		return root[key];
	}
//	CCLOG("no key");
	return root;
}

const Value &XReadJson::getSubValue_json(const rapidjson::Value& arrayRoot,int index)
{
	if (arrayRoot.IsArray() && index < arrayRoot.Size())//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		return arrayRoot[index];
	}
	return arrayRoot;
}
bool XReadJson::isHave(const rapidjson::Value& root,const char* key)
{
	if (root.HasMember(key))//这个时候要保证d湿IsObject类型 最好是 if(d.Isobject() && d.HasMember("a"))  
	{  
		return true;
	}
	return false;
}
int XReadJson::getInt_json(const rapidjson::Value& root,int def)
{
	int bRet = def;
	
	bRet = root.GetInt();
		
	return bRet; //没查找到任何对象则返回空字符串
}
long XReadJson::getLong_json(const rapidjson::Value& root,int def)
{
	long bRet = def;

	bRet = root.GetInt64();

	return bRet; //没查找到任何对象则返回空字符串
}
float XReadJson::getFloatValue_json(const rapidjson::Value& root,float def)
{
	float fRet = def;
	
	fRet = root.GetDouble();	

	return fRet;
}
string XReadJson::getString_json(const rapidjson::Value& root)
{
	string fRet = "";
	
	fRet = root.GetString();

	return fRet;
}

bool XReadJson::getBooleanValue_json(const rapidjson::Value& root, bool def)
{
	bool bRet = def;
	
	bRet = root.GetBool();	

	return bRet;
}

void XReadJson::getArrayString_json(const rapidjson::Value& arrayRoot,vector<string> &list)
{
	if (!arrayRoot.IsNull() && arrayRoot.IsArray())
	{
		int size = arrayRoot.Size();
		for (int i = 0 ; i < size ; i ++)
		{
			const rapidjson::Value &value = getSubValue_json(arrayRoot,i);
			string str = getString_json(value);
			list.push_back(str);
		}
	}
}
void XReadJson::getArrayInt_json(const rapidjson::Value& arrayRoot,vector<int> &list)
{
	if (!arrayRoot.IsNull() && arrayRoot.IsArray())
	{
		int size = arrayRoot.Size();
		for (int i = 0 ; i < size ; i ++)
		{
			const rapidjson::Value &value = getSubValue_json(arrayRoot,i);
			int str = getInt_json(value,0);
			list.push_back(str);

		}
	}
}