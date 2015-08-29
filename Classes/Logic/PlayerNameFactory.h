/************************************************************************/
/* Random name creator, when player create a new hero, and he/she can use random name which create by GameClient   */
/* Where to use this class, and where to call destroyFactory when you leave */
/************************************************************************/

#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

class PlayerNameFactory
{
private:
	PlayerNameFactory();
	static PlayerNameFactory* _playerNameFactory;

	vector<string> _maleFirstName;
	vector<string> _femaleFirstName;
	vector<string> _lastName;
	vector<string> _repeatName;

public:
	static PlayerNameFactory* getFactory();
	void destroyFactory();

	string getRandomName(bool male);
	void addOneRepeatName(string name);
};