#include "NetmsgManger.h"
#include "PlayerNameFactory.h"
#include "ActiveValueHelper.h"
#include "RankingListHelper.h"
#include "BasicLayer.h"
#include "PlayerInfoHelper.h"
#include "BasicLayer.h"
#include "EmailHelper.h"
#include "cs_common.h"
#include "GameDirector.h"
#define  ipstr "192.168.2.10"

static  int  Dk=23577; 
static  int  weishu=16;

#include "net/NetworkPackageDispatch.h"
#include "net/NetworkThreadProc.h"


NetmsgManger::NetmsgManger(void)
{

	yinyue_state=false;
    yinxiao_state=false;
	texiao_state=false;
	value_num=0;
	mark=-1;
	islogin=false;
	errorrank=0;
	isdesdoy=true;
	isstart=false;
	NetworkPackageDispatch::getInstance()->setMsgProc(std::bind(&NetmsgManger::msgProc,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
}


NetmsgManger::~NetmsgManger(void)
{

}


/**
 * 静态单例变量初始化
 */
NetmsgManger* NetmsgManger::sharedNetmsgManger = nullptr;



NetmsgManger* NetmsgManger::create() {
	NetmsgManger *pRet = new NetmsgManger();
	return pRet;
}


NetmsgManger* NetmsgManger::getNetManager()
{
	if (! sharedNetmsgManger) {
	   NetmsgManger *pRet = new NetmsgManger();
	   sharedNetmsgManger=pRet;
	}
	sharedNetmsgManger->isdesdoy=true;
	return sharedNetmsgManger;
}

void  NetmsgManger::setAllsale(){

	      
	    
	    if(heros.size()>0) heros.clear();
		if(pets.size()>0) pets.clear();
		if(magis.size()>0) magis.clear();
		heros=ActorHelper::getHelper()->getAllHero();
		pets=ActorHelper::getHelper()->getAllPet();
		magis=ActorHelper::getHelper()->getAllMagic();
		

}

void NetmsgManger::linquDay(int warld_Id){

     GameDirector::getDirector()->showWaiting();
	 CoolRun::GetSystemBonusReq req = NetmsgManger::getNetManager()->getGetSystemBonusReq(warld_Id);
	 string data;
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_GetSystemBonus, data.c_str(), req.ByteSize());

}


void NetmsgManger::sendMail(){

	 GameDirector::getDirector()->showWaiting();
	 CoolRun::QueryMailReq req = NetmsgManger::getNetManager()->getQueryMailReq();
	 string data;
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_QueryMail, data.c_str(), req.ByteSize());
}

void NetmsgManger::sendLoginday(){

	GameDirector::getDirector()->showWaiting();
	CoolRun::QuerySystemBonusReq req = NetmsgManger::getNetManager()->getQuerySystemBonusReq();
	string data;
	req.SerializeToString(&data);
	NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_QuerySystemBonus, data.c_str(), req.ByteSize());

}


void NetmsgManger::updateUserinfo(){


    //GameDirector::getDirector()->showWaiting();
	string data;
	CoolRun::UpdateInfoReq req=NetmsgManger::getNetManager()->getupdateInfoReq();
	req.SerializeToString(&data);
	NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_UpdateInfo, data.c_str(), req.ByteSize());

}


void NetmsgManger::loadNetRank(){

	      
	 GameDirector::getDirector()->showWaiting();
	 string data;
	 CoolRun::QueryRankListReq req=NetmsgManger::getNetManager()->getQueryRankListReq();
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_QueryRankList, data.c_str(), req.ByteSize());

}

void NetmsgManger::loadNetboundsinfo(){

	 
	 GameDirector::getDirector()->showWaiting();
	 string data;
	 CoolRun::QueryRankBonusReq req=NetmsgManger::getNetManager()->getQueryRankBonusReq();
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_QueryRankBonus, data.c_str(), req.ByteSize());

}

