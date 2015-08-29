#include "PlayerInfoHelper.h"
#include "PlayerInfoDataManager.h"
#include "cocos2d.h"
using namespace cocos2d;
PlayerInfoHelper* PlayerInfoHelper::s_mPlayerInfoHelper = nullptr;

PlayerInfoHelper::PlayerInfoHelper()
{

}

PlayerInfoHelper* PlayerInfoHelper::getHelper()
{
	if (! s_mPlayerInfoHelper) s_mPlayerInfoHelper = new PlayerInfoHelper;
	return s_mPlayerInfoHelper;
}
//上次游戏选择的跑酷人物
void PlayerInfoHelper::setDeafaultHero(unsigned int unitID)
{
	PlayerInfoDataManager::getManager()->setDeafaultHero(unitID);
}
unsigned int PlayerInfoHelper::getDeafaultHero()
{
	return PlayerInfoDataManager::getManager()->getDeafaultHero();
}
void PlayerInfoHelper::setDeafaultPet(unsigned int unitID)
{
	PlayerInfoDataManager::getManager()->setDeafaultPet(unitID);
}
unsigned int PlayerInfoHelper::getDeafaultPet()
{
	return PlayerInfoDataManager::getManager()->getDeafaultPet();
}

//魔法物
void PlayerInfoHelper::setDeafaultMagicItem(unsigned int unitID)
{
	PlayerInfoDataManager::getManager()->setDeafaultMagicItem(unitID);
}
unsigned int PlayerInfoHelper::getDeafaultMagicItem()
{
	return PlayerInfoDataManager::getManager()->getDeafaultMagicItem();
}

void PlayerInfoHelper::setTeacher(unsigned int unitID){

	PlayerInfoDataManager::getManager()->setPlayeriscomteacher(unitID);
}
unsigned int PlayerInfoHelper::getTeacher(){
	return PlayerInfoDataManager::getManager()->getPlayeriscomteacher();
}

//
void PlayerInfoHelper::setHistoryMaxScore(unsigned int score, unsigned int meter)
{
	PlayerInfoDataManager::getManager()->setOnePlayerInfo("MaxHistoryScore", score);
	PlayerInfoDataManager::getManager()->setOnePlayerInfo("HistoryDistance_WhenMaxScore", meter);
}
void PlayerInfoHelper::getHistoryMaxScore(unsigned int & score, unsigned int & meter)
{
	score = PlayerInfoDataManager::getManager()->getOnePlayerInfo("MaxHistoryScore");
	meter = PlayerInfoDataManager::getManager()->getOnePlayerInfo("HistoryDistance_WhenMaxScore");
}
//
void PlayerInfoHelper::setThisWeekMaxScore(unsigned int score, unsigned int meter)
{
	PlayerInfoDataManager::getManager()->setOnePlayerInfo("ThisWeekMaxScore", score);
	PlayerInfoDataManager::getManager()->setOnePlayerInfo("ThisWeekDistance_WhenMaxScore", meter);
}
void PlayerInfoHelper::getThisWeekMaxScore(unsigned int & score, unsigned int & meter)
{
	score = PlayerInfoDataManager::getManager()->getOnePlayerInfo("ThisWeekMaxScore");
	meter = PlayerInfoDataManager::getManager()->getOnePlayerInfo("ThisWeekDistance_WhenMaxScore");
}

void PlayerInfoHelper::setOneInfo(string key, int value)
{
	PlayerInfoDataManager::getManager()->setOnePlayerInfo(key, value);
}
int PlayerInfoHelper::getOneInfo(string key)
{
	return PlayerInfoDataManager::getManager()->getOnePlayerInfo(key);
}

//用户的uerid
void PlayerInfoHelper::setUserId(unsigned int id)
{
	PlayerInfoDataManager::getManager()->setUserId(id);
}
unsigned int PlayerInfoHelper::getUserId()
{
	return PlayerInfoDataManager::getManager()->getUserId();
}
void PlayerInfoHelper::changeLoginTime()
{
	PlayerInfoDataManager::getManager()->changeLoginTime();
}
bool PlayerInfoHelper::isFirstLogin()
{
	struct tm * tm;
	time_t time_sec ;
	time(&time_sec);
	tm = localtime(&time_sec);
	LoginTime time;
	time.year = tm->tm_year + 1900;
	time.month = tm->tm_mon + 1;
	time.data = tm->tm_mday;
	time.hours = tm->tm_hour;

	const LoginTime *lastTime = PlayerInfoDataManager::getManager()->getLastLoginTime();
	if (lastTime->year == time.year && lastTime->month == time.month && lastTime->data == time.data)
	{
		return false;
	}
	return true;
}
void PlayerInfoHelper::setPlayerName(const std::string &name)
{
	PlayerInfoDataManager::getManager()->setPlayerName(name);
}
const std::string PlayerInfoHelper::getPlayerName()
{
	return PlayerInfoDataManager::getManager()->getPlayerName();
}
void PlayerInfoHelper::addLoginDays()
{
	PlayerInfoDataManager::getManager()->addLoginDays();
}
unsigned int PlayerInfoHelper::getLoginDays()
{
	return PlayerInfoDataManager::getManager()->getLoginDays();
}
void PlayerInfoHelper::setUpdate(UpdateTag tag)
{
	PlayerInfoDataManager::getManager()->setUpdata(tag);
}

bool PlayerInfoHelper::getIsUpdate(UpdateTag tag)
{
	return PlayerInfoDataManager::getManager()->getIsUpdate(tag);
}
void PlayerInfoHelper::setGameLock(GameSetLock lockType,bool lock)
{
	switch (lockType)
	{
	case PlayerInfoHelper::MusicLock:
		{
			return PlayerInfoDataManager::getManager()->setOnePlayerInfo("MusicLock",lock);
		}
		break;
	case PlayerInfoHelper::MusciEffectLock:
		{
			return PlayerInfoDataManager::getManager()->setOnePlayerInfo("MusciEffectLock",lock);
		}
		break;
	case PlayerInfoHelper::SpecialEffectLock:
		{
			return PlayerInfoDataManager::getManager()->setOnePlayerInfo("SpecialEffectLock",lock);
		}
		break;
	default:
		break;
	}
}
bool PlayerInfoHelper::getGameLock(GameSetLock lockType)
{
	switch (lockType)
	{
	case PlayerInfoHelper::MusicLock:
		{
			return PlayerInfoDataManager::getManager()->getOnePlayerInfo("MusicLock");
		}
		break;
	case PlayerInfoHelper::MusciEffectLock:
		{
			return PlayerInfoDataManager::getManager()->getOnePlayerInfo("MusciEffectLock");
		}
		break;
	case PlayerInfoHelper::SpecialEffectLock:
		{
			return PlayerInfoDataManager::getManager()->getOnePlayerInfo("SpecialEffectLock");
		}
		break;
	default:
		break;
	}
	return true;
}

void PlayerInfoHelper::setMusicEnable(bool open)
{
	if (open)
		PlayerInfoDataManager::getManager()->setOnePlayerInfo("IsMusicOpen", 1);
	else 
		PlayerInfoDataManager::getManager()->setOnePlayerInfo("IsMusicOpen", -1);
}
bool PlayerInfoHelper::getMusicEnable()
{
	if (PlayerInfoDataManager::getManager()->getOnePlayerInfo("IsMusicOpen") == -1)
		return false;
	else 
		return true;
}