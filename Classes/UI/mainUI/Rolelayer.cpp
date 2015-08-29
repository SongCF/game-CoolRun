#include "Rolelayer.h"
#include "MainuiLayer.h"
#include "StringsMap.h"
#include "GameDirector.h"
#include "Netpet.h"
#include "SportsLayer.h"
#include "BaselisttableLayer.h"
#include "BasicLayer.h"
#include "GuideLayer.h"
#include "Shake.h"

#define pet_img     "UI_zhuye_anniu_chongwu.png"
#define hero_img    "UI_zhuye_anniu_renwu.png"
#define magic_img   "UI_zhuye_anniu_modian.png"
#define zb_img      "UI_zhuye_anniu_zhunbei.png"
#define tollgatezbItem_img "UI_zhuye_anniu_tiaozhanguanqia.png"

#define jia_img     "UI_zhuye_kuozhan_anniu_jian.png"
#define shou_img    "UI_zhuye_kuozhan_anniu_jia.png"
#define tiao_img    "UI_zhuye_kuozhan_bg.png"
#define hy_img      "UI_zhuye_kuozhan_anniu_huoyuedu.png"
#define xiaoxi_img  "UI_zhuye_kuozhan_anniu_xiaoxi.png"
#define denglu_img  "UI_zhuye_kuozhan_anniu_denglujiangli.png"
#define huodon_img  "UI_zhuye_kuozhan_anniu_huodong.png"
#define shezi_img   "UI_zhuye_kuozhan_anniu_shezhi.png"

#define shop_img     "UI_zhuye_shangdian_biaoti.png"
#define backshop_img "UI_zhuye_shangdian_bg.png"
#define  startgame_img "UI_zhuye_kaishi.png"
#define shang_img    "UI_zhuye_shangdian_fanye_shang.png"
#define faihui_img   "UI_zhuye_fanhui.png"
#define rand_img     "UI_zhuye_shangdian_suijidaoju_kuang.png"
#define hero_yingyin_img "touying_UI.png"
#define teacher_shozhi "shouzhi.png"



#define yellow_c  Color3B(255,238,148)

enum Buton{
	cjiunet=100,
	jinjinet,
	huodongnet,
	hero_ab,
	hero_b,
	pet_ab,
	pet_b,
	magic_ab,
	magic_b,
	zb_b,
	tiao_sp,
	hy_b,
	msg_b,
	login_b,
	huodong_b,
	shezhi_b,
	jian_mark,
	jiao_mark,
	shop_layertag,
	tollgateItem_tag,
	startgame_tag,
	menu_zb3,
	menu_zb2,
	menu_zb1,
	menu_zb0,
	hero_layer,
	pet_layer,
	magic_layer,
	zhuye_ftag,
	rand_daojutag,
	layer_left_tag,
	click_tag,
	click_tag2,
	tishi_daotag,
};

enum Rolelayertag{

	 left,
	 right,
};


enum Isteachmove{

	 nomarl=0,
     teacher,
};

Rolelayer::Rolelayer()
	
{


}

bool Rolelayer::init(){

	if(!Layer::init()){
		return false;
	}
	setUiMove(PlayerInfoHelper::getHelper()->getTeacher());
	state=-1;
	bingbi=-1;
	fahuimark=0;
	numtoch=0;
	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	current_oprate=0;
	isdone=0;
	layertag=Rolelayertag::left;
	initData();
	inintLayerNet();

	return true;
}

void Rolelayer::initData(){

	hero=ActorHelper::getHelper()->getOneHero(PlayerInfoHelper::getHelper()->getDeafaultHero());
	pet=ActorHelper::getHelper()->getOnePet(PlayerInfoHelper::getHelper()->getDeafaultPet());
	magic=ActorHelper::getHelper()->getOneMagic(PlayerInfoHelper::getHelper()->getDeafaultMagicItem());

}

