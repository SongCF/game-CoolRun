#pragma once
#include "XReadJson.h"


struct Risk{

	  int id;
	  string name;
	  int numbers;

	  Risk(){

		  this->id=0;
		  this->numbers=0;
	  }

};

class RiskdataManager
{

public:
	static RiskdataManager* s_RiskManager;
	RiskdataManager(void);
	~RiskdataManager(void);
	void initData();

public:
	static  RiskdataManager* getRiskmanager();

	vector<Risk> *getRiskList();
	Risk*  getOneRisk(int id);

private:
	vector<Risk>  risks;

};

