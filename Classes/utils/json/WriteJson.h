#ifndef __WriteJson_H__
#define __WriteJson_H__
#include "iostream"
#include <functional>
#include <map>
#include<stdio.h> 
#include <map>
using namespace std;
typedef std::function<string ()> getString;
typedef std::function<int ()> getInt;
typedef std::function<long ()> getLong;
typedef std::function<double ()> getDouble;
typedef std::function<float ()> getFloat;
typedef std::function<bool ()> getBool;
typedef std::function<void* ()> getObject;
class WriteJson;
typedef int (WriteJson::*SEL_CallFunInt)(int);
typedef long (WriteJson::*SEL_CallFunLong)(int);
typedef string (WriteJson::*SEL_CallFunString)(int);
typedef double (WriteJson::*SEL_CallFunDouble)(int);
typedef float (WriteJson::*SEL_CallFunFloat)(int);
typedef bool (WriteJson::*SEL_CallFunBool)(int);
typedef vector<int> (WriteJson::*SEL_CallFunArrayInt)(int);
typedef vector<string> (WriteJson::*SEL_CallFunArrayString)(int);

struct StringFun
{
	string name;
	int index;
	SEL_CallFunString callback;
};
struct IntFun
{
	string name;
	int index;
	SEL_CallFunInt callback;
};
struct LongFun
{
	string name;
	int index;
	SEL_CallFunLong callback;
};
struct DoubleFun
{
	string name;
	int index;
	SEL_CallFunDouble callback;
};
struct FloatFun
{
	string name;
	int index;
	SEL_CallFunFloat callback;
};
struct BoolFun
{
	string name;
	int index;
	SEL_CallFunBool callback;
};
struct ObjectFun
{
	string name;
	WriteJson* object;
};
struct ArrayIntFun
{
	string name;
	int index;
	SEL_CallFunArrayInt callback;
};
struct ArrayStringFun
{
	string name;
	int index;
	SEL_CallFunArrayString callback;
};
struct ArrayObjectFun
{
	string name;
	vector<WriteJson*> object;
};
class WriteJson
{
public:
	WriteJson(){};
	virtual ~WriteJson(){
		removeAllObject();
		m_lJsonObjectList.clear();
		m_lJsonStringList.clear();
		m_lJsonIntList.clear();
		m_lJsonDoubleList.clear();
		m_lJsonFlostList.clear();
		m_lJsonBoolList.clear();
		m_lJsonArrayObjectFun.clear();
		m_lJsonArrayIntFun.clear();
		m_lJsonArrayStringFun.clear();
	};
protected:
	virtual inline void addJsonObject(string name,WriteJson *obj)
	{
		int id = m_lJsonObjectList.size() + 1;
		ObjectFun fun;
		fun.name = name;
		fun.object = obj;
		m_lJsonObjectList.insert(map<int,ObjectFun>::value_type(id,fun));
	}
	virtual inline void removeAllObject()
	{
		for (auto iter = m_lJsonArrayObjectFun.begin(); iter != m_lJsonArrayObjectFun.end(); iter ++)
		{
			for (auto rIter = iter->second.object.begin() ; rIter != iter->second.object.end();rIter ++)
			{
				delete *rIter;
			}
			
		}
		for (auto iter = m_lJsonObjectList.begin(); iter != m_lJsonObjectList.end(); iter ++)
		{
			delete (iter->second.object);
		}
		m_lJsonArrayObjectFun.clear();
		m_lJsonObjectList.clear();
	}
	virtual inline void addJsonArrayObject(string name,vector<WriteJson*> obj)
	{
		int id = m_lJsonArrayObjectFun.size() + 1;
		ArrayObjectFun fun;
		fun.name = name;
		fun.object = obj;
		m_lJsonArrayObjectFun.insert(map<int,ArrayObjectFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunString str,int index = 0)
	{
		int id = m_lJsonStringList.size() + 1;
		StringFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonStringList.insert(map<int,StringFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunInt str,int index = 0)
	{
		int id = m_lJsonIntList.size() + 1;
		IntFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonIntList.insert(map<int,IntFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunLong str,int index = 0)
	{
		int id = m_lJsonIntList.size() + 1;
		LongFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonLongList.insert(map<int,LongFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunDouble str,int index = 0)
	{
		int id = m_lJsonDoubleList.size() + 1;
		DoubleFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonDoubleList.insert(map<int,DoubleFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunFloat str,int index = 0)
	{
		int id = m_lJsonFlostList.size() + 1;
		FloatFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonFlostList.insert(map<int,FloatFun>::value_type(id,fun));
		
	}
	virtual inline void addHandler(string name,SEL_CallFunBool str,int index = 0)
	{
		int id = m_lJsonBoolList.size() + 1;
		BoolFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonBoolList.insert(map<int,BoolFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunArrayInt str,int index = 0)
	{
		int id = m_lJsonArrayIntFun.size() + 1;
		ArrayIntFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonArrayIntFun.insert(map<int,ArrayIntFun>::value_type(id,fun));
	}
	virtual inline void addHandler(string name,SEL_CallFunArrayString str,int index = 0)
	{
		int id = m_lJsonArrayStringFun.size() + 1;
		ArrayStringFun fun;
		fun.name = name;
		fun.callback = str;
		fun.index = index;
		m_lJsonArrayStringFun.insert(map<int,ArrayStringFun>::value_type(id,fun));
	}
	virtual string getJsonString()
	{
		string result = "{";
		//返回int类型数据
		for (map<int,IntFun>::iterator iter = m_lJsonIntList.begin() ; iter != m_lJsonIntList.end() ; iter++)
		{
			{
				SEL_CallFunInt callback = iter->second.callback;
				result += "\n";
				result += '"';
				result += iter->second.name;
				result += '"';
				result += ':';
				int id = (this->*callback)(iter->second.index);
				char id_str[100];
				sprintf(id_str,"%d",id);
				//itoa(id,id_str,10);
				result += id_str;
				result += ",\n";
			}
		
		}
		if (m_lJsonIntList.size() > 0 && m_lJsonStringList.size() == 0 && m_lJsonObjectList.size() == 0 && m_lJsonDoubleList.size() == 0 && m_lJsonFlostList.size() == 0 && m_lJsonBoolList.size() == 0  && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0  && m_lJsonLongList.size() == 0)
		{
			result = result.substr(0,result.size() - 2);
			result += "\n";
		}
		//返回long类型数据
		for (map<int,LongFun>::iterator iter = m_lJsonLongList.begin() ; iter != m_lJsonLongList.end() ; iter++)
		{
			{
				SEL_CallFunLong callback = iter->second.callback;
				result += "\n";
				result += '"';
				result += iter->second.name;
				result += '"';
				result += ':';
				long id = (this->*callback)(iter->second.index);
				char id_str[100];
				sprintf(id_str,"%ld",id);
				//itoa(id,id_str,10);
				result += id_str;
				result += ",\n";
			}

		}
		if (m_lJsonLongList.size() > 0 && m_lJsonStringList.size() == 0 && m_lJsonObjectList.size() == 0 && m_lJsonDoubleList.size() == 0 && m_lJsonFlostList.size() == 0 && m_lJsonBoolList.size() == 0  && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 2);
			result += "\n";
		}
		//返回string类型数据
		for (map<int,StringFun>::iterator iter = m_lJsonStringList.begin() ; iter != m_lJsonStringList.end() ; iter++)
		{
			{
				SEL_CallFunString callback = iter->second.callback;
				result += "\n";
				result += '"';
				result += iter->second.name;
				result += '"';
				result += ':';
				result += '"';
				string ret = (this->*callback)(iter->second.index);
				result += ret;
				result += '"';
				result += ",\n";
			}

		}
		if (m_lJsonStringList.size() > 0 && m_lJsonObjectList.size() == 0 && m_lJsonDoubleList.size() == 0 && m_lJsonFlostList.size() == 0 && m_lJsonBoolList.size() == 0  && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 2);
			result += "\n";
		}
		//返回double类型数据
		for (map<int,DoubleFun>::iterator iter = m_lJsonDoubleList.begin() ; iter != m_lJsonDoubleList.end() ; iter++)
		{
			{
				SEL_CallFunDouble callback = iter->second.callback;
				result += "\n";
				result += '"';
				result += iter->second.name;
				result += '"';
				result += ':';
				double ret = (this->*callback)(iter->second.index);
				char id_str[1000];
				sprintf(id_str,"%lf",ret);
				result += id_str;
				result += ",\n";
			}

		}
		if (m_lJsonDoubleList.size() > 0 && m_lJsonObjectList.size() == 0 && m_lJsonFlostList.size() == 0 && m_lJsonBoolList.size() == 0  && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 2);
			result += "\n";
		}
		//返回float类型数据
		for (map<int,FloatFun>::iterator iter = m_lJsonFlostList.begin() ; iter != m_lJsonFlostList.end() ; iter++)
		{
			{
				SEL_CallFunFloat callback = iter->second.callback;
				result += "\n";
				result += '"';
				result += iter->second.name;
				result += '"';
				result += ':';
				float ret = (this->*callback)(iter->second.index);
				char id_str[1000];
				sprintf(id_str,"%f",ret);
				result += id_str;
				result += ",\n";
			}

		}
		if (m_lJsonFlostList.size() > 0 && m_lJsonObjectList.size() == 0 && m_lJsonBoolList.size() == 0  && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 2);
			result += "\n";
		}
		//返回bool类型数据
		for (map<int,BoolFun>::iterator iter = m_lJsonBoolList.begin() ; iter != m_lJsonBoolList.end() ; iter++)
		{
			{
				SEL_CallFunBool callback = iter->second.callback;
				result += "\n";
				result += '"';
				result += iter->second.name;
				result += '"';
				result += ':';
				int ret = (this->*callback)(iter->second.index);
				char id_str[2];
				sprintf(id_str,"%d",ret);
				result += id_str;
				result += ",\n";
			}

		}
		if (m_lJsonBoolList.size() > 0 && m_lJsonObjectList.size() == 0  && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 2);
			result += "\n";
		}

		for (map<int,ObjectFun>::iterator iter = m_lJsonObjectList.begin() ; iter != m_lJsonObjectList.end() ; iter++)
		{
			result += "\n";
			result += '"';
			result += iter->second.name;
			result += '"';
			result += ":[\n";
			result += (iter->second.object)->getJsonString();
			result += "],";
		}
		if (m_lJsonObjectList.size() > 0 && m_lJsonArrayObjectFun.size() == 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 1);
			result += "\n";
		}

		for (map<int,ArrayObjectFun>::iterator iter = m_lJsonArrayObjectFun.begin() ; iter != m_lJsonArrayObjectFun.end() ; iter++)
		{
			result += "\n";
			result += '"';
			result += iter->second.name;
			result += '"';
			result += ":[\n";
			for (vector<WriteJson*>::iterator iter_json = iter->second.object.begin();iter_json != iter->second.object.end();iter_json ++)
			{
				result += (*iter_json)->getJsonString();
				result += ",";
			}
			if (iter->second.object.size() > 0)
			{
				result = result.substr(0,result.size() - 1);
				result += "\n";
			}
			result += "],";
		}
		if (m_lJsonArrayObjectFun.size() > 0 && m_lJsonArrayStringFun.size() == 0 && m_lJsonArrayIntFun.size() == 0)
		{
			result = result.substr(0,result.size() - 1);
			result += "\n";
		}

		for (map<int,ArrayIntFun>::iterator iter = m_lJsonArrayIntFun.begin() ; iter != m_lJsonArrayIntFun.end() ; iter++)
		{
			SEL_CallFunArrayInt callback = iter->second.callback;
			result += "\n";
			result += '"';
			result += iter->second.name;
			result += '"';
			result += ":[\n";
			vector<int> list = (this->*callback)(iter->second.index);
			for (vector<int>::iterator iter_json = list.begin();iter_json != list.end();iter_json ++)
			{
				char id_str[10];
				sprintf(id_str,"%d",*iter_json);
				result += id_str;
				result += ",\n";
			}
			if (list.size() > 0)
			{
				result = result.substr(0,result.size() - 2);
				result += "\n";
			}
			result += "],";
		}
		if (m_lJsonArrayIntFun.size() > 0 && m_lJsonArrayStringFun.size() == 0)
		{
			result = result.substr(0,result.size() - 1);
			result += "\n";
		}

		for (map<int,ArrayStringFun>::iterator iter = m_lJsonArrayStringFun.begin() ; iter != m_lJsonArrayStringFun.end() ; iter++)
		{
			SEL_CallFunArrayString callback = iter->second.callback;
			result += "\n";
			result += '"';
			result += iter->second.name;
			result += '"';
			result += ":[\n";
			vector<string> list = (this->*callback)(iter->second.index);
			for (vector<string>::iterator iter_json = list.begin();iter_json != list.end();iter_json ++)
			{
				result += *iter_json;
				result += ",\n";
			}
			if (list.size() > 0)
			{
				result = result.substr(0,result.size() - 2);
				result += "\n";
			}
			result += "],";
		}
		if (m_lJsonArrayStringFun.size() > 0)
		{
			result = result.substr(0,result.size() - 1);
			result += "\n";
		}
		result += "}";
		return result;
	}
private:
	map<int,ObjectFun>							m_lJsonObjectList;
	map<int,StringFun>							m_lJsonStringList;
	map<int,IntFun>							m_lJsonIntList;
	map<int,LongFun>							m_lJsonLongList;
	map<int,DoubleFun>							m_lJsonDoubleList;
	map<int,FloatFun>							m_lJsonFlostList;
	map<int,BoolFun>							m_lJsonBoolList;
	map<int,ArrayObjectFun>					m_lJsonArrayObjectFun;
	map<int,ArrayIntFun>					m_lJsonArrayIntFun;
	map<int,ArrayStringFun>					m_lJsonArrayStringFun;
};
#endif