#include "UnitActionHelper.h"
#include "ActionConfigManager.h"
#include "TriggerConfigManager.h"
#include "ObstacleSprite.h"
#include "RunningHelper.h"
#include "GameUnitLayer.h"
#include "HeroSprite.h"
#include "FXJump.h"
#include "VersionConfig.h"
#include "BossSprite.h"
#include "StateBar.h"
#include "Font.h"
using namespace cocos2d;

// UnitActionHelper* UnitActionHelper::m_sUnitHelper;
// 
// UnitActionHelper::UnitActionHelper()
// {
// 
// }
// 
// UnitActionHelper* UnitActionHelper::getHelper()
// {
// 	if (! m_sUnitHelper) m_sUnitHelper = new UnitActionHelper;
// 	return m_sUnitHelper;
// }

void UnitActionHelper::runActionByTrigger(cocos2d::Node* target, const Trigger* trigger)
{
	if (!trigger)
	{
		return;
	}
	if (trigger->actionID != action_None)
		runActionByActionId(target, trigger->actionID, trigger);
}

void UnitActionHelper::runActionByActionId(cocos2d::Node* target, unsigned int actionID, const Trigger* trigger /*= nullptr*/)
{
	float framePreTime = Director::getInstance()->getAnimationInterval();
	Size winSize = Director::getInstance()->getWinSize();
	const ActionItem *actInfo = ActionConfigManager::getManager()->getActionItemByID(actionID);
	switch (actionID)
	{
	case action_FeiXingBi:
		{

		}
		break;
	case action_AiShuZhangChu:
		{

		}
		break;
	case action_GaoShuZhangChu:
		{

		}
		break;
	case action_YeZhuHuXi:
		{
			target->runAction(RepeatForever::create(
				Sequence::create(
				Spawn::create(ScaleTo::create(25*framePreTime, 1.0f, 1.06f), MoveBy::create(25*framePreTime, Vec2(0, 3)), nullptr),
				Spawn::create(ScaleTo::create(35*framePreTime, 1.0f, 1.0f), MoveBy::create(25*framePreTime, Vec2(0, -3)), nullptr),
				nullptr)
				));
		}
		break;
	case action_HouZiHuXi:
		{
			float t = 8*framePreTime;
			RepeatForever* action = RepeatForever::create(
				Sequence::create(
				Spawn::create(ScaleBy::create(t*2, 1.1f, 0.9f), Spawn::create(RotateBy::create(t,1),RotateBy::create(t,-1),nullptr), nullptr),
				Spawn::create(ScaleBy::create(t*2, 1/1.1f, 1/0.9f), Spawn::create(RotateBy::create(t,-1),RotateBy::create(t,1),nullptr), nullptr),
				nullptr)
				);
			action->setTag(action_HouZiHuXi);
			target->runAction(action);
		}
		break;
	case action_ShiTouFeiXing:
		{

		}
		break;
	case action_MiFengFeiXing:
		MessageBox("Not Support this Action", "Error");
// 		{
// 			char name[64] = {};
// 			Animation* pAnimation = Animation::create();
// 			for (unsigned int i = 0; i < 4; ++i)
// 			{
// 				sprintf(name, "daoju_huxi_mifeng_%02d.png", i+1);
// 				pAnimation->addSpriteFrameWithFile(name);
// 			}
// 			pAnimation->setDelayPerUnit(framePreTime);
// 			pAnimation->setLoops(-1);
// 			target->runAction(Animate::create(pAnimation));
// 		}
		break;

	case action_ChongJi://(触发器动画)//冲击、飞出来：蜜蜂、导弹触发后飞出来，从右到左
		{
			target->runAction(MoveBy::create(5.0f, Vec2(-1280, 0)));
		}
		break;

	case action_Gun:
		{
			target->runAction(RepeatForever::create(RotateBy::create(5.0f,-360.0f)));
		}
		break;
	case action_GunDong:
		{
			ObstacleSprite* pObs = dynamic_cast<ObstacleSprite*>(target);
			if (pObs)
			{
				//actInfo->value  1帧滚动的像素
				float r2;
				if (pObs->getSelfUnit()->rect.size() > 0) r2 = pObs->getSelfUnit()->rect[0].size.width;
				else r2 = pObs->getContentSize().width;
				float distancePerSecond = actInfo->value / framePreTime;  //1s滚动的距离
				float timePerRoll = 3.14159f * r2 / distancePerSecond;  //滚一圈的时间  t = PI*2R / v
				float rotatePerSecond = -360 / timePerRoll;
				RepeatForever* action = RepeatForever::create(Spawn::create(MoveBy::create(1.f,Vec2(-distancePerSecond,0.0f)),RotateBy::create(1.f,rotatePerSecond),nullptr));
				action->setTag(action_GunDong);
				pObs->runAction(action);
				pObs->schedule(schedule_selector(ObstacleSprite::tick_StoneScroll));
			}
		}
		break;
	case action_LuMianTaXian:
		{
			target->runAction(Sequence::create(DelayTime::create(0.3f) ,MoveBy::create(0.3f,Vec2(0,-g_fGridWidth * 12)), nullptr));
		}
		break;
	case action_BossMake:
		{
			BossSprite* pBoss = RunningHelper::getHelper()->getGameUnitLayer()->getBoss();
			if (! pBoss || target->isVisible()) return;
			if (pBoss->getBossHP() <= 0) return;

			if (pBoss->getBossState() == BossSprite::BS_Normal)
			{
				bool hasHit = RunningHelper::getHelper()->getGameUnitLayer()->getBoss()->hit();
				if (! hasHit) //只有攻击状态才会攻击
				{
					break;
				}

				Node* pNode = Node::create();
				ParticleSystemQuad* p = ParticleSystemQuad::create("diaoluoxiaoguo.plist");
				p->setPosition(Vec2(60, 20));
				pNode->addChild(p);
				pBoss->addChild(pNode);
				pNode->setVisible(false);

				float dsy =  pBoss->getParent()->convertToWorldSpace(pBoss->getPosition()).y - dynamic_cast<ObstacleSprite*>(target)->getCollisonRectInWorldSpace().getMidY();

				int x = 5;
				if (trigger)
				{
					x = trigger->distance;
				}

				float dt = 0.3f;
				float dsx =  x*g_fGridWidth + dt*RunningHelper::getHelper()->getGameUnitLayer()->getScrollWidth() + 150;

				target->retain(); // 如果boss里hero发送了冲击波，这时候target会被清掉， lamda里面访问target就会崩。
				pNode->runAction(Sequence::create(
					Show::create(),
					Spawn::create(FX::ActionJumpDown::create(dt, -dsy),
					MoveBy::create(dt, Vec2(-dsx, 0)),
					CallFunc::create([=](){
						Armature *ar = Armature::create("effect");
						if (ar){
							target->addChild(ar);
							ar->setPosition(target->getContentSize()/2);
							ar->getAnimation()->setMovementEventCallFunc([ar](Armature *armature, MovementEventType movementType, const std::string& movementID){
								if (movementID == "e_wujianshengcheng" && movementType == COMPLETE) ar->removeFromParent();
							});
							ar->getAnimation()->play("e_wujianshengcheng");
						}
						target->runAction(Sequence::create(
							DelayTime::create(dt),
							Show::create(),
							ScaleTo::create(0.1f, 1.0f),
							nullptr));
						//target->setVisible(true);
						target->release();// 如果boss里hero发送了冲击波，这时候target会被清掉， lamda里面访问target就会崩。
				}),
					nullptr),
					RemoveSelf::create(),
					nullptr));
			}
			else if (pBoss->getBossState() == BossSprite::BS_Rest)
			{
				// boss lightning
				Armature *ar = Armature::create("effect");
				pBoss->addChild(ar, 10);
				ar->setPosition(-55, 153);
				ar->getAnimation()->play("e_shangdian");
				ar->getAnimation()->setMovementEventCallFunc([target,winSize](Armature *armature, MovementEventType movementType, const std::string& movementID){
					if (movementID == "e_shangdian" && movementType == COMPLETE) armature->removeFromParent();
				});


				//target
				target->runAction(Sequence::create(
					DelayTime::create(0.3f),
					CallFunc::create([target,winSize](){

						//target lightning
						Armature *arShanDian = Armature::create("effect");
						target->addChild(arShanDian, 10);
						arShanDian->setPosition(target->getContentSize().width/2, winSize.height-200);
						arShanDian->getAnimation()->play("e_shangdian");
						arShanDian->runAction(
							MoveTo::create(0.3f, Vec2(target->getContentSize().width/2, target->getContentSize().height+60)));
						arShanDian->getAnimation()->setMovementEventCallFunc([](Armature *armature, MovementEventType movementType, const std::string& movementID){
							if (movementID == "e_shangdian" && movementType == COMPLETE) armature->removeFromParent();
						});

						//target apear
						Armature *arChuXian = Armature::create("effect");
						if (arChuXian){
							target->addChild(arChuXian);
							arChuXian->setPosition(target->getContentSize()/2);
							arChuXian->getAnimation()->play("e_wujianshengcheng");
							arChuXian->getAnimation()->setMovementEventCallFunc([](Armature *armature, MovementEventType movementType, const std::string& movementID){
								if (movementID == "e_wujianshengcheng" && movementType == COMPLETE) armature->removeFromParent();
							});
						}

				}),
					DelayTime::create(0.25f),
					Show::create(),
					ScaleTo::create(0.1f, 1.0f),
					nullptr));
			}
		}
		break;
	case action_LuMianShangsheng:
		{
			target->runAction(Sequence::create(MoveBy::create(0.3f,Vec2(0,g_fGridWidth * 5)), nullptr));
		}
		break;
// 	case action_ZhongDu:
// 		{
// 			RunningHelper::getHelper()->getHelper()->getRunnerHero()->setZhongDuState(true);
// 		}
// 		break;
	case action_XiaoXiTiShi:
		{
			if (trigger)
			{
				Sprite* bg = Sprite::create("UI_jiaose_anniu_shengji_manji.png");
				bg->setPosition(Vec2(winSize.width - bg->getContentSize().width/2 - 20, winSize.height/2 + 100));

				Label* pLabel = Label::createWithSystemFont(trigger->describe, Font_TTF_KaTong, 24, Size(130, 60), TextHAlignment::LEFT);
				bg->addChild(pLabel);
				pLabel->setColor(Font_Color_BaseColor);
				pLabel->setPosition(Vec2(bg->getContentSize().width/2 + 15, bg->getContentSize().height/2 - 10));

				RunningHelper::getHelper()->getCurrentStateBar()->addChild(bg, 20);	

				bg->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(1.2f), FadeOut::create(0.2f),  RemoveSelf::create(), nullptr));
			}
		}
		break;

	case action_XuanFuWu_Up:
		{
			target->runAction(Sequence::create(MoveBy::create(0.25f,Vec2(0,g_fGridWidth * 3)), nullptr));
		}
		break;
	case action_XuanFuWu_Down:
		{
			target->runAction(Sequence::create(MoveBy::create(0.25f,Vec2(0,-g_fGridWidth * 3)), nullptr));
		}
		break;
	case action_XianDianLianUpDown:
		{
			MoveBy* moveby = MoveBy::create(2.2f, Vec2(0, g_fGridWidth*3));
			target->runAction(RepeatForever::create(Sequence::create(moveby, moveby->reverse(), nullptr)));
		}
		break;

	case action_ZhongDuJieYaoPingZhiShanSuo:
		{
			Sprite* pJianTou = Sprite::create("UI_zhuye_shangdian_fanye_xia.png");
			target->addChild(pJianTou);
			pJianTou->setPosition(Vec2(target->getContentSize().width/2, target->getContentSize().height + pJianTou->getContentSize().height/2));
			pJianTou->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.2f), FadeOut::create(0.2f), nullptr)));
		}
		break;
	case action_JiaoXue_LongHuXi:
		{
			ObstacleSprite* pObs = dynamic_cast<ObstacleSprite*>(target);
			if (pObs)
			{
				pObs->initWithFile("0none.png");
				Armature* pLong = Armature::create("long");
				pObs->addChild(pLong);
				pLong->getAnimation()->play("m_daiji");
			}
		}
		break;

	case action_PenDu:
		{
			target->setOpacity(0);
			Armature* pDuQi = Armature::create("pengdu");
			target->addChild(pDuQi);
			pDuQi->getAnimation()->play("pendu");
			pDuQi->setPosition(Vec2(target->getContentSize().width/2, target->getContentSize().height/2 ));
		}
		break;
	case action_ScaleBy2:
		{
			target->stopActionByTag(action_HouZiHuXi);
			target->retain();
			target->runAction(Sequence::create(
				ScaleTo::create(0.25f,2/target->getScaleX()),
				CallFunc::create([target](){
					UnitActionHelper::runActionByActionId(target,action_HouZiHuXi);
					target->release();
			}),
				nullptr));
		}
		break;
	case action_FadeOut:
		{
			target->runAction(FadeOut::create(0.25f));
		}
		break;
	default:
		{
			CCLOG("Warrior: %s ---> no this action (id = %d)", __FUNCTION__, actionID);
//			CCASSERT(false, "");
		}
		break;
	}
}
//void UnitActionHelper::runActionByTriggerId(cocos2d::Node* target, unsigned int triggerId)
//{
//	const Trigger* trig = TriggerConfigManager::getManager()->getTriggerItemByID(triggerId);
//	runActionByActionId(target, trig->actionID);
//}