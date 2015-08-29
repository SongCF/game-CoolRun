#include "DataParse.h"
void DataParse::attributeParse(string str,vector<AttributeStruct> &list)
{
	while (str.size() > 0)
	{
		int pos = str.find(',');
		while (pos > 0)
		{
			string text = str.substr(0,pos);
			str.erase(0,pos + 1);
			int ret = text.find('[');
			if (ret > 0)
			{
				string attr1 = text.substr(0,ret);
				text.erase(0,ret + 1);

				ret = text.find(']');
				if (ret > 0)
				{
				}
				else
				{
					assert(NULL);
				}
				string attr2 = text.substr(0,ret);
				AttributeStruct attr;
				attr.name = attr1;
				attr.value = attr2;
				list.push_back(attr);
			}
			else
			{
				string attr1 = text;
				string attr2 = "0";
				AttributeStruct attr;
				attr.name = attr1;
				attr.value = attr2;
				list.push_back(attr);
			}
			
			pos = str.find(',');
		}
		int ret = str.find('[');
		if (ret > 0)
		{
			string attr1 = str.substr(0,ret);
			str.erase(0,ret + 1);

			ret = str.find(']');
			if (ret > 0)
			{
			}
			else
			{
				assert(NULL);
			}
			string attr2 = str.substr(0,ret);
			AttributeStruct attr;
			attr.name = attr1;
			attr.value = attr2;
			list.push_back(attr);
			str.clear();
		}
		else
		{
			string attr1 = str;
			string attr2 = "0";
			AttributeStruct attr;
			attr.name = attr1;
			attr.value = attr2;
			list.push_back(attr);
			str.clear();
		}
	}
}
void DataParse::rectParse(string str,UnitRect &rect)
{
	int pos = str.find(',');
	string text = str.substr(0,pos);
	str.erase(0,pos + 1);
	rect.x = atof(text.c_str());

	pos = str.find(',');
	text = str.substr(0,pos);
	str.erase(0,pos + 1);
	rect.y = atof(text.c_str());

	pos = str.find(',');
	text = str.substr(0,pos);
	str.erase(0,pos + 1);
	rect.width = atof(text.c_str());

	pos = str.find(',');
	text = str.substr(0,pos);
	str.erase(0,pos + 1);
	rect.height = atof(text.c_str());
}