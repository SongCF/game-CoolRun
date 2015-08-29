#ifndef __DataParse_H__
#define __DataParse_H__
#include "iostream"
#include <map>
#include <vector>
#include "assert.h"
using namespace std;
struct  AttributeStruct
{
	string name;
	string value;
};
struct UnitRect
{
	float x;
	float y;
	float width;
	float height;
};

class DataParse
{
public:
	static void attributeParse(string std,vector<AttributeStruct> &list);
	static void rectParse(string std,UnitRect &rect);
private:

};
#endif