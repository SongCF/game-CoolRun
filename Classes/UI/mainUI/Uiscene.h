#pragma once


#include "cocos2d.h"
#include  "MainuiLayer.h"
#include "Netpet.h"
#include "Uistruct.h"
#include "RiskdataManager.h"
USING_NS_CC;



enum {

	rootr=1,
	netpet=2,
};





class Uiscene : public cocos2d::Layer
{
public:
	  // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void Callback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Uiscene);

	void setupdateLayer(int tag,int son);

	void registMsglayer();


	

	~Uiscene();
};