void Rolelayer::drawCenter(){

	                float x_width= getDelwithint(200,0);
					//魔法物 人物 宠物
					string hero_str=ImageHelper::getUnitSpriteUIScene(hero.unit.ID)->getName();
					auto hero_default=MenuItemImage::create(hero_str,hero_str,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					menus.push_back(hero_default);
					hero_default->setTag(hero_ab);
					hero_default->setNormalImage(ImageHelper::getUnitSpriteUIScene(hero.unit.ID));
					hero_default->setSelectedImage(ImageHelper::getUnitSpriteUIScene(hero.unit.ID));
					//hero_default->setScale(0.85f);
					hero_default->setAnchorPoint(Vec2(0.5,0.0));
	                

					string namekl=hero_default->getName();
					hero_default->setPosition(Vec2(getContentSize().width*0.3+x_width,getContentSize().height*0.5));
					hero_default->setOpacity(0.0f);

					Layer* h_l=Layer::create();
					h_l->setTag(hero_layer);
					h_l->setPosition(hero_default->getPosition());
					addChild(h_l,1000);
					jianls.push_back(h_l);

					string arExportName, armatureName, animationData;
					getActorInfor(hero.unit.ID, arExportName, armatureName, animationData);
					playAnimationByname(h_l,"xiaoxiao_NG.ExportJson","xiaoxiao_NG",animationData,true,false,Vec2(hero_default->getContentSize().width/2,50));

					//阴影
					



					MenuItemImage* hero_default1=nullptr;
					Layer* pet_l=nullptr;

					if(pet.unit.ID!=unitID_None){
		
						pet_l=Layer::create();
						pet_l->setTag(pet_layer);

						string hero_str=pet.unit.imageFile;
						CCLOG("---->%s",hero_str.c_str());
						hero_default1=MenuItemImage::create(hero_str,hero_str,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
		
						hero_default1->setTag(pet_ab);
						hero_default1->setNormalImage(Sprite::create(hero_str));
						hero_default1->setSelectedImage(Sprite::create(hero_str));
						hero_default1->setAnchorPoint(Vec2(0.0f,0.0f));
						hero_default1->setOpacity(0.0f);
						

						string namekl=hero_default1->getName();
						float pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/2+hero_default1->getContentSize().width/2;


						float pet_yh=getContentSize().height*0.25;
						float pet_xh=getContentSize().width*0.1+x_width;
// 						if(pet.unit.ID==unitID_PiPiZhu){
// 							//animationData="limao";
// 							pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/1.5;
// 			
// 						}else if(pet.unit.ID==unitID_XiXiaoYang ){
// 							//animationData="xiaoxiong";
// 							pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/2;
// 		
// 						}else if(pet.unit.ID==unitID_BuGuNiao	){
// 							//animationData="maotouying";
// 							pet_y=hero_default1->getPositionX()+hero_default1->getContentSize().width/2;
// 						}
						hero_default1->setPosition(Vec2(pet_xh,pet_yh));
						pet_l->setPosition(hero_default1->getPosition());
						addChild(pet_l,1000);
						jianls.push_back(pet_l);
						menus.push_back(hero_default1);

						string arExportFilename, armatureName, animationName;
						getPetInfor(pet.unit.ID,arExportFilename, armatureName, animationName);
						playAnimationByname(pet_l,arExportFilename,armatureName,animationName,true,false,Vec2(hero_default1->getContentSize().width/2,50));


						//
						//
						vector<Achieve> allacives=*AchievementHelper::getHelper()->getAllAchievement();

						for(auto m:allacives){

							
                            bool isfinish=AchievementHelper::getHelper()->getAchieveByID(m.ID)->isOver;
					        bool islq=AchievementHelper::getHelper()->getIsGotAchievement(m.ID);
                            const AchieveSpecialAward* pa= AchievementHelper::getHelper()->getAchieveSpecialAwardInfo(m.ID);
							if(pa!=nullptr){
								if(isfinish==true&&islq==false&&(pa->award_unitID>=unitID_PiPiZhu&&pa->award_unitID<unitID_PetEnd)){

									bingbi=1;
									pet_l->setVisible(false);
									auto click=Sprite::create("UI_jiesuan_longdan.png");
									click->setPosition(Vec2(getContentSize().width*0.3+x_width,getContentSize().height*0.65));
									click->setTag(click_tag2);
									addChild(click);
									click->setLocalZOrder(1025);
									achiveId=m.ID;

									click->runAction(Sequence::create(Shake::create(0.5f,10),
										MoveTo::create(0.5f,Vec2(94,getContentSize().height*0.60)),
										ScaleTo::create(0.5f,0),
										CallFuncN::create(CC_CALLBACK_1(Rolelayer::Movedone,this)),
										nullptr));  

									break;

								}
							}

						}
					}

					MenuItemImage* hero_default2=nullptr;
					Layer* magic_l=nullptr;
					if(magic.unit.ID!=unitID_None){
		
						magic_l=Layer::create();
						magic_l->setTag(magic_layer);

						string magic_name="UI_modian_1.png";
						magic_name=__String::createWithFormat("UI_modian_%d.png",magic.level)->getCString();
					    

						hero_default2=MenuItemImage::create(magic_name,magic_name,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
						hero_default2->setTag(magic_ab);
						hero_default2->setOpacity(0);
						
						
						if(hero.unit.ID==unitID_Actor2){
						   hero_default2->setPosition(Vec2((getContentSize().width)*0.15+x_width,getContentSize().height*0.875));
						}else{
						   hero_default2->setPosition(Vec2((getContentSize().width)*0.12+x_width,getContentSize().height*0.875));

						}
						string animationData=  CCString::createWithFormat("b_huxi%d", magic.level)->getCString();
						magic_l->setPosition(hero_default2->getPosition());
						addChild(magic_l,1000);
						jianls.push_back(magic_l);
						menus.push_back(hero_default2);
						playAnimationByname(magic_l,"book_ui.ExportJson","book_ui",animationData,true,false,Vec2(hero_default2->getContentSize().width/2,0));
					}

					auto menu0=Menu::create(hero_default2,hero_default,hero_default1,nullptr);
					menu0->setPosition(Vec2::ZERO);
					menu0->setTag(menu_zb0);
					addChild(menu0,1000);

}

void Rolelayer::Movedone(Node* node){


	bingbi=0;
	this->getChildByTag(pet_layer)->setVisible(true);
	this->getChildByTag(click_tag2)->removeFromParent();
	AchievementHelper::getHelper()->getAchievement(achiveId);
	auto p=Sprite::create("UI_zhuye_tishi_youshang.png");
	p->setPosition(Vec2(getDelwithint(188,1),getContentSize().height*0.68));
	p->setTag(tishi_daotag);
	p->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5,0.8f),
		                                 ScaleTo::create(0.5,1.0f),nullptr
		                                 )));
	addChild(p,1025);
}

