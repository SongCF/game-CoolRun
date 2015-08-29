#include "Toolayer.h"



bool Toolayer::init(){
	if(!Layer::init()){
		return false;
	}
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	inintLayer();
	return true;
}
void Toolayer::onEnter(){
	Layer::onEnter();
}
void Toolayer::onExit(){

	Layer::onExit();
}
Toolayer::Toolayer(void)
{

}


Toolayer::~Toolayer(void)
{
}



void Toolayer::inintLayer(){

	this->setContentSize(Size(Actual_x,(Design_hight-Tophight)*(Actual_y/Design_hight)));
    
	
	list=BaselisttableLayer::create();
	
	list->setMtabletype(Gongju_list,Size(getDelwithint(300,0),getDelwithint(550,1)),3,cocos2d::extension::ScrollView::Direction::HORIZONTAL,TableView::VerticalFillOrder::TOP_DOWN,5);
	list->setPosition(Vec2(getDelwithint(40,0),0));
	addChild(list);
	

}
	//返回主页面
void Toolayer::return_Main(Ref* sender){

	
	
	
}


int Toolayer::getDelwithint(int number,int iswidth){

	int dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;

}