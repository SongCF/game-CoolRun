//================================================
//* 音效管理类
//* 对SimpleAudioEngine的封装
//* 功能 ：播放音效、背景音乐、更改文件后缀
//================================================

#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>


namespace FX{

using std::string;


// 描述背景音乐的播放类型
enum MusicType
{
	MusicType_NULL,	// 无效
	MusicType_Main,
	MusicType_Play,
};

class Music
{
private:
	Music();
	static Music* s_musicInstance;
	bool m_bMusicSwitch; //开关
public:
	static Music* getInstance();

	bool isMusicEnable() {return m_bMusicSwitch;}
	void setMusicEnable(bool on);

	// 播放游戏外面的声音
	void playMainBackgroundMusic();
	// 播放游戏里面的声音
	void playGameInBackgroundMusic();
	void buttonClicked();

	void playBackgroundMusic(std::string fileName,bool = true);
	void pauseBackgroundMusic();
	void resumeBackgroundMusic();
	void stopBackgroundMusic(bool release = true);

	unsigned int playEffectMusic(std::string fileName,bool loop = false);
	void pauseAllEffects();
	void resumeAllEffects();
	void stopAllEffect();
	void pauseEffectMusic(unsigned int nSoundId);
	void resumeEffectiveMusic(unsigned int nSoundId);
	void stopEffectMusic(unsigned int nSoundId);

	void loadEffectMusic(std::string fileName);
    void unLoadEffectMusic(std::string fileName);

	//声音大小 （ps：无效果，因为SimpleAudioEngine库本身就没有实现）
	//[0,1.0]
	void setVolume(float volume);
	//[0,1.0]
	void setEffectVolume(float volume);
	//[0,1.0]
	void setBgVolume(float volume);

protected:

	// 获取去掉扩展名的文件名
	string getFileNameWithoutExtentsion(string fileName);

	// 修改文件的扩展名
	// fileName:比如 kkk.wav
	// extensionNameWithDot : 比如.mp3
	// 新的fileName即为：kkk.mp3
	void changeExtensionName(string& fileName,string extensionNameWithDot);

	MusicType type;
	CocosDenshion::SimpleAudioEngine *m_simpleAudioEngine;
};


}//namespace