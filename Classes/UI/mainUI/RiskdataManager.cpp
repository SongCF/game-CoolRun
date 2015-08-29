#include "RiskdataManager.h"

#include "cocos2d.h"
using namespace cocos2d;
static string DataName = "config/risk.txt";
RiskdataManager* RiskdataManager::s_RiskManager = nullptr;


RiskdataManager::RiskdataManager(void)
{
}


RiskdataManager::~RiskdataManager(void)
{
}

void RiskdataManager::initData(){

	XReadJson* json=XReadJson::getIntanse();
	Document root;

	if (!json->getJsonRootValue(FileUtils::getInstance()->fullPathForFilename(DataName),root))
	{
		CCLOG("openFile %s : faild\n",DataName.c_str());
		return;
	}
	CCLOG("openFile %s : success\n",DataName.c_str());

	const rapidjson::Value &arrayValue = json->getSubValue_json(root,"risk");

	for(int i=0;i<arrayValue.Size();i++){

		 Risk rk;
		 const rapidjson::Value &value = json->getSubValue_json(arrayValue,i);
		 if (json->isHave(value,"ID"))
		 {
			 rk.id = json->getInt_json(value,"ID",0);
		 }
		 if(json->isHave(value,"name")){
			
			 rk.name=json->getString_json(value,"name");
		 }
		 if(json->isHave(value,"numbers")){
			 rk.numbers=json->getInt_json(value,"numbers",0);
		 }
		 risks.push_back(rk);
	}
}

RiskdataManager* RiskdataManager::getRiskmanager()
{

	if (! s_RiskManager) {
		s_RiskManager = new RiskdataManager;
		s_RiskManager->initData();
	}
	return 	s_RiskManager;
}

Risk*  RiskdataManager::getOneRisk(int id){

	for(auto& i : risks){
		   if(i.id==id){
			   return &i;
		   }
	   }
	 return nullptr;
}


vector<Risk> *RiskdataManager::getRiskList(){

	return &risks;
}