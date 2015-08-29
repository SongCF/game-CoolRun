//================================================
//* 文字类
//* 功能 ：字符串工具，是中文字符的键值对
/*  json文件格式
{
	"strings":[
		{
			"include" : "strings_guide.txt"
		},
		{
			"key":"hanzi",
			"value":"汉字"
		}
	]
}
*/
//================================================


#pragma once
#include <string>
#include <map>

namespace FX{

using std::map;
using std::string;

class StringsMap
{
public:
	static bool init();
	static string getValue(string _key);
	static bool haveValue(string _key);
private:
	static bool m_bHasInit;

	static bool parseFile(string relativePath);

	static map<string,string> s_dataMap;
};

}//namespace