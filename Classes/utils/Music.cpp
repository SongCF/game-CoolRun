#include "Music.h"
#include "cocos2d.h"
#include "PlayerInfoHelper.h"
#include "FuncTools.h"
#include <vector>
using std::vector;
using namespace cocos2d;

namespace FX{

#define USE_NEW_FORMAT	1	// 使用新的编码格式，1就直接使用新格式，0就不管


#define MAIN_UI_BG_MusicFile "bg_ui"
#define GAMEIN_BG_MusicFile "bg_run"
#define BUTTON_EFFECT_MusicFile "u_btn_click"


// 音效的根目录
static string dirRoot = "sounds/";

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//static string FORMAT = ".wav";
static string FORMAT = ".mp3";
//static string dirRoot = "sounds/win32/ios";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//static string FORMAT = ".ogg";
static string FORMAT = ".mp3";
//static string dirRoot = "sounds/android/";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//static string FORMAT = ".wav";
static string FORMAT = ".mp3";
//static string dirRoot = "sounds/win32/";
#else
#undef USE_NEW_FORMAT
//static string dirRoot = "sounds/";
#endif

Music* Music::s_musicInstance = nullptr;
Music::Music()
{
	m_simpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

//#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
//	m_bMusicSwitch = true;
//#else
//	m_bMusicSwitch = PlayerInfoHelper::getHelper()->getMusicEnable();
//#endif
	m_bMusicSwitch = PlayerInfoHelper::getHelper()->getMusicEnable();
	type = MusicType_NULL;

	CCLOG("\n\n");
	CCLOG("%s --> Music construct,Effect sound format = '%s'",__FUNCTION__,FORMAT.c_str());
	CCLOG("\n\n");
}

Music* Music::getInstance()
{
	if (s_musicInstance == NULL)
	{
		s_musicInstance = new Music();
	}
	return s_musicInstance;
}

void Music::setMusicEnable(bool on)
{
	PlayerInfoHelper::getHelper()->setMusicEnable(on);
	m_bMusicSwitch = on;
	if (m_bMusicSwitch){
		if (type == MusicType_Play)
			this->playGameInBackgroundMusic();
		else 
			this->playMainBackgroundMusic();
	}else {
		this->stopBackgroundMusic();
		this->stopAllEffect();
	}
}


// ============================ 游戏常用函数 =============================//
void Music::playMainBackgroundMusic()
{
	resumeBackgroundMusic();
	if (type == MusicType_Main)
		return;

	playBackgroundMusic(MAIN_UI_BG_MusicFile);
	type = MusicType_Main;
}
void Music::playGameInBackgroundMusic()
{
	resumeBackgroundMusic();
	if (type == MusicType_Play)
		return;
	
	type = MusicType_Play;
	playBackgroundMusic(GAMEIN_BG_MusicFile);
}
//
void Music::buttonClicked()
{
	playEffectMusic(BUTTON_EFFECT_MusicFile);
}


//background
void Music::playBackgroundMusic(std::string fileName,bool loop /* = true*/)
{
	if (! m_bMusicSwitch) return;

#if USE_NEW_FORMAT == 1
	changeExtensionName(fileName,FORMAT);
#endif
	fileName = dirRoot + fileName;
	m_simpleAudioEngine->playBackgroundMusic( fileName.c_str(),loop);
}
void Music::pauseBackgroundMusic()
{ 
	m_simpleAudioEngine->pauseBackgroundMusic();
}
void Music::resumeBackgroundMusic()
{
	m_simpleAudioEngine->resumeBackgroundMusic();
}
void Music::stopBackgroundMusic(bool release)
{
	type = MusicType_NULL;
	m_simpleAudioEngine->stopBackgroundMusic(release);
}


//all effect
void Music::pauseAllEffects()
{
	m_simpleAudioEngine->pauseAllEffects();
}
void Music::resumeAllEffects()
{
	m_simpleAudioEngine->resumeAllEffects();
}
void Music::stopAllEffect()
{
	m_simpleAudioEngine->stopAllEffects();
}

//one effect
unsigned int Music::playEffectMusic(std::string fileName,bool loop/* = false*/)
{
	if (! m_bMusicSwitch) return 0;

#if USE_NEW_FORMAT == 1
	changeExtensionName(fileName,FORMAT);
#endif

	fileName = dirRoot + fileName;
	return m_simpleAudioEngine->playEffect(fileName.c_str(),loop);
}
void Music::stopEffectMusic(unsigned int nSoundId)
{
	m_simpleAudioEngine->stopEffect(nSoundId);
}
void Music::pauseEffectMusic(unsigned int nSoundId)
{
	m_simpleAudioEngine->pauseEffect(nSoundId);
}
void Music::resumeEffectiveMusic(unsigned int nSoundId)
{
	m_simpleAudioEngine->resumeEffect(nSoundId);
}


// file preload
void Music::loadEffectMusic(std::string fileName)
{
#if USE_NEW_FORMAT == 1
	changeExtensionName(fileName,FORMAT);
#endif
	m_simpleAudioEngine->preloadEffect(fileName.c_str());
}

void Music::unLoadEffectMusic(std::string fileName)
{
#if USE_NEW_FORMAT == 1
	changeExtensionName(fileName,FORMAT);
#endif

	fileName = dirRoot + fileName;

    m_simpleAudioEngine->unloadEffect(fileName.c_str());
}


// sound volume
void Music::setVolume(float volume)
{
	setEffectVolume(volume);
	setBgVolume(volume);
}
void Music::setEffectVolume(float volume)
{
	m_simpleAudioEngine->setEffectsVolume(volume);
}
void Music::setBgVolume(float volume)
{
	m_simpleAudioEngine->setBackgroundMusicVolume(volume);
}


inline string Music::getFileNameWithoutExtentsion(string fileName)
{
	vector<string> tokens = FX::string_split(fileName,".");
	int size = tokens.size();
	if (size <= 1)
	{
		return fileName;
	}
	else
	{
		string newName = "";
		for (int i=0;i<size-1;i++)
		{
			newName += tokens[i];
		}
		return newName;
	}
}
inline void Music::changeExtensionName(string& fileName,string extensionNameWithDot)
{
	fileName = getFileNameWithoutExtentsion(fileName);
	fileName += extensionNameWithDot;
}

}//namespace