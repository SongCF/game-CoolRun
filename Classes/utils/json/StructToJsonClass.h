#ifndef __StructToJsonClass_H__
#define __StructToJsonClass_H__
#include "WriteJson.h"
#include "cocos2d.h"
using namespace cocos2d;
enum AttributeType
{
	AttributeType_Int,
	AttributeType_Long,
	AttributeType_Float,
	AttributeType_Double,
	AttributeType_Bool,
	AttributeType_String,
	AttributeType_ArrayInt,
	AttributeType_ArrayString,
};
struct AttributeStruc
{
	AttributeType type;
	string name;
};
class StructToJsonClass:WriteJson
{
public:
	StructToJsonClass()
	{
	}
	~StructToJsonClass(){
		m_lIntValueList.clear();
		m_lLongValueList.clear();
		m_lDoubleValueList.clear();
		m_lFloatValueList.clear();
		m_lBoolValueList.clear();
		m_lStringValueList.clear();
		m_lArrayIntValueList.clear();
		m_lArrayStringValueList.clear();
		m_lAttributeStrucList.clear();
	}
public:
	void addAttribute(int index,AttributeType type,string name)//结构体的第几个参数,参数类型,需要存储的名字
	{
		AttributeStruc info;
		info.name = name;
		info.type = type;
		m_lAttributeStrucList.insert(map<int,AttributeStruc>::value_type(index,info));
		switch (type)
		{
		case AttributeType_Int:
			{
				addHandler(name,SEL_CallFunInt(&StructToJsonClass::getInt),index);
			}
			break;
		case AttributeType_Long:
			{
				addHandler(name,SEL_CallFunLong(&StructToJsonClass::getLong),index);
			}
			break;
		case AttributeType_Float:
			{
				addHandler(name,SEL_CallFunFloat(&StructToJsonClass::getFloat),index);
			}
			break;
		case AttributeType_Double:
			{
				addHandler(name,SEL_CallFunDouble(&StructToJsonClass::getDouble),index);
			}
			break;
		case AttributeType_Bool:
			{
				addHandler(name,SEL_CallFunBool(&StructToJsonClass::getBool),index);
			}
			break;
		case AttributeType_String:
			{
				addHandler(name,SEL_CallFunString(&StructToJsonClass::getString),index);
			}
			break;
		case AttributeType_ArrayInt:
			{
				addHandler(name,SEL_CallFunArrayInt(&StructToJsonClass::getArrayInt),index);
			}
			break;
		case AttributeType_ArrayString:
			{
				addHandler(name,SEL_CallFunArrayString(&StructToJsonClass::getArrayString),index);
			}
			break;

		default:
			break;
		}
	}
	void addValue(int index,int value)
	{
		m_lIntValueList.insert(map<int,int>::value_type(index,value));
	}
	void addValue(int index,long value)
	{
		m_lLongValueList.insert(map<int,int>::value_type(index,value));
	}
	void addValue(int index,float value)
	{
		m_lFloatValueList.insert(map<int,float>::value_type(index,value));
	}
	void addValue(int index,double value)
	{
		m_lDoubleValueList.insert(map<int,double>::value_type(index,value));
	}
	void addValue(int index,bool value)
	{
		m_lBoolValueList.insert(map<int,bool>::value_type(index,value));
	}
	void addValue(int index,string value)
	{
		m_lStringValueList.insert(map<int,string>::value_type(index,value));
	}
	void addValue(int index,vector<int> value)
	{
		m_lArrayIntValueList.insert(map<int,vector<int> >::value_type(index,value));
	}
	void addValue(int index,vector<string> value)
	{
		m_lArrayStringValueList.insert(map<int,vector<string> >::value_type(index,value));
	}

	int getInt(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_Int)
			{
				CCAssert(NULL,"getInt index = %d faild!");
			}
			else
			{
				map<int,int>::iterator iter_value = m_lIntValueList.find(index);
				if (iter_value != m_lIntValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getInt index = %d faild!");
			}
		}
		CCAssert(NULL,"getInt getFloat = %d faild!");
		return 0;
	}

	long getLong(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_Long)
			{
				CCAssert(NULL,"getLong index = %d faild!");
			}
			else
			{
				map<int,long>::iterator iter_value = m_lLongValueList.find(index);
				if (iter_value != m_lLongValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getLong index = %d faild!");
			}
		}
		CCAssert(NULL,"getLong getFloat = %d faild!");
		return 0;
	}
	float getFloat(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_Float)
			{
				CCAssert(NULL,"getFloat index = %d faild!");
			}
			else
			{
				map<int,float>::iterator iter_value = m_lFloatValueList.find(index);
				if (iter_value != m_lFloatValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getFloat index = %d faild!");
			}
		}
		CCAssert(NULL,"getFloat index = %d faild!");
		return 0;
	}
	double getDouble(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_Double)
			{
				CCAssert(NULL,"getDouble index = %d faild!");
			}
			else
			{
				map<int,double>::iterator iter_value = m_lDoubleValueList.find(index);
				if (iter_value != m_lDoubleValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getDouble index = %d faild!");
			}
		}
		CCAssert(NULL,"getDouble index = %d faild!");
		return 0;
	}
	bool getBool(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_Bool)
			{
				CCAssert(NULL,"getBool index = %d faild!");
			}
			else
			{
				map<int,bool>::iterator iter_value = m_lBoolValueList.find(index);
				if (iter_value != m_lBoolValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getBool index = %d faild!");
			}
		}
		CCAssert(NULL,"getBool index = %d faild!");
		return 0;
	}
	string getString(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_String)
			{
				CCAssert(NULL,"getString index = %d faild!");
			}
			else
			{
				map<int,string>::iterator iter_value = m_lStringValueList.find(index);
				if (iter_value != m_lStringValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getString index = %d faild!");
			}
		}
		CCAssert(NULL,"getString index = %d faild!");
		return 0;
	}
	vector<int> getArrayInt(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_ArrayInt)
			{
				CCAssert(NULL,"getArrayInt index = %d faild!");
			}
			else
			{
				map<int,vector<int> >::iterator iter_value = m_lArrayIntValueList.find(index);
				if (iter_value != m_lArrayIntValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getBool index = %d faild!");
			}
		}
		CCAssert(NULL,"getBool index = %d faild!");
		vector<int> list;
		return list;
	}
	vector<string> getArrayString(int index)
	{
		map<int,AttributeStruc>::iterator iter = m_lAttributeStrucList.find(index);
		if (iter != m_lAttributeStrucList.end())
		{
			if (iter->second.type != AttributeType_ArrayInt)
			{
				CCAssert(NULL,"getArrayInt index = %d faild!");
			}
			else
			{
				map<int,vector<string> >::iterator iter_value = m_lArrayStringValueList.find(index);
				if (iter_value != m_lArrayStringValueList.end())
				{
					return iter_value->second;
				}
				CCAssert(NULL,"getBool index = %d faild!");
			}
		}
		CCAssert(NULL,"getBool index = %d faild!");
		vector<string> list;
		return list;
	}
private:
	map<int,int>						m_lIntValueList;
	map<int,long>						m_lLongValueList;
	map<int,double>						m_lDoubleValueList;
	map<int,float>						m_lFloatValueList;
	map<int,bool>						m_lBoolValueList;
	map<int,string>						m_lStringValueList;
	map<int,vector<int> >				m_lArrayIntValueList;
	map<int,vector<string> >			m_lArrayStringValueList;
	map<int,AttributeStruc>				m_lAttributeStrucList;
};
#endif