//修改昵称
void NetmsgManger::updateNickname(string nick_name){

	
	 GameDirector::getDirector()->showWaiting();
	 string data;
	 CoolRun::AlterInfoReq req=NetmsgManger::getNetManager()->getAlterInfoReq(nick_name);
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_AlterInfo, data.c_str(), req.ByteSize());


}


void NetmsgManger::sendUpdateLibao(){

	 //CCLOG("--->%d",PlayerInfoHelper::getHelper()->getUserId());

	 GameDirector::getDirector()->showWaiting();
	 string data;
	 CoolRun::QueryGiftBagReq req=NetmsgManger::getNetManager()->getQueryGiftBagReq();
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_QueryGiftBag, data.c_str(), req.ByteSize());
	 
}

void NetmsgManger::isregist(){

	 
	

	 GameDirector::getDirector()->showWaiting();
	 string data;
	 CoolRun::RegistReq req=NetmsgManger::getNetManager()->getregistReq(CoolRun::PlatformType::PlatformType_Common);
	 req.SerializeToString(&data);
	 NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_Regist, data.c_str(), req.ByteSize());

}



void NetmsgManger::setisLogin(){

	 islogin=true;

}

//登陆操作
void NetmsgManger::Loginoparte(){


	
	//GameDirector::getDirector()->showWaiting();
	string data;
	CoolRun::LoginReq req=NetmsgManger::getNetManager()->getLoginReq();
	req.SerializeToString(&data);
	NetworkThreadProc::getInstance()->sendData(MessageCmd::Req_Login, data.c_str(), req.ByteSize());


}



CoolRun::LoginReq NetmsgManger::getLoginReq(){


	     CoolRun::LoginReq req;
		 req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
		 return req;

}


// 注册
CoolRun::RegistReq  NetmsgManger::getregistReq(CoolRun::PlatformType type,string acount,string name,string pass){


	
	CoolRun::RegistReq req;
	req.set_platform(type);
    PlayerNameFactory::getFactory();
	string nmae=PlayerInfoHelper::getHelper()->getPlayerName();//  PlayerNameFactory::getFactory()->getRandomName(true);

	if(name.size()==0){
		nmae=PlayerNameFactory::getFactory()->getRandomName(true);
		PlayerInfoHelper::getHelper()->setPlayerName(nmae);
	}
	req.set_nickname(nmae.c_str());
	PlayerNameFactory::getFactory()->destroyFactory();

	return req;
}


//排行榜 req
CoolRun::QueryRankListReq NetmsgManger::getQueryRankListReq(){

	    CoolRun::QueryRankListReq req;
		req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
		return req;

}



//更新用户信息 req
CoolRun::UpdateInfoReq  NetmsgManger::getupdateInfoReq(){

	unsigned int meter=0, score=0;
	PlayerInfoHelper::getHelper()->getThisWeekMaxScore(score, meter);

	setAllsale();
	CoolRun::UpdateInfoReq req;
	req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
	req.set_score(score);
	req.set_distance(meter);
	for(size_t i=0;i<heros.size();i++){

		CoolRun::UnitInfo* info=req.add_actor();
		info->set_type(heros.at(i).unit.ID);
		bool islock=ActorHelper::getHelper()->getIsUnlockItem(heros.at(i).unit.ID);
		info->set_is_activate(islock);
		bool isuse=false;
		if(PlayerInfoHelper::getHelper()->getDeafaultPet()==pets.at(i).unit.ID){
			isuse=true;
		}
		info->set_is_use(isuse);
		int level=1;
		if(GoodsHelper::getHelper()->getOneItem(heros.at(i).unit.ID)!=nullptr){
		   level= GoodsHelper::getHelper()->getOneItem(heros.at(i).unit.ID)->value;
	    }
		info->set_level(level);
		
	}


	for(size_t j=0;j<pets.size();j++){

		CoolRun::UnitInfo* info=req.add_pet();
		info->set_type(pets.at(j).unit.ID);
		bool islock=ActorHelper::getHelper()->getIsUnlockItem(pets.at(j).unit.ID);
		info->set_is_activate(islock);
		bool isuse=false;
		if(PlayerInfoHelper::getHelper()->getDeafaultPet()==pets.at(j).unit.ID){
			isuse=true;
		}
		info->set_is_use(isuse);
		int level=1;
	    if(GoodsHelper::getHelper()->getOneItem(pets.at(j).unit.ID)!=nullptr){
		   level=GoodsHelper::getHelper()->getOneItem(pets.at(j).unit.ID)->value;
	    }
		info->set_level(level);
	}


	for(size_t k=0;k<magis.size();k++){

		CoolRun::UnitInfo* info=req.add_magic();
		info->set_type(magis.at(k).unit.ID);
		bool islock=false;
	    BagItem* bag= (BagItem*)GoodsHelper::getHelper()->getOneItem(magis.at(k).unit.ID);
	    if(bag!=nullptr){
		   islock=true;
	    }
		info->set_is_activate(islock);

	    bool isuse=false;
		if(PlayerInfoHelper::getHelper()->getDeafaultMagicItem()==magis.at(k).unit.ID){
			isuse=true;
		}
		info->set_is_use(isuse);

	}
    
	return req;

}


