#include "AchievementHelper.h"
#include "AchievementConfigManager.h"
#include "BagDataManager.h"
#include "PlayerInfoDataManager.h"
#include "MsgDefine.h"
#include "cocos2d.h"
#include "BaseLayer.h"
#include "ActiveValueHelper.h"
#include "NetactorLayer.h"
#include "StopLayer.h"
//#include "sdk/SdkManager.h"

using namespace cocos2d;

AchievementHelper* AchievementHelper::s_mAchievementHelper = nullptr;

AchievementHelper::AchievementHelper()
{

}

AchievementHelper* AchievementHelper::getHelper()
{
	if (! s_mAchievementHelper) s_mAchievementHelper = new AchievementHelper;
	return s_mAchievementHelper;
}


//获取所有成就 
const vector<Achieve>* AchievementHelper::getAllAchievement()
{
	return AchievementConfigManager::getManager()->getAllAchievement();
}
void AchievementHelper::completeOneAchievement(unsigned int achieveID,int value,long lTime)
{
	{
		map<string,string> achiveInfo;
		time_t curTime = time(0);
		struct tm *curTm = localtime(&curTime);//时区时间， 非时区时间使用struct tm *gmtime(const time_t *timep); 
		char * timeStr = asctime(curTm);
		achiveInfo.insert(std::make_pair("achiveTime", timeStr));
		achiveInfo.insert(std::make_pair("achiveID", String::createWithFormat("%u", achieveID)->getCString()));
		achiveInfo.insert(std::make_pair("achiveCount", String::createWithFormat("%d", value)->getCString()));
		//SdkManager::getInstance()->logEvent(analyticsEVENT_Achive, &achiveInfo);
	}


	{
		const Achieve *achieve = getAchieveByID(achieveID);
		if (achieve->isOver)
		{
			return;
		}
	}

	if (lTime == 0)
	{
		time_t curTime;
		lTime = time(&curTime);
	}

	BagDataManager::getManager()->changeOneAchievement(achieveID,value,lTime);
	{
		const Achieve *achieve = getAchieveByID(achieveID);
		if (achieve->isOver)
		{
			m_lNewAchievementList.push_back(achieveID);
			ActiveValueHelper::getHelper()->addActiveByType(activeType_WanChengYiGeChengJiu);

			getAchieveSpecialAward(achieveID);

			//getAchievementAward(achieveID);
		}
	}
}
void AchievementHelper::getAchievementAward(unsigned int achieveID)
{
	const Achieve *achieve = AchievementConfigManager::getManager()->getAchievementByID(achieveID);
	BagDataManager::getManager()->changeOneUnitCount(achieve->award_unitID,achieve->count);
}
const Achieve *AchievementHelper::getAchieveByID(unsigned int id)
{
	Achieve *achieve = const_cast<Achieve*>(AchievementConfigManager::getManager()->getAchievementByID(id));
	const BagItem *item = BagDataManager::getManager()->getOneUnit(id);
	if (item)
	{
		achieve->process = item->value;
		switch (id)
		{
		case achieveID_ChiKuangI:
			{
				if (item->value >= 1)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_ChiKuangII:
			{
				if (item->value >= 10)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_ChiKuangIII:
			{
				if (item->value >= 50)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_ChiKuangIV:
			{
				if (item->value >= 100)
				{
					achieve->isOver = true;
				}
			}
			break;

		case achieveID_JianChiI:
			{
				if (item->value >= 10)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_JianChiII:
			{
				if (item->value >= 20)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_JianChiIII:
			{
				if (item->value >= 30)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_JianChiIV:
			{
				if (item->value >= 50)
				{
					achieve->isOver = true;
				}
			}
			break;


		case achieveID_FangYuI:
			{
				if (item->value >= 10)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_FangYuII:
			{
				if (item->value >= 20)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_FangYuIII:
			{
				if (item->value >= 30)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_FangYuIV:
			{
				if (item->value >= 50)
				{
					achieve->isOver = true;
				}
			}
			break;

		case achieveID_ShaShouI:
			{
				if (item->value >= 10)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_ShaShouII:
			{
				if (item->value >= 30)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_ShaShouIII:
			{
				if (item->value >= 50)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_ShaShouIV:
			{
				if (item->value >= 100)
				{
					achieve->isOver = true;
				}
			}
			break;

		case achieveID_JiaYou:
			{
				if (item->value >= 2)
				{
					achieve->isOver = true;
				}
			}
			break;
		case achieveID_TuanZhang:
			{
				if (item->value >= 7)
				{
					achieve->isOver = true;
				}
			}
			break;
		default:
			{
				achieve->isOver = true;
			}
			break;
		}
	}
	else
	{
		achieve->isOver = false;
	}
	return achieve;
}
void AchievementHelper::getNewAchievementList(vector<unsigned int>& list)
{
	for (vector<unsigned int>::iterator iter = m_lNewAchievementList.begin() ; iter != m_lNewAchievementList.end(); iter++)
	{
		list.push_back(*iter);
	}
	m_lNewAchievementList.clear();
}
// bool AchievementHelper::getAllAchievementOver()
// {
// 	const vector<Achieve>*list = getAllAchievement();
// 	for (vector<Achieve>::const_iterator iter = list->begin(); iter != list->end() ; iter++)
// 	{
// 		if (iter->ID != achieveID_GetAllAchievement)
// 		{
// 			if (!getAchieveByID(iter->ID)->isOver)
// 			{
// 				return false;
// 			}
// 		}
// 	}
// 	return true;
// }
const vector<unsigned int>* AchievementHelper::getGotAchievement()
{
	return PlayerInfoDataManager::getManager()->getGotAchievementList();
}

bool AchievementHelper::getIsGotAchievement(unsigned int id)
{
	const vector<unsigned int> *list = PlayerInfoDataManager::getManager()->getGotAchievementList();
	for (vector<unsigned int>::const_iterator iter = list->begin() ; iter != list->end() ; iter++)
	{
		if (*iter == id)
		{
			return true;
		}
	}
	return false;
}
void AchievementHelper::getAchievement(unsigned int id)
{
	const Achieve *achieve = getAchieveByID(id);
	if (achieve->isOver)
	{
		getAchievementAward(id);
		PlayerInfoDataManager::getManager()->setGotAchievement(id);
	}	
}
bool AchievementHelper::isAchieveSpecialAward(unsigned int id)
{
	const vector<AchieveSpecialAward> *list = AchievementConfigManager::getManager()->getAllAchieveSpecialAwardVec();
	for (vector<AchieveSpecialAward>::const_iterator iter = list->begin() ; iter != list->end() ; iter++)
	{
		if (iter->ID == id)
		{
			return true;
		}
	}
	return false;
}

const AchieveSpecialAward* AchievementHelper::getAchieveSpecialAwardInfo(unsigned int id)
{
	const vector<AchieveSpecialAward> *list = AchievementConfigManager::getManager()->getAllAchieveSpecialAwardVec();
	for (vector<AchieveSpecialAward>::const_iterator iter = list->begin() ; iter != list->end() ; iter++)
	{
		if (iter->ID == id)
		{
			return &*iter;
		}
	}
	return nullptr;
}
void AchievementHelper::getAchieveSpecialAward(unsigned int id)
{
	const AchieveSpecialAward* award = getAchieveSpecialAwardInfo(id);
	if (award)
	{
		//活跃值统计 获得新宠物
		ActiveValueHelper::getHelper()->addActiveByType(activeType_HuoDeXinChongWu);

		BagDataManager::getManager()->changeOneUnitCount(award->award_unitID,award->award_value);

		if (award->award_unitID == unitID_PiPiZhu)
		{
			if (PlayerInfoHelper::getHelper()->getDeafaultPet() == 0)
			{
				PlayerInfoHelper::getHelper()->setDeafaultPet(award->award_unitID);
			}
		}

		int iCount = 0;
		vector<BagItem> Goods = GoodsHelper::getHelper()->getAllGoods();
		for (unsigned int i = 0; i < Goods.size(); ++i)
		{
			if ((Goods.at(i).id >= ID_Pet_Start && Goods.at(i).id <= ID_Pet_End))
			{
				iCount++;
			}
		}

		//检测收藏II，收藏IV成就
		if (iCount == 2)
		{
			Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShouCangII);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShouCangII, 1);
			}
		}
		else if (iCount == 3)
		{
			Achieve*info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ShouCangIV);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_ShouCangIV, 1);
			}

			NetactorLayer::checkShouCangJiaAchievement();
		}
		StopLayer::checkFightPowerAchievement();
	}
}
long AchievementHelper::getAchieveCompleteTime(unsigned int id)
{
	long time = 0;
	const BagItem *item = BagDataManager::getManager()->getOneUnit(id);
	if (item)
	{
		time = item->value_2;
	}
	return time;
}