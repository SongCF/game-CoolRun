//============================================================
//* 函数工具
//* 一些常用的函数工具，及宏定义
//============================================================

#pragma once

#include <vector>
#include <string>
#include "cocos2d.h"
using std::vector;
using std::string;
using namespace cocos2d;

namespace FX{

	void allChildrenFade(Node* node, float fadeValue/*0-255*/, float time, bool includeSelf = true);

	// 把source 根据 token进行拆分
	vector<string> string_split(string source,string token);

	Sprite* createLightMaskSprite(vector<Node*> lightNodes);
}