CoolRun::QuerySystemBonusReq NetmsgManger::getQuerySystemBonusReq(){


	     CoolRun::QuerySystemBonusReq req;
		 req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
		 return req;

}


CoolRun::GetSystemBonusReq   NetmsgManger::getGetSystemBonusReq(int wardId){

	   CoolRun::GetSystemBonusReq req;
	   req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
	   req.set_id(wardId);
	   return req;

}


CoolRun::QueryMailReq  NetmsgManger::getQueryMailReq(){

	   CoolRun::QueryMailReq req;
	   req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
	   return req;

}


CoolRun::ReadMailReq  NetmsgManger::getReadMailReq(int ID){
	   
	   CoolRun::ReadMailReq req;
	   req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
	   req.set_id(ID);
	   return req;

}


CoolRun::QueryGiftBagReq  NetmsgManger::getQueryGiftBagReq(){

	   CoolRun::QueryGiftBagReq req;
	   req.set_version(RandomGiftBagHelper::getHelper()->getVersion());
	   return req;


}



void NetmsgManger::load_data(){

	loadNetRank();
}




void NetmsgManger::msgProc(int err, unsigned short type, void* data,unsigned int size){

	
	if(type!=Rsp_Login||type!=Rsp_UpdateInfo){
	    GameDirector::getDirector()->hideWaiting();
	}

	if (err != 0)
	{
		//MessageBox("Network request error !", "Error");
		return;
	}

	if(type==Rsp_AlterInfo){

		//通知发送成功
		CoolRun::AlterInfoRsp rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		if(tag==MessageErrorCode::Success){

			BagDataManager::getManager()->changeOneUnitCount(unitID_Diamond,-50);
			PlayerInfoHelper::getHelper()->setPlayerName(NetmsgManger::getNetManager()->playername);
			auto layer=BasicLayer::create();
			layer->shuoming(FX::StringsMap::getValue("xgcg"));
			Director::getInstance()->getRunningScene()->addChild(layer,1025);	
		}
	}else if(type==Rsp_Regist){
		CoolRun::RegistRsp rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		if(tag==MessageErrorCode::Success&&rsp.has_user_id()){
			PlayerInfoHelper::getHelper()->setUserId(rsp.user_id());
		    MsgCenter(1);
		}else if(tag==MessageErrorCode::Error){
			MsgCenter(0);
		}
	}else if(type==Rsp_QueryRankList){

		    CoolRun::QueryRankListRsp rsp;
			rsp.ParseFromArray(data,size);
			int tag=rsp.rsp_code();
		    int size=rsp.user_list_size();
			
			if(tag==MessageErrorCode::Success&&rsp.user_list_size()>0){

				
				if(netSports.size()>0){
					netSports.clear();
				}
				
				if(rsp.has_cur_group()){
					msgNet.cur_group=rsp.cur_group();//在那各组
				}
				
				for(int i=0;i<rsp.user_list_size();i++){

					RankUser  msg;
					CoolRun::RankUserInfo  userinfo=rsp.user_list(i);
					msg.user_id=userinfo.user_id();
					msg.user_order=userinfo.user_order();//排名

					msg.distance=userinfo.distance();
					msg.score=userinfo.score();

					if(userinfo.has_user_name()){
					  msg.user_name=userinfo.user_name();
					  if(msg.user_name.compare(PlayerInfoHelper::getHelper()->getPlayerName())){
						  msgNet.last_order=msg.user_order;
					  }
					}

					CoolRun::UnitInfo info_hr;
					info_hr=userinfo.actor();
						UnitLayer actorlayer;
						actorlayer.is_activate=info_hr.is_activate();
						actorlayer.is_use=info_hr.is_use();
						actorlayer.level=info_hr.level();
						actorlayer.type=info_hr.type();
					msg.hero=actorlayer;

					CoolRun::UnitInfo info_pt;
					if(userinfo.has_pet()){
					info_pt=userinfo.pet();

						UnitLayer petlayer;
						petlayer.is_activate=info_pt.is_activate();
						petlayer.is_use=info_pt.is_use();
						petlayer.level=info_pt.level();
						petlayer.type=info_pt.type();
					msg.pet=petlayer;
					}
					CoolRun::UnitInfo info_mg;
					if(userinfo.has_magic()){
					info_mg=userinfo.magic();
					    UnitLayer mglayer;
						mglayer.is_activate=info_mg.is_activate();
						mglayer.is_use=info_mg.is_use();
						mglayer.level=info_mg.level();
						mglayer.type=info_mg.type();
					msg.magic=mglayer;
					}
				 netSports.push_back(msg);
				 
			    }
				MsgCenter(2);
				errorrank=0;
				mark=1;
	    }else{
			   MsgCenter(3);
	    }

		
	}else if(type==Rsp_QueryRankBonus){

		    CoolRun::QueryRankBonusRsp rsp;
	        rsp.ParseFromArray(data,size);
	        int tag=rsp.rsp_code();
			int size=rsp.bonus_size();
	    if(tag==MessageErrorCode::Success&&rsp.bonus_size()>0){
		  for(int i=0;i<rsp.bonus_size();i++){
			  RBonusInfo r_info;
			  AInfo   r_ainfo;
			  CoolRun::RankBonusInfo info=rsp.bonus(i);
			  r_info.id=info.id();
			  if(info.has_group_id()){
				  r_info.group_id=info.group_id();
			  }
			  if(info.has_group_name()){
				  r_info.group_name=info.group_name();
			  }
			  if(info.has_bonus_id()){
				  r_info.bonus_id=info.bonus_id();
			  }
			  if(info.has_attach()){
				  CoolRun::AttachInfo ainfo;
				  	       ainfo=info.attach();
						   if(ainfo.item_size()>0){
							   for(int j=0;j<ainfo.item_size();j++){
								   CoolRun::ItemInfo cr_info;
								   cr_info=ainfo.item(j);
								   ItInfo layer_info;
								   layer_info.type=cr_info.type();
								   layer_info.count=cr_info.count();
								   r_ainfo.items.push_back(layer_info);
							   }
						   }
						   r_info.attach=r_ainfo;
			  }
			  bounds.push_back(r_info);
		  }
		  reiveMsg(iswhoLoad,1);
		  //弹出奖励对话框
          
		  

		  
	  }else{
		  reiveMsg(iswhoLoad,0);
	  }

	}else if(type==Rsp_AlterInfo){ //修改昵称

		CoolRun::AlterInfoRsp rsp;
		rsp.ParseFromArray(data,size);
	    int tag=rsp.rsp_code();
		if(tag==MessageErrorCode::Success){
			MsgCenter(1);
		}else{
            MsgCenter(0);
		}

	}else if(type==Rsp_QueryGiftBag){// 获取礼包数据
       
		CoolRun::QueryGiftBagRsp  rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		if(tag==MessageErrorCode::Success&&rsp.has_is_update()&&rsp.is_update()==true){

			

			 for(int i=0;i<rsp.gift_bag_size();i++){

				 CoolRun::GiftBagInfo b_ginfo=rsp.gift_bag(i);

				 for(int j=0;j<b_ginfo.attach_size();j++){

					   CoolRun::GiftBagAttach  atch=b_ginfo.attach(j);
					   int qz=atch.weight();
					   BagItem item;
					   if(atch.has_attach()){

						   CoolRun::AttachInfo  a_if=atch.attach();
						   if(a_if.item_size()>0){
						      CoolRun::ItemInfo  it_if=a_if.item(0);
							  item.id=it_if.type();
							  item.value=it_if.count();
						   }
					   }
					   pair<BagItem, unsigned int> p_qitem;
					   p_qitem.first=item;
					   p_qitem.second=qz;
					   lbs.push_back(p_qitem);
				 }
			 }
			 RandomGiftBagHelper::getHelper()->setGiftBagTable(lbs);
			 
		}else{
			 
		}
	}else if(type==Rsp_QuerySystemBonus){

		CoolRun::QuerySystemBonusRsp rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		int size=rsp.bonus_size();
		if(tag==MessageErrorCode::Success&&rsp.bonus_size()>0){
		
			for(int i=0;i<rsp.bonus_size();i++){

				CoolRun::BonusInfo bounds=rsp.bonus(i);
				Warld ward;
				ward.id=bounds.id();
				ward.is_get=bounds.is_get();
				ward.enable=bounds.enable();
				if(bounds.has_attach()){
				  CoolRun::AttachInfo ainfo=bounds.attach();
				  if(ainfo.item_size()>0){
					  CoolRun::ItemInfo  iteminfo=ainfo.item(0);
					  ward.type=iteminfo.type();
					  ward.count=iteminfo.count();
				  }
				}
				wards.push_back(ward);
			}

			sendMsg(Logoinmsg,5);
			//MsgCenter(5); 

		}else{
			sendMsg(Logoinmsg,4);
			//MsgCenter(4); 
		}
		

	}else if(type==Rsp_GetSystemBonus){
		//解析返回数据
		CoolRun::GetSystemBonusRsp rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		//领取成功
		if(tag==MessageErrorCode::Success){
			MsgCenter(7);
			//活跃值统计
			ActiveValueHelper::getHelper()->addActiveByType(activeType_LingQuDengLuJiangLi);
			//天数统计
			PlayerInfoHelper::getHelper()->addLoginDays();

			Achieve* info = nullptr;
			//检测加油!成就
			info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_JiaYou);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_JiaYou, 1);
			}

			//检测团长成就
			info = (Achieve*)AchievementHelper::getHelper()->getAchieveByID(achieveID_TuanZhang);
			if (info && !info->isOver)
			{
				AchievementHelper::getHelper()->completeOneAchievement(achieveID_TuanZhang, 1);
			}

		}else{
			MsgCenter(6);
		}
	}else if(type==Rsp_QueryMail){

		CoolRun::QueryMailRsp rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		int size=rsp.email_size();
		if(tag==MessageErrorCode::Success&&rsp.email_size()>0){

			if(mails.size()>0){
				mails.clear();
			}
			  for(int i=0;i<rsp.email_size();i++){

				  MailInfo_layer info;
			      MailType_layer tp;

				  CoolRun::MailInfo c_info=rsp.email(i);
				            info.is_read=c_info.is_read();
							info.id=c_info.id();
							info.addresser_id=c_info.addresser_id();
								CoolRun::MailType c_type;
								c_type=c_info.type();
								/*  MailType_Invalid = 0, MailType_Bonus = 1,MailType_Notice = 2, MailType_Friend = 3 */
								if(c_type==CoolRun::MailType::MailType_Invalid){
									tp=MailType_layer::MailType_Invalid;
								}else if(c_type==CoolRun::MailType::MailType_Bonus){
									tp=MailType_layer::MailType_Bonus;
								}else if(c_type==CoolRun::MailType::MailType_Notice){
									tp=MailType_layer::MailType_Notice;
								}else if(c_type==CoolRun::MailType::MailType_Friend){
									tp=MailType_layer::MailType_Friend;

								}
								info.type=tp;
								if(c_info.has_addresser()){
									info.addresse=c_info.addresser();
								}
								if(c_info.has_content()){
									info.content=c_info.content();
								}
								if(c_info.has_attach()){
									AttachInfo_layer ainfo;
									CoolRun::AttachInfo c_ainfo;
									         c_ainfo=c_info.attach();
											 if(c_ainfo.has_vitality()){
												 ainfo.vitality=c_ainfo.vitality();
											 }
											 for(int j=0;j<c_ainfo.item_size();j++){
												  CoolRun::ItemInfo c_if;
												  c_if=c_ainfo.item(j);
													  ItemInfo_layer ifo_item;
													  ifo_item.type=c_if.type();
													  ifo_item.count=c_if.count();
													  ainfo.items.push_back(ifo_item);
											 }
											 info.attach=ainfo;
								}
								mails.push_back(info);
			  }
            
			 for(size_t i=0;i<mails.size();i++){

				 Email emails;

				 emails.isAlreadyRead=mails.at(i).is_read;
				 emails.id=mails.at(i).id;
				 emails.content=mails.at(i).content;
				 if(mails.at(i).attach.items.size()>0){
				    emails.bonus_unitID=mails.at(i).attach.items.at(0).type;
					emails.bonusCount=mails.at(i).attach.items.at(0).count;
				 }
				 emails.recvTime=getCurrenttime();

				 switch (mails.at(i).type)
				 {
					 case 0:
						 emails.type=Email::ET_Invalid;
						 break;
					 case 1:
						 emails.type=Email::ET_SystemBonus;
						 break;
					 case 2:
						 emails.type=Email::ET_SystemNotice;
						 break;
					 case 3:
						 emails.type=Email::ET_FriendChat;
						 break;
				 }
				 EmailHelper::getHeper()->addEmail(emails);

			 }
			 MsgCenter(3);
			 EmailHelper::getHeper()->save();
			 EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		     EventCustom event(Youjiantx);
		     eventDspc->dispatchEvent(&event);

		}else{
			 MsgCenter(4);
		}
		
	}else if(type==Rsp_ReadMail){


		 
	}else if(type==Rsp_Login){

           CoolRun::LoginRsp rsp;
		   rsp.ParseFromArray(data,size);
		   int tag=rsp.rsp_code();
		   if(tag==MessageErrorCode::Success){

			   NetmsgManger::getNetManager()->islogin=true;
			   setisLogin();
			   if(rsp.has_new_mail()){
				   //faxiangxi 有未读邮件
				   EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
		           EventCustom event(Youjiantx);
		           eventDspc->dispatchEvent(&event);
			   }
			   if(rsp.has_last_group()){
				   int lastgr=rsp.last_group();
				   msgNet.last_group=lastgr;
				   if(rsp.has_last_order()){
					   int last_o=rsp.last_order();
					   int mc=CS::get_rank_bonus_id(last_o);
					   msgNet.last_order=CS::get_rank_bonus_id(last_o);
					   RankingData data= RankingListHelper::getHelper()->getOneRankingData(lastgr,mc);
					   auto layer=BaseLayer::create();
					   BagItem item;
					   item.id=data.bonus_unitId;
					   item.value=data.bonusCount;
					   layer->active_warldbox(item);

					   Director::getInstance()->getRunningScene()->addChild(layer);  
				   }
			   }
			   if(PlayerInfoHelper::getHelper()->getIsUpdate(Update_Score)==false){
			      NetmsgManger::getNetManager()->updateUserinfo();
			   }
		   }else{

		   }

	}else if(type==Rsp_UpdateInfo){

		//通知发送成功
		CoolRun::UpdateInfoRsp rsp;
		rsp.ParseFromArray(data,size);
		int tag=rsp.rsp_code();
		if(tag==MessageErrorCode::Success){
			PlayerInfoHelper::getHelper()->setUpdate(Update_Score);
		}
	}
}



