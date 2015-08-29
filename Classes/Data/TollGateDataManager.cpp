#include "TollGateDataManager.h"
#include "cocos2d.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace cocos2d;
static string MapFileName = "config/maps/Tollgate/TollgateMap.txt";
static string InforFileName = "TollgateInfo.txt";
TollgateDataManager* TollgateDataManager::s_mTollgateManager = nullptr;


TollgateDataManager::TollgateDataManager()
{
}

TollgateDataManager* TollgateDataManager::getManager()
{
	if (! s_mTollgateManager) {
		s_mTollgateManager = new TollgateDataManager;
		s_mTollgateManager->initData();
	}
	return s_mTollgateManager;
}
void TollgateDataManager::initData()
{
	mTollgateMapList.clear();
	mTollgateInforList.clear();
	initMapConfigFile();
	initUserDataInfor();
}

void TollgateDataManager::initMapConfigFile()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	if (!json->getJsonRootValue(FileUtils::getInstance()->fullPathForFilename(MapFileName),root))
	{
		CCLOG("openFile %s : faild\n",MapFileName.c_str());
		return;
	}
	CCLOG("openFile %s : success\n",MapFileName.c_str());
	{	
		//season的
		const rapidjson::Value &arrSeason = json->getSubValue_json(root,"season");
		for (size_t i=0; i<arrSeason.Size(); ++i)
		{
			Season tempSeason;
			const rapidjson::Value &oneSeason = json->getSubValue_json(arrSeason,i);
			if (json->isHave(oneSeason,"seasonId"))
			{
				tempSeason.seasonId = json->getInt_json(oneSeason,"seasonId",0);
			}
			if (json->isHave(oneSeason,"unlockLv"))
			{
				tempSeason.unlockCondition = json->getInt_json(oneSeason,"unlockLv", 0xffffff);
			}
			//section的
			const rapidjson::Value &arrSection = json->getSubValue_json(oneSeason,"section");
			for (size_t j=0; j<arrSection.Size(); ++j)
			{
				Section tempSection;
				const rapidjson::Value &oneSection = json->getSubValue_json(arrSection,j);
				if (json->isHave(oneSection,"sectionId"))
				{
					tempSection.sectionId = json->getInt_json(oneSection,"sectionId",0);
				}
				if (json->isHave(oneSection,"name"))
				{
					tempSection.name = json->getString_json(oneSection,"name");
				}
				if (json->isHave(oneSection,"map"))
				{
					tempSection.mapName = json->getString_json(oneSection,"map");
				}
				if (json->isHave(oneSection,"passType"))
				{
					tempSection.passType = (Section::PassConditionType)json->getInt_json(oneSection,"passType",0);
				}
				if (json->isHave(oneSection,"difficulty"))
				{
					const rapidjson::Value &arrDefficulty = json->getSubValue_json(oneSection,"difficulty");
					for (size_t iDeffi=0; iDeffi<arrDefficulty.Size(); ++iDeffi)
					{
						const rapidjson::Value &oneDifficult = json->getSubValue_json(arrDefficulty,iDeffi);
						tempSection.levels.push_back(Section::levelInfor());
						if (json->isHave(oneDifficult,"describe"))
						{
							tempSection.levels[iDeffi].lvDescribe = json->getString_json(oneDifficult,"describe");
						}
						if (json->isHave(oneDifficult,"speed"))
						{
							tempSection.levels[iDeffi].speed = json->getFloatValue_json(oneDifficult,"speed", 0);
						}
						if (json->isHave(oneDifficult, "award"))
						{
							const rapidjson::Value &arrAward = json->getSubValue_json(oneDifficult,"award");
							for (size_t iAward=0; iAward<arrAward.Size(); ++iAward)
							{
								const rapidjson::Value &oneAward = json->getSubValue_json(arrAward,iAward);
								tempSection.levels[iDeffi].awardList.push_back(Section::award());
								if (json->isHave(oneAward,"awardID"))
								{
									tempSection.levels[iDeffi].awardList[iAward].awardUnitID = json->getInt_json(oneAward,"awardID",0);
								}
								if (json->isHave(oneAward,"awardCount"))
								{
									tempSection.levels[iDeffi].awardList[iAward].awardUnitCount = json->getInt_json(oneAward,"awardCount",0);
								}
								if (json->isHave(oneAward,"probability"))
								{
									const rapidjson::Value &arrProb = json->getSubValue_json(oneAward,"probability");
									CCASSERT(arrProb.Size()==3, "");
									for (size_t iProb=0; iProb<arrProb.Size(); ++iProb)
									{
										tempSection.levels[iDeffi].awardList[iAward].probability.push_back(json->getSubValue_json(arrProb,iProb).GetDouble());
									}
								}
							}
						}
					}
				}
				tempSeason.sectionList.push_back(tempSection);
			}
			mTollgateMapList.push_back(tempSeason);
		}
	}

	//
	for (int i=0,count=mTollgateMapList.size(); i<count; ++i)
	{
		for (int j=i+1; j<count; ++j)
		{
			if (mTollgateMapList[i].seasonId > mTollgateMapList[j].seasonId)
				swap(mTollgateMapList[i], mTollgateMapList[j]);
		}
	}
}