void Rolelayer::drawLeftlayer(){

	float x_width= getDelwithint(200,0);
	layertag=Rolelayertag::left;
	if(this->getChildByTag(layer_left_tag)==nullptr){
		            
		            left_layer=Layer::create();
					left_layer->setContentSize(Size(Actual_x,Actual_y));
					left_layer->setTag(layer_left_tag);
					left_layer->setPosition(Vec2(0,0));
					addChild(left_layer);
		            
					auto herobutton=MenuItemImage::create(hero_img,hero_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					herobutton->setPosition(Vec2(herobutton->getContentSize().width/2,getContentSize().height*0.8));
					herobutton->setTag(Buton::hero_b);
					fillOuty(herobutton,hero_img);
	  
					if(getUimove()==Isteachmove::teacher){

						/*
						auto p_s=GuideLayer::createWithPrama(GuideLayer::MaskType::MaskType_Rectangle,herobutton->getPosition());
						p_s->setSize(herobutton->getContentSize());
						addChild(p_s);
						 */
						
						auto p=Sprite::create(teacher_shozhi);
						p->setAnchorPoint(Vec2(0.5,0.5));
						p->setRotation(-90);
						p->setPosition(Vec2(herobutton->getContentSize().width,herobutton->getPositionY()+herobutton->getContentSize().height/2));
						addChild(p);
						p->setGlobalZOrder(1000);
						p->runAction(RepeatForever::create(Sequence::create( FadeOut::create(0.5f),FadeIn::create(0.5f),nullptr)));
						
					}

					auto petbutton=MenuItemImage::create(pet_img,pet_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					petbutton->setPosition(Vec2(petbutton->getContentSize().width/2,getContentSize().height*0.60));
					petbutton->setTag(Buton::pet_b);
					fillOuty(petbutton,pet_img);

					auto magicbutton=MenuItemImage::create(magic_img,magic_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					magicbutton->setPosition(Vec2(magicbutton->getContentSize().width/2,getContentSize().height*0.4));
					magicbutton->setTag(Buton::magic_b);
					fillOuty(magicbutton,magic_img);

					auto menu=Menu::create(
										   herobutton,petbutton,magicbutton,
										   nullptr);
					menu->setPosition(Vec2::ZERO);
					menu->setTag(menu_zb1);
					left_layer->addChild(menu, 1000);

					tollgate_zb_menu=MenuItemImage::create(tollgatezbItem_img,tollgatezbItem_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					tollgate_zb_menu->setTag(Buton::tollgateItem_tag);
					tollgate_zb_menu->setPosition(Vec2(getContentSize().width*0.8,tollgate_zb_menu->getContentSize().height*0.5));
					fillOuty(tollgate_zb_menu,tollgatezbItem_img);
					if(getUimove()!=Isteachmove::teacher)
					{
						auto armature =Armature::create("Run_UI_zhuye_anniu_xiaoguo");
						armature->getAnimation()->play("chuxian",-1,1);
						armature->setPosition(Vec2(tollgate_zb_menu->getContentSize().width/2,tollgate_zb_menu->getContentSize().height/2));
						tollgate_zb_menu->addChild(armature);
					}


					zb_menu=MenuItemImage::create(zb_img,zb_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					zb_menu->setTag(Buton::zb_b);
					zb_menu->setPosition(Vec2(tollgate_zb_menu->getPositionX(),tollgate_zb_menu->getPositionY()+zb_menu->getContentSize().height*1.2));
					fillOuty(zb_menu,zb_img);

					auto menutag=Menu::create(tollgate_zb_menu,zb_menu,nullptr);
					menutag->setPosition(Vec2::ZERO);
					left_layer->addChild(menutag);

					auto back_img2=Sprite::create(tiao_img);
					back_img2->setTag(tiao_sp);
					back_img2->setAnchorPoint(Vec2(0,0));
					back_img2->setPosition(Vec2(70,10));//zb_menu->getPositionY()-50));
					left_layer->addChild(back_img2);

					auto hy_bt=MenuItemImage::create(hy_img,hy_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					hy_bt->setTag(hy_b);
					hy_bt->setPosition(Vec2(back_img2->getContentSize().width*0.23,back_img2->getContentSize().height*0.65));
	                fillOuty(hy_bt,hy_img);

					auto text_hy=Label::createWithSystemFont(FX::StringsMap::getValue("active"),"minijiankatong.ttf", 24);
					text_hy->setColor(yellow_c);
					text_hy->setPosition(Vec2(hy_bt->getPositionX(),back_img2->getContentSize().height*0.25));
					back_img2->addChild(text_hy);

	
					auto xiaoxi_bt=MenuItemImage::create(xiaoxi_img,xiaoxi_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					xiaoxi_bt->setTag(msg_b);
					xiaoxi_bt->setPosition(Vec2(back_img2->getContentSize().width*0.4,back_img2->getContentSize().height*0.65));
					fillOuty(xiaoxi_bt,xiaoxi_img);


					auto text_xx=Label::createWithSystemFont(FX::StringsMap::getValue("msg"),"minijiankatong.ttf", 24);
					text_xx->setColor(yellow_c);
					text_xx->setPosition(Vec2(xiaoxi_bt->getPositionX(),back_img2->getContentSize().height*0.25));
					back_img2->addChild(text_xx);


					auto login_bt=MenuItemImage::create(denglu_img,denglu_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					login_bt->setTag(login_b);
					login_bt->setPosition(Vec2(back_img2->getContentSize().width*0.55,back_img2->getContentSize().height*0.65));
					fillOuty(login_bt,denglu_img);


					auto text_lg=Label::createWithSystemFont(FX::StringsMap::getValue("logg"),"minijiankatong.ttf", 24);
					text_lg->setColor(yellow_c);
					text_lg->setPosition(Vec2(login_bt->getPositionX(),back_img2->getContentSize().height*0.25));
					back_img2->addChild(text_lg);

					auto activ_bt=MenuItemImage::create(huodon_img,huodon_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					activ_bt->setTag(huodong_b);
					activ_bt->setPosition(Vec2(back_img2->getContentSize().width*0.72,back_img2->getContentSize().height*0.65));
					fillOuty(activ_bt,huodon_img);


					auto text_ac=Label::createWithSystemFont(FX::StringsMap::getValue("huodong"),"minijiankatong.ttf", 24);
					text_ac->setColor(yellow_c);
					text_ac->setPosition(Vec2(activ_bt->getPositionX(),back_img2->getContentSize().height*0.25));
					back_img2->addChild(text_ac);

					auto shezi_bt=MenuItemImage::create(shezi_img,shezi_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					shezi_bt->setTag(shezhi_b);
					shezi_bt->setPosition(Vec2(back_img2->getContentSize().width*0.88,back_img2->getContentSize().height*0.65));
					fillOuty(shezi_bt,shezi_img);

					auto text_sz=Label::createWithSystemFont(FX::StringsMap::getValue("set"),"minijiankatong.ttf", 24);
					text_sz->setColor(yellow_c);
					text_sz->setPosition(Vec2(shezi_bt->getPositionX(),back_img2->getContentSize().height*0.25));
					back_img2->addChild(text_sz);



					auto menu1=Menu::create(hy_bt,xiaoxi_bt,login_bt,activ_bt,shezi_bt,
											nullptr);
					menu1->setPosition(Vec2::ZERO);
					back_img2->addChild(menu1);

					//默认为减
					oprate_bt=MenuItemImage::create(jia_img,jia_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
					oprate_bt->setAnchorPoint(Vec2(0.5,0));
					oprate_bt->setPosition(Vec2(oprate_bt->getContentSize().width/2,10));//zb_menu->getPositionY()-50));
					oprate_bt->setLocalZOrder(100);
					oprate_bt->setTag(jian_mark);
					

					auto menu2=Menu::create(oprate_bt,nullptr);
					menu2->setPosition(Vec2::ZERO);
					menu2->setTag(menu_zb2);
					left_layer->addChild(menu2);
					allCallbackm(oprate_bt);
		}
}


void Rolelayer::inintLayerNet(){


	float x_width= getDelwithint(200,0);
	this->setContentSize(Size(Actual_x,Actual_y-getDelwithint(Tophight,1)));
	drawLeftlayer();
	drawCenter();
}

void Rolelayer::CscallBack(Ref* pSender,Control::EventType event){

	 log("touchdown");  

}

bool Rolelayer::isContactpoint(Vec2 p,Sprite* sp){
 
	  float widthl=sp->getPositionX()-sp->getContentSize().width/2;
	  float widthr=sp->getPositionX()+sp->getContentSize().width/2;
	  float hightx=sp->getPositionY()-sp->getContentSize().height/2;
	  float hights=sp->getPositionY()+sp->getContentSize().height/2;
	  if(widthl<p.x&&p.x<widthr&&hightx<p.y&&p.y<hights){
		  FX::Music::getInstance()->buttonClicked();
		  return true;
	  }else{
		  return false;
	  }
 }


void Rolelayer::Gonengweikaifan(){

	auto p=BasicLayer::create();
    p->Cannokaifang();
	Director::getInstance()->getRunningScene()->addChild(p);
}

void Rolelayer::addNodeevent(){




}

void Rolelayer::drawshoplayer(){
	
	layertag=Rolelayertag::right;
	if(this->getChildByTag(shop_layertag)==nullptr){
			shop_layer=Layer::create();
			shop_layer->setContentSize(Size(Actual_x,Actual_y));
			shop_layer->setTag(shop_layertag);
			shop_layer->setPosition(Vec2(getContentSize().width,0));
			addChild(shop_layer);

			auto center=Sprite::create(backshop_img);

			auto top=Sprite::create(shop_img);
			top->setPosition(Vec2(shop_layer->getContentSize().width-center->getContentSize().width/2,
						shop_layer->getContentSize().height-top->getContentSize().height/2));
			top->setLocalZOrder(100);
			shop_layer->addChild(top);
			

	
			center->setPosition(Vec2(shop_layer->getContentSize().width-center->getContentSize().width/2,
				shop_layer->getContentSize().height-top->getContentSize().height-center->getContentSize().height/2+40));
			center->setLocalZOrder(90);
			shop_layer->addChild(center,99);
	
	        


			auto sha_img=Sprite::create(shang_img);
			sha_img->setPosition(Vec2(center->getContentSize().width*0.5,center->getContentSize().height*0.9f));
			center->addChild(sha_img);

			auto list=BaselisttableLayer::create();
			list->setMtabletype(Daoju_list,Size(499,105),3,cocos2d::extension::ScrollView::Direction::VERTICAL,TableView::VerticalFillOrder::TOP_DOWN,0);
			list->ignoreAnchorPointForPosition(false);
			list->setAnchorPoint(Vec2(0.5f,0.5f));
			list->setPosition(Vec2(center->getContentSize().width/2,center->getContentSize().height*0.52));
			center->addChild(list);

			auto xia_img=Sprite::create(shang_img);
			xia_img->setRotation(180);
			xia_img->setPosition(Vec2(center->getContentSize().width*0.5,center->getContentSize().height*0.13f));
			center->addChild(xia_img);

			auto dom=MenuItemImage::create(startgame_img,startgame_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
			dom->setTag(startgame_tag);
			dom->setPosition(Vec2(center->getPositionX(),dom->getContentSize().height/2));
			fillOuty(dom,startgame_img);


			auto zheye_b=MenuItemImage::create(faihui_img,faihui_img,CC_CALLBACK_1(Rolelayer::allCallbackm,this));
			zheye_b->setPosition(Vec2(zheye_b->getContentSize().width/2,zheye_b->getContentSize().height/2));
			zheye_b->setTag(zhuye_ftag);
			fillOuty(zheye_b,faihui_img);

			auto menu=Menu::create(dom,zheye_b,nullptr);
			menu->setPosition(Vec2::ZERO);
			menu->setLocalZOrder(100);
			shop_layer->addChild(menu);


			rand_ig=Sprite::create(rand_img);
			rand_ig->setPosition(Vec2(center->getContentSize().width/2,center->getContentSize().height*0.15));
			center->addChild(rand_ig);
			

			auto text_rd=Label::createWithSystemFont("1234", "minijiankatong.ttf",24);
			text_rd->setTag(rand_daojutag);
			text_rd->setPosition(Vec2(rand_ig->getContentSize().width/2,rand_ig->getContentSize().height/2));
			rand_ig->addChild(text_rd);


	}
	auto move = MoveTo::create(0.5f, Vec2(Actual_x-shop_layer->getContentSize().width,0));    
    auto move_ease_inout0 = EaseExponentialOut::create(move);
	shop_layer->runAction(move_ease_inout0);

	rand_ig->setVisible(false);
}

void Rolelayer::setcall(Node* node){

	 numtoch=0;
	 Layer* p=(Layer*)this->getChildByTag(layer_left_tag);
	 p->setPosition(Vec2(-Actual_x,0));
}

void  Rolelayer::layerChange(){
	
   
	
	    Layer* p=(Layer*)this->getChildByTag(layer_left_tag);
		auto move = MoveTo::create(0.25,Vec2(-Actual_x,0));    
		auto move_ease_inout0 = EaseExponentialOut::create(move);
		p->runAction(
			
			Sequence::create(move_ease_inout0,CallFuncN::create( CC_CALLBACK_0(Rolelayer::setcall,this,nullptr)),nullptr)
		);
	   for(auto n:menus){
			auto move = MoveTo::create(0.5f, Vec2(n->getPositionX()-250,n->getPositionY()));    
			auto move_ease_inout0 = EaseExponentialOut::create(move);
			n->runAction(move_ease_inout0);
	   }
	
	   for(auto m:jianls){
			auto move = MoveTo::create(0.5f, Vec2(m->getPositionX()-250,m->getPositionY()));    
			auto move_ease_inout0 = EaseExponentialOut::create(move);
			m->runAction(move_ease_inout0);
	   }
	   //toplayer 成就 按钮消失
	   EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	   EventCustom event(Topmsg_ch);
	   MsgData data;
	   data.value =1;
	   event.setUserData(&data);
	   eventDspc->dispatchEvent(&event);

	   //主页商店背景效果 
	   {
		   EventCustom event_m(Layerchange);
		   LayerData data_m;
		   data_m.current=101;
		   data_m.push=-1;
		   event_m.setUserData(&data_m);
		   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event_m);
	   }
}

void Rolelayer::setcallfaihui(Node* node){

	fahuimark=0;
	Layer* p=(Layer*)this->getChildByTag(layer_left_tag);
	p->setPosition(Vec2(0,0));
	 
}

void  Rolelayer::layeReturnpop(){

	if (tollgate_zb_menu) tollgate_zb_menu->setVisible(true);
	if (zb_menu) zb_menu->setVisible(true);

	layertag=Rolelayertag::left;
	{

		 Layer* p=(Layer*)this->getChildByTag(layer_left_tag);
		 if (p){
			auto move = MoveTo::create(0.25,Vec2(0,0));    
			auto move_ease_inout0 = EaseExponentialOut::create(move);
			p->runAction(
				
				Sequence::create(move_ease_inout0,CallFuncN::create( CC_CALLBACK_0(Rolelayer::setcallfaihui,this,nullptr)),nullptr)
			);
		 }
	}
	
	Layer* p=(Layer*)this->getChildByTag(shop_layertag);
	if (p){
		auto move = MoveTo::create(0.25,Vec2(Actual_x,0));    
		auto move_ease_inout0 = EaseExponentialOut::create(move);
		p->runAction(
			Sequence::create(move_ease_inout0,
			CallFuncN::create( CC_CALLBACK_0(Rolelayer::spcall,this,p,true)),nullptr)				
			);
	}

	 for(auto n:menus){
			auto move = MoveTo::create(0.5f, Vec2(n->getPositionX()+250,n->getPositionY()));    
			auto move_ease_inout0 = EaseExponentialOut::create(move);
			n->runAction(move_ease_inout0);
	 }
	 for(auto m:jianls){
	
			auto move = MoveTo::create(0.5f, Vec2(m->getPositionX()+250,m->getPositionY()));    
			auto move_ease_inout0 = EaseExponentialOut::create(move);
			m->runAction(move_ease_inout0);
	 }
     {
			   EventCustom event(Layerchange);
			   LayerData data_m;
			   data_m.current=101;
			   data_m.push=1;
			   event.setUserData(&data_m);
			   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
           
     }
	

}

void Rolelayer::spcall(Node* node,bool mr){

	Layer* p=(Layer*)this->getChildByTag(shop_layertag);
	p->setPosition(Vec2(Actual_x,0));
   
   EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
   EventCustom event(Topmsg_ch);
   MsgData data;
   data.value =2;
   event.setUserData(&data);
   eventDspc->dispatchEvent(&event);

   
}

float Rolelayer::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
	return dealnumber;
}

void Rolelayer::bAction(MenuItemImage* node){

	
}

void Rolelayer::layerdaozhi(){



	if(layertag==Rolelayertag::right){
			   EventCustom event(Layerchange);
			   LayerData data_m;
			   data_m.current=101;
			   data_m.push=1;
			   event.setUserData(&data_m);
			   Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	   }
           
}

void Rolelayer::callsp(int value){
	
	     

	    
		if(current_oprate==hero_b)
		{
	        layerdaozhi();
			go_layer(12);
		}else if(current_oprate==pet_b){
			layerdaozhi();
			go_layer(15);
		}else if(current_oprate==click_tag2){
		
			

		
		}else if(current_oprate==magic_b){
			layerdaozhi();
			go_layer(14);
		}else if(current_oprate==hy_b){
            current_oprate=0;
	        isdone=0;
			Gonengweikaifan(); //测试分享内容
			/*
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
						SdkManager::getInstance()->share(nullptr);
			#endif
			*/
           
		}else if(current_oprate==msg_b){
			current_oprate=0;
	        isdone=0;
			Gonengweikaifan();
		}else if(current_oprate==login_b){
			current_oprate=0;
	        isdone=0;
			Gonengweikaifan();
		}else if(current_oprate==huodong_b){
			current_oprate=0;
	        isdone=0;
			Gonengweikaifan();
		}else if(current_oprate==shezhi_b){
			go_layer(100);

		}else if(current_oprate==jian_mark){
			current_oprate=0;
	        isdone=0;
			//收起
			oprate_bt->setNormalImage(Sprite::create(shou_img));
			oprate_bt->setSelectedImage(Sprite::create(shou_img));
			oprate_bt->setTag(jiao_mark);



		}else if(current_oprate==jiao_mark){
			current_oprate=0;
	        isdone=0;
			//展开
			oprate_bt->setNormalImage(Sprite::create(jia_img));
			oprate_bt->setSelectedImage(Sprite::create(jia_img));
			oprate_bt->setTag(jian_mark);
		}else if(current_oprate==startgame_tag){

			current_oprate=0;
	        isdone=0;
			//展开
				int iCount = PlayerInfoDataManager::getManager()->getOnePlayerInfo(key_playGameCount);
				PlayerInfoDataManager::getManager()->setOnePlayerInfo(key_playGameCount, iCount+1);
				Achieve* info = nullptr;
				//检测痴狂相关成就
				info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangI);
				if (info && !info->isOver)
				{
					AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangI, 1);
				}
				info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangII);
				if (info && !info->isOver)
				{
					AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangII, 1);
				}
				info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangIII);
				if (info && !info->isOver)
				{
					AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangIII, 1);
				}
				info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_ChiKuangIV);
				if (info && !info->isOver)
				{
					AchievementHelper::getHelper()->completeOneAchievement(achieveID_ChiKuangIV, 1);
				}
			 GameDirector::getDirector()->loadingToRunningScene(RunningMode::endless);
		}else if(current_oprate==zhuye_ftag){
			//zhuye_ftag
			 current_oprate=0;
	         isdone=0;
			 if(fahuimark>1) return;
			 layeReturnpop();
		}else if(current_oprate==zb_b){
			
			 current_oprate=0;
			 isdone=0;
			 layerChange();
			 drawshoplayer();

		}else if (current_oprate == tollgateItem_tag){
			//layerChange();
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(MSG_TollgateLayer_comein);
		}
}


void Rolelayer::go_layer(int tag){

    current_oprate=0;
	isdone=0;

	EventCustom event(Layerchange);
	LayerData data;
	data.current=tag;
	data.push=0;
	event.setUserData(&data);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

	
}

void Rolelayer::allCallbackm(Ref* ref){

	
	   if(bingbi==1){
			return;
	   }else if(bingbi==0){

		   this->removeChildByTag(tishi_daotag);

	   }


	isdone=1;
	FX::Music::getInstance()->buttonClicked();
	MenuItemImage* p=(MenuItemImage *)ref;
	current_oprate=p->getTag();
	int va=(int)(long long)p->getUserData();

	if(current_oprate==click_tag2){

	}else{
		va=0;
	}

	if(current_oprate==magic_ab){
		current_oprate=magic_b;
	}else if(current_oprate==pet_ab){
		current_oprate=pet_b;
	}else if(current_oprate==hero_ab){
		current_oprate=hero_b;
	}else if(current_oprate==jian_mark){
		Sprite* p=(Sprite*)this->getChildByTag(layer_left_tag)->getChildByTag(tiao_sp);
		auto move = ScaleTo::create(0.5f, 0.0f);    
        auto move_ease_inout1 = EaseExponentialOut::create(move);
		p->runAction(move_ease_inout1);
	}else if(current_oprate==jiao_mark){
		Sprite* p=(Sprite*)this->getChildByTag(layer_left_tag)->getChildByTag(tiao_sp);
		auto move = ScaleTo::create(0.5f,1.0f);    
        auto move_ease_inout1 = EaseExponentialOut::create(move);
		p->runAction(move_ease_inout1);	
	}else if(current_oprate==zhuye_ftag){
		fahuimark++;
	}else if(current_oprate==startgame_tag){
		p->setCallback(nullptr);
	}else if(current_oprate==zb_b){
		p->setVisible(false);
	}else if (current_oprate==tollgateItem_tag){
		p->setVisible(false);
	}


	
	if(getUimove()==Isteachmove::teacher){
		if(current_oprate==hero_b){
			callsp(va);
		}
	}else{
	    callsp(va);
	}
}


void Rolelayer::getActorInfor(unsigned int actorId, string& arExportFile, string& armatureName, string& animationName)
{
	if (actorId == unitID_Actor1){
		arExportFile = "xiaoxiao_NG.ExportJson";
		armatureName = "xiaoxiao_NG";
		animationName = "xx_ng";
	}else if (actorId == unitID_Actor2){
		arExportFile = "xiaoxiao_NG.ExportJson";
		armatureName = "xiaoxiao_NG";
		animationName = "ald_ng";
	}else {
		CCASSERT(false, "");
	}
}
void Rolelayer::getPetInfor(unsigned int petId, string& arExportFile, string& armatureName, string& animationName)
{
	if (petId == unitID_PiPiZhu){
		arExportFile = "ppz_ng.ExportJson";
		armatureName = "pzz_ng";
		animationName = "pzz_ng";
	}else if (petId == unitID_XiXiaoYang){
		arExportFile = "xxy_ng.ExportJson";
		armatureName = "xxy_ng";
		animationName = "xxy_ng";
	}else if (petId == unitID_BuGuNiao){
		arExportFile = "bgn_ng.ExportJson";
		armatureName = "bgn_ng";
		animationName = "bgn_ng";
	}else if (petId == unitID_ZhanHongLong){
		arExportFile = "zhl_ng.ExportJson";
		armatureName = "zhl_ng";
		animationName = "zhl_ng";
	}else {
		CCASSERT(false, "");
	}
}
UnitType Rolelayer::getUnitTypeByArmatureName(string armatureName)
{
	if ("xiaoxiao_NG" == armatureName
		|| "xiaoxiao_NG" == armatureName){
			return unittype_Actor;
	}else if ("pzz_ng" == armatureName
		|| "xxy_ng" == armatureName
		|| "bgn_ng" == armatureName
		|| "zhl_ng" == armatureName){
			return unittype_Pet;
	}else if ("book_ui" == armatureName){
			return unittype_Magic;
	}else {
		CCASSERT(false, "unknown armature name");
		return unittype_None;
	}
}
void Rolelayer::playAnimationByname(Node *bs,std::string jsonfile,std::string filename,std::string animationName,bool isloop,bool isscal,Vec2 p){

	auto armature =Armature::create(filename.c_str());
	if(isloop==true){
		armature->getAnimation()->play(animationName.c_str(),-1,1);
	}
	armature->removeFromParentAndCleanup(true);
	armature->setPosition(p);
	armature->setTag(100);
	armature->setLocalZOrder(100);
	bs->addChild(armature);
	//xx_sly
	if(getUnitTypeByArmatureName(filename) == unittype_Actor){
		auto yinying_img=Sprite::create(hero_yingyin_img); //(hero_yingyin_img);
		yinying_img->setAnchorPoint(Vec2(0.5,0));
		yinying_img->setScaleX(1.5f);
		yinying_img->setPosition(Vec2(p.x,-armature->getContentSize().height/2+40));
		bs->addChild(yinying_img);
// 		xkongz.xx_action=armature;
// 		xkongz.time=0;
// 		xkongz.xxtime[0]=20;
// 		xkongz.xxtime[1]=1;
// 		xkongz.change=false;
	}
	else if(getUnitTypeByArmatureName(filename) == unittype_Pet){
		auto yinying_img=Sprite::create(hero_yingyin_img); //(hero_yingyin_img);
		yinying_img->setAnchorPoint(Vec2(0.5,0));
		yinying_img->setPosition(Vec2(p.x,-armature->getContentSize().height/2+30));
		bs->addChild(yinying_img);
	}
	else if(getUnitTypeByArmatureName(filename) == unittype_Magic){
		armature->setScale(0.5f);
	}
}

void Rolelayer::updateSz(float dt){
	 
	 
		
}

void Rolelayer::callfunc_AnimateEnd(Armature *ar, MovementEventType movementType, const std::string& movementID){
	ar->removeFromParent();
}

void Rolelayer::callfunc_AnimateEndGk(Armature *ar, MovementEventType movementType, const std::string& movementID){

/*	xkongz.time++;
	if(movementType==MovementEventType::COMPLETE){
        
	    ar->getAnimation()->play("xx_ng");
		ar->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Rolelayer::callfunc_AnimateEndGk, 
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		xkongz.time=0;

	}else if(movementType==MovementEventType::LOOP_COMPLETE){

		if(xkongz.time<xkongz.xxtime[0]){
			ar->getAnimation()->play("xx_ng");
			ar->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Rolelayer::callfunc_AnimateEndGk, 
				this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}else if(xkongz.time>=xkongz.xxtime[0]){
			ar->getAnimation()->play("xx_sly");
			ar->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(Rolelayer::callfunc_AnimateEndGk, 
				this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		    xkongz.time=0;
		}
	}*/
}

void Rolelayer::onEnter(){
	rigistdaoju();
	Layer::onEnter();
}

void Rolelayer::onExit(){

	//xkongz.xx_action->stopAllActions();
	this->stopAllActions();
	Director::getInstance()->getEventDispatcher()->removeEventListener(Data_changevent);
	Layer::onExit();
}


void Rolelayer::rigistdaoju(){

	auto callBack       =  [&](EventCustom* evt)  
                            {  
								 MsgData* data=(MsgData*)evt->getUserData();
                                 const  Unit* unit=GoodsHelper::getHelper()->getUnitByID(GoodsHelper::getHelper()->getRandomMarketItem());
								 if(rand_ig!=nullptr){
									 rand_ig->setVisible(true);
									 Label* p=(Label*)rand_ig->getChildByTag(rand_daojutag);
									 string ms=unit->descrigbe;
									 p->setString(ms);
									 p->runAction(Sequence::create(ScaleTo::create(0.1f,0.1f),ScaleTo::create(0.15f,1.0f),nullptr));
								 }
								 GoodsHelper::getHelper()->changeOneUnitCount(unit->ID,data->value);
							 };  
	Data_changevent=EventListenerCustom::create(Randxiaoguo,callBack);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(Data_changevent,1);  

}


Rolelayer::~Rolelayer(void)
{
         	          	
}


void Rolelayer::fillOuty(MenuItemImage* p,string imgfile){
	 
	auto s=Sprite::create(imgfile);
	s->setScale(1.05f);
	p->setSelectedImage(s);

}


void Rolelayer::setUiMove(int tag){

	 whatMove=tag;

}

int  Rolelayer::getUimove(){
	 
	 return whatMove;
}


/*

        当鼠标处于按下并曾经点中按钮时，则触发一次 
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchDownAction), CCControlEventTouchDown);    
        
        当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发一次    
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchDragEnter), CCControlEventTouchDragEnter);    
        
        当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发一次    
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchDragExit), CCControlEventTouchDragExit);    
        
        当鼠标处于按下并曾经点中按钮的状态下，鼠标进入按钮范围，则触发，只要达到条件，就不断触发    
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchDragInside), CCControlEventTouchDragInside);    
        
        当鼠标处于按下并曾经点中按钮的状态下，鼠标离开按钮范围，则触发，只要达到条件，就不断触发    
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchDragOutside), CCControlEventTouchDragOutside);    
        
        当鼠标处于按下并曾经点中按钮的状态下，鼠标松开且在按钮范围内，则触发一次  
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchUpInside), CCControlEventTouchUpInside);    
        
        当鼠标处于按下并曾经点中按钮的状态下，鼠标松开且在按钮范围外，则触发一次   
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchUpOutside), CCControlEventTouchUpOutside);    
        
        暂时没有发现能用鼠标触发这个事件的操作，看了注释，应该是由其它事件中断按钮事件而触发的  
        controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchCancel), CCControlEventTouchCancel);    

*/