CoolRun::QueryRankBonusReq NetmsgManger::getQueryRankBonusReq(){
       
	CoolRun::QueryRankBonusReq req;
	req.set_stub(PlayerInfoHelper::getHelper()->getUserId());
	return req;
}

//获得我的奖励
ItInfo NetmsgManger::get_myItinfo(){

	   vector<RankingData> m_vecRankingList=RankingListHelper::getHelper()->getAllRankingList();
	   ItInfo ifo;
	   for(size_t i=0;i<m_vecRankingList.size();i++){
		   if(msgNet.last_group==m_vecRankingList.at(i).groupId){
			   ifo.count=m_vecRankingList.at(i).bonusCount;
			   ifo.type=m_vecRankingList.at(i).bonus_unitId;
			break;
		}
	}
	  
	return ifo;
}


//得到自己的组名
string NetmsgManger::getMy_name(){

	vector<RankingData> m_vecRankingList=RankingListHelper::getHelper()->getAllRankingList();
	string name;
	for(size_t i=0;i<m_vecRankingList.size();i++){
		if(NetmsgManger::getNetManager()->msgNet.cur_group==m_vecRankingList.at(i).groupId){
			name=m_vecRankingList.at(i).describe;
			break;
		}
	}
	return name;
}


void NetmsgManger::reiveMsg(int tag,int error){

	 
	 
	 
	 
	  

}