void TollgateDataManager::initUserDataInfor()
{
	//parse file
	XReadJson *json = XReadJson::getIntanse();
	Document root;
	if (!json->getJsonRootValue(FileUtils::getInstance()->getWritablePath()+InforFileName, root))
	{
		CCLOG("openFile %s : faild\n",MapFileName.c_str());
		return;
	}
	CCLOG("openFile %s : success\n",MapFileName.c_str());
	{
		if (! json->isHave(root,"season")) return;

		const rapidjson::Value &arrSeason = json->getSubValue_json(root,"season");
		for (size_t i=0; i<arrSeason.Size(); ++i)
		{
			const rapidjson::Value &oneSeason = json->getSubValue_json(arrSeason,i);
			if (! json->isHave(oneSeason,"section")) continue;

			const rapidjson::Value &arrSection = json->getSubValue_json(oneSeason,"section");
			for (size_t j=0; j<arrSection.Size(); ++j)
			{
				TollgateInfor tempInfor;
				tempInfor.seasonId = i+1;
				tempInfor.sectionId = j+1;
				const rapidjson::Value &oneSection = json->getSubValue_json(arrSection,j);
				if (json->isHave(oneSection,"lv") && json->isHave(oneSection,"got"))
				{
					const rapidjson::Value &arrPassLv = json->getSubValue_json(oneSection,"lv");
					const rapidjson::Value &arrGotRewards = json->getSubValue_json(oneSection,"got");
					if(arrPassLv.Size() != arrGotRewards.Size()){
						CCASSERT(false, "error infor");
						CCLOG("Tollgate info ---> lv arrSize != got arrSize");
						return;
					}
					for (int iNum=0,allNum=arrPassLv.Size(); iNum<allNum; ++iNum)
					{
						int lv = json->getSubValue_json(arrPassLv,iNum).GetInt();
						int got = json->getSubValue_json(arrGotRewards,iNum).GetInt();
						tempInfor.passLevel_gotRewards.push_back(std::make_pair(lv,got));
					}
				}
				mTollgateInforList.push_back(tempInfor);
			}
		}
	}
}

