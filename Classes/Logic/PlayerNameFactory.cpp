#include "PlayerNameFactory.h"
#include "cocos2d.h"
#include <fstream>
using std::ifstream;

#define FILE_FirstName_Male   "config/name_FirstName.txt"
#define FILE_FirstName_Female "config/name_FirstName.txt"
#define FILE_LastName		  "config/name_lastName.txt"


PlayerNameFactory* PlayerNameFactory::_playerNameFactory = 0;


PlayerNameFactory* PlayerNameFactory::getFactory()
{
	if (! _playerNameFactory) _playerNameFactory = new PlayerNameFactory;
	return _playerNameFactory;
}

void PlayerNameFactory::destroyFactory()
{
	if (_playerNameFactory){
		delete _playerNameFactory;
		_playerNameFactory = 0;
	}
}

PlayerNameFactory::PlayerNameFactory()
{
	srand(time(0));
#define MAX_BUF 256
	char temp[MAX_BUF];
	std::ifstream iFile;


	//male first name
	iFile.open(cocos2d::FileUtils::getInstance()->fullPathForFilename(FILE_FirstName_Male).c_str());
	if (iFile.good()){
		while (iFile.getline(temp, MAX_BUF))
		{
			_maleFirstName.push_back(temp);
		}
		iFile.close();
	}

	//female first name
	iFile.open(cocos2d::FileUtils::getInstance()->fullPathForFilename(FILE_FirstName_Female).c_str());
	if (iFile.good()){
		while (iFile.getline(temp, MAX_BUF))
		{
			_femaleFirstName.push_back(temp);
		}
		iFile.close();
	}

	//last name
	iFile.open(cocos2d::FileUtils::getInstance()->fullPathForFilename(FILE_LastName).c_str());
	if (iFile.good()){
		while (iFile.getline(temp, MAX_BUF))
		{
			_lastName.push_back(temp);
		}
		iFile.close();
	}
}

string PlayerNameFactory::getRandomName(bool male)
{
	string name;

	//last name
	int lastIdx = rand() % _lastName.size();
	name = _lastName.at(lastIdx);

	

	bool xb=true;

	if(rand()/100>50){
		xb=false;
	}
	//name+=".";
	//first name
	/*
	if (xb){
		int firstIdx = rand() % _maleFirstName.size();
		name += _maleFirstName.at(firstIdx);
	}else {
	*/
		int firstIdx = rand() % _femaleFirstName.size();
		name += _femaleFirstName.at(firstIdx);
	//}
		
	return name;
}
void PlayerNameFactory::addOneRepeatName(string name)
{
	_repeatName.push_back(name);
}