CoolRun::AlterInfoReq NetmsgManger::getAlterInfoReq(string nickname){

	   CoolRun::AlterInfoReq req;
	   req.set_user_id(PlayerInfoHelper::getHelper()->getUserId());
	   req.set_nickname(nickname.c_str());
	   return req;

	   //PlayerInfoHelper::getHelper()->setUserId(40);
}



void NetmsgManger::loadCsdata(){


	cs.push_back(1);
	cs.push_back(2);
	cs.push_back(3);
	cs.push_back(5);
}


void NetmsgManger::Czdata(){

	 
}


int NetmsgManger::IsLeapYear(){

	if((year%4==0&&year%100!=0)||(year%400==0)) 
		return 1; 
	else 
	    return 0; 

}

int NetmsgManger::month_day(int m) 
{ int mon_day[]={31,28,31,30,31,30,31,31,30,31,30,31};
  if(IsLeapYear()&&m==2) 
	  return 29; 
  else 
	  return(mon_day[m-1]);
} 


int NetmsgManger::DaySearch(){ 

	 //获得年月日
	struct tm *tm;  
	time_t timep; 

	//time(null);
	
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	   time(&timep); 
    #else
	    struct timeval now;    
		gettimeofday(&now, NULL); 
	    timep=now.tv_sec; 
    #endif

        tm = localtime(&timep); 
	    
        int year = tm->tm_year + 1900; 
        int month = tm->tm_mon + 1; 
        int day = tm->tm_mday; 
        int hour=tm->tm_hour; 
        int min=tm->tm_min; 
        int second=tm->tm_sec; 

      
		this->year=year;
		this->month=month;
		this->day=day;

		

	 int hour_s;
	 vector<int> str;
	 str.push_back(0);
     str.push_back(1);
	 str.push_back(2);
	 str.push_back(3);
	 str.push_back(4);
	 str.push_back(5);
	 str.push_back(6);
	 
	 int c=0; 
	 float s; 
	 int m; 
	 for(m=1;m<month;m++) 
		 c=c+month_day(m);

	     c=c+day; 
		 s=year-1+(float)(year-1)/4+(float)(year-1)/100+(float)(year-1)/400-40+c; 
		int mn= str.at((int)s%7);
	if(mn==0){
		mn=7;
	}
    hour_s=24*(7-mn)+24-hour;

	//string time="";
	//time=CCString::createWithFormat("%d-%d-%d %d:%d:%d",year,month,day,hour,min,second)->getCString();

	
	return hour_s;
} 