void TollgateDataManager::saveData()
{
	if (mTollgateInforList.empty()) return;

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//1.description
	rapidjson::Value description(rapidjson::kStringType);
	description.SetString("1.lv means the level which achieve this section, it can be 3,2 or 1 stars. 2.got means is got reward that this section. 3.why is array lv and got, because have The difficulty of the different.");
	document.AddMember("description", description, allocator);
	//2. season
	//sort
	for (int i=0,count=mTollgateInforList.size(); i<count; ++i)
	{
		for (int j=i+1; j<count; ++j)
		{
			if ((mTollgateInforList[i].seasonId > mTollgateInforList[j].seasonId) ||
				(mTollgateInforList[i].seasonId == mTollgateInforList[j].seasonId && mTollgateInforList[i].sectionId > mTollgateInforList[j].sectionId))
			{
				swap(mTollgateInforList[i], mTollgateInforList[j]);
			}
		}
	}
	rapidjson::Value arrSeason(rapidjson::kArrayType);
	rapidjson::Value arrSection(rapidjson::kArrayType);
	int seasonIdx = mTollgateInforList[0].seasonId;
	for (int i=0,count=mTollgateInforList.size(); i<count; ++i)
	{
		rapidjson::Value oneSection(rapidjson::kObjectType);

		rapidjson::Value arrLv(rapidjson::kArrayType);
		rapidjson::Value arrGot(rapidjson::kArrayType);
		for (size_t infoIdx=0; infoIdx<mTollgateInforList[i].passLevel_gotRewards.size(); ++infoIdx)
		{
			arrLv.PushBack<int>(mTollgateInforList[i].passLevel_gotRewards.at(infoIdx).first, allocator);
			arrGot.PushBack<int>(mTollgateInforList[i].passLevel_gotRewards.at(infoIdx).second, allocator);
		}
		oneSection.AddMember("lv", arrLv, allocator);
		oneSection.AddMember("got", arrGot, allocator);

		if (mTollgateInforList[i].seasonId == seasonIdx){
			arrSection.PushBack(oneSection, allocator);
		}
		else{
			rapidjson::Value oneSeason(rapidjson::kObjectType);
			oneSeason.AddMember("section", arrSection, allocator);
			arrSeason.PushBack(oneSeason,allocator);
			arrSection.Clear();
			++seasonIdx;
		}
	}
	rapidjson::Value oneSeason(rapidjson::kObjectType);
	oneSeason.AddMember("section", arrSection, allocator);
	arrSeason.PushBack(oneSeason,allocator);//最后一个
	document.AddMember("season", arrSeason, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);


	string filePath = FileUtils::getInstance()->getWritablePath();
	filePath += InforFileName;
	FILE* fp = fopen(FileUtils::getInstance()->fullPathForFilename(filePath).c_str(),"w+");
	if (!fp)
	{
		CCASSERT(fp,"active value file open faild!");
		return;
	}
	fwrite(buffer.GetString(),sizeof(char),buffer.Size(),fp);
	fclose(fp);

	CCLOG("%s ---> %s", __FUNCTION__, buffer.GetString());
}

const vector<Season>* TollgateDataManager::getAllTollgate()
{
	//seasonId已排序
	return &mTollgateMapList;
}
const Season* TollgateDataManager::getOneSeason(unsigned int seasonId)
{
	for (int i=0,count=mTollgateMapList.size(); i<count; ++i)
	{
		if (mTollgateMapList[i].seasonId == seasonId)
			return &mTollgateMapList[i];
	}
	return nullptr;
}
const Section* TollgateDataManager::getOneSection(unsigned int seasonId, unsigned int sectionId)
{
	for (size_t i=0,count=mTollgateMapList.size(); i<count; ++i)
	{
		if (mTollgateMapList[i].seasonId == seasonId)
		{
			for (size_t j=0; j<mTollgateMapList[i].sectionList.size(); ++j)
			{
				if (mTollgateMapList[i].sectionList[j].sectionId == sectionId)
					return &mTollgateMapList[i].sectionList[j];
			}
			break;
		}
	}
	return nullptr;
}

//infor
const vector<TollgateInfor>* TollgateDataManager::getTollgateInforList()
{
	return &mTollgateInforList;
}
// vector<TollgateInfor> TollgateDataManager::getTollgateInfor(unsigned int seasonId)
// {
// 	vector<TollgateInfor> ret;
// 	for (int i=0,count=mTollgateInforList.size(); i<count; ++i)
// 	{
// 		if (mTollgateInforList[i].seasonId == seasonId)
// 			ret.push_back(mTollgateInforList[i]);
// 	}
// 	return ret;
// }
const TollgateInfor* TollgateDataManager::getTollgateInfor(unsigned int seasonId, unsigned int sectionId)
{
	for (int i=0,count=mTollgateInforList.size(); i<count; ++i)
	{
		if (mTollgateInforList[i].seasonId == seasonId && mTollgateInforList[i].sectionId == sectionId)
			return &mTollgateInforList[i];
	}
	return nullptr;
}
void TollgateDataManager::setTollgateInfor(const TollgateInfor* newInfor)
{
	if (newInfor == nullptr) return;

	for (int i=0,count=mTollgateInforList.size(); i<count; ++i)
	{
		if (mTollgateInforList[i].seasonId == newInfor->seasonId && mTollgateInforList[i].sectionId == newInfor->sectionId)
		{
			mTollgateInforList[i] = *newInfor;
			saveData();
			return;
		}
	}

	mTollgateInforList.push_back(*newInfor);
	saveData();
}