
#include "GameInKeyboardLayer.h"
#include "MsgDefine.h"
#include "RunningHelper.h"
#include "HeroSprite.h"
#include "GameInTouchLayer.h"
GameInKeyboardLayer::GameInKeyboardLayer()
{
	m_listener = nullptr;
}

bool GameInKeyboardLayer::init()
{
	return Layer::init();
}

void GameInKeyboardLayer::onEnter()
{
	Layer::onEnter();

	m_listener = EventListenerKeyboard::create();
	m_listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event){
		// KEY_SPACE		---> fight
		// KEY_UP_ARROW		---> jump
		// KEY_DOWN_ARROW	---> down
		if (EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode){
			if (RunningHelper::getHelper()->isTeachScene())
			{
				return;
			}
			this->unschedule(schedule_selector(GameInKeyboardLayer::checkIsTouchingLeft));
			EventCustom event(CustomButton_Jump);
			_eventDispatcher->dispatchEvent(&event);
		}else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode){
			if (RunningHelper::getHelper()->isTeachScene())
			{
				return;
			}
			this->unschedule(schedule_selector(GameInKeyboardLayer::checkIsTouchingLeft));
			this->schedule(schedule_selector(GameInKeyboardLayer::checkIsTouchingLeft), 0.01f);
			EventCustom event(CustomButton_Down);
			_eventDispatcher->dispatchEvent(&event);
		}else if (EventKeyboard::KeyCode::KEY_SPACE == keyCode){
			if (RunningHelper::getHelper()->isTeachScene())
			{
				return;
			}
			EventCustom event(CustomButton_Fight);
			_eventDispatcher->dispatchEvent(&event);
		}
	};

	m_listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){
		// KEY_SPACE		---> fight
		// KEY_UP_ARROW		---> jump
		// KEY_DOWN_ARROW	---> down
		if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode){
			RunningHelper::getHelper()->getGameInTouchLayer()->setIsTouchingLeft(false);
			if (!RunningHelper::getHelper()->isTeachScene())
			{
				RunningHelper::getHelper()->getRunnerHero()->resetRun();
			}
			this->unschedule(schedule_selector(GameInKeyboardLayer::checkIsTouchingLeft));
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}
void GameInKeyboardLayer::onExit()
{
	_eventDispatcher->removeEventListener(m_listener);
	Layer::onExit();
}

void GameInKeyboardLayer::checkIsTouchingLeft(float dt)
{
	if (HeroSprite* pHero = RunningHelper::getHelper()->getRunnerHero())
	{
		if (pHero->getCurState() == HeroSprite::state_Run)
		{
			pHero->DownGlide();
		}
	}
}