//测试

void NetmsgManger::Csmsgis_null(){

	 if(msg_c){
		 
	 }else{
		 
	 }

}

void NetmsgManger::setMsgc_null(){
	msg_c=nullptr;
	isdesdoy=false;
}


 unsigned int NetmsgManger::feizhuangoneint(unsigned int id,unsigned int value){

	  unsigned int k1=0;
	  unsigned int k2=0;
	  for(int i=0;i<32;i++){
		if(i<weishu){
		  k1=id;
		  k1=k1>>i;
		  k1=k1&1;
		  k2+=pow(2,i)*k1;
		}else{
		  k1=value;
		  k1=k1>>(i-weishu);
		  k1=k1&1;
		  k2+=pow(2,i)*k1;
		}
	 }
	 return k2;
 }


 MsgData  NetmsgManger::getUnitdata(int he){
	      MsgData data;
	      unsigned int m3=0;
	      unsigned int m4=0;
		  unsigned int k1=0;
	      unsigned int k2=he;
		  for(int i=0;i<32;i++){
				k1=k2;
				if(i<16){
					  k1=k1>>i;
					  k1=k1&1;
					  m3+=pow(2,i)*k1;
				}else{
					  k1=k1>>i;
					  k1=k1&1;
					  m4+=pow(2,(i-16))*k1;
				}
		  }
		  data.errorCode=m3;
		  data.value=m4;
		  return data;
 }    


 int NetmsgManger::getCurrenttime(){


	 /*
	struct tm *tm;  
	time_t timep; 

	//time(null);
	
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	   time(&timep); 
    #else
	    struct timeval now;    
		gettimeofday(&now, NULL); 
	    timep=now.tv_sec; 
    #endif

        tm = localtime(&timep); 
	    
        int year = tm->tm_year + 1900; 
        int month = tm->tm_mon + 1; 
        int day = tm->tm_mday; 
        int hour=tm->tm_hour; 
        int min=tm->tm_min; 
        int second=tm->tm_sec;

		int md=0;
		for(int i=1;i<month;i++){

			md+=GetDaysInMonth(year,i);
		}
		int m=day+md+year*365;
		*/
	 int m=time(0);
	

	 return m;
 }

 bool NetmsgManger::chaochuSevenday(int time1,int time2){

	 if((time1-time2)/60/60/24>7){
		 return true;
	 }else{
		 return false;
	 }

 }


 int NetmsgManger::GetDaysInMonth(int y,int m)
{
    int d;
    int day[]= {31,28,31,30,31,30,31,31,30,31,30,31};
    if (2==m)
    {
        d=(((0==y%4)&&(0!=y%100)||(0==y%400))?29:28);
    }
    else
    {
        d=day[m-1];

    }
    return d;
}

 /*

  EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
					      EventCustom event(Randxiaoguo);
	                      MsgData data;
						  data.value =tag_id;
	                      event.setUserData(&data);
	                      eventDspc->dispatchEvent(&event);


 */

 void NetmsgManger::sendMsg(std::string type,int mark){

	   EventDispatcher * eventDspc = Director::getInstance()->getEventDispatcher();
	   EventCustom event(type);
	   MsgData data;
	   data.value =mark;
	   event.setUserData(&data);
	   eventDspc->dispatchEvent(&event);

 }