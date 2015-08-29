#include "Uiscene.h"




Scene* Uiscene::createScene(){



	  // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Uiscene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
bool Uiscene::init(){

	 if(!Layer::init()){
		 return false;
	 }
	 
	 MainuiLayer* layer=MainuiLayer::create();
	 layer->setTag(200);
	 this->addChild(layer);
	 return true;
}
    



    // a selector callback
void Uiscene::Callback(cocos2d::Ref* pSender){


}


void Uiscene::setupdateLayer(int tag,int son){

	if(this->getChildByTag(200)!=nullptr){
		this->removeChildByTag(200);
	}

	if(tag==1){
		  MainuiLayer* layer1=MainuiLayer::create();
	      layer1->setTag(200);
	      this->addChild(layer1);
	}else if(tag==2){
		  Netpet* layer2=Netpet::create();
		  layer2->setTag(200);
		  this->addChild(layer2,1,200);
	}

}



void  Uiscene::registMsglayer(){


	

}




Uiscene::~Uiscene(){
    
	
}


