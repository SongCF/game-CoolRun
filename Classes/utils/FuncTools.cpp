#include "FuncTools.h"

namespace FX{


void allChildrenFade(Node* node, float fadeValue/*0-255*/, float time, bool includeSelf)
{
	if (node == nullptr) return;
	if (includeSelf)
	{
		node->runAction(FadeTo::create(time, fadeValue));
	}

	const Vector<Node*> &vec = node->getChildren();
	for (int i=0,count=vec.size(); i<count; ++i)
	{
		allChildrenFade(vec.at(i), fadeValue, time, true);
	}
}

vector<string> string_split(string source,string token)
{
	vector<string> ret;

	// 参数解析
	char* cmd = new char[source.length() + 1];
	memset(cmd,0,source.length() + 1);
	strncpy(cmd,source.c_str(),source.length());
	char *tokenPtr=strtok(cmd,token.c_str());
	while(tokenPtr!=NULL)
	{
		ret.push_back(string(tokenPtr));
		tokenPtr=strtok(NULL,token.c_str());
	}
	delete [] cmd;

	return ret;
}


Sprite* createLightMaskSprite(vector<Node*> lightNodes)
{
	//Sprite * base = Sprite::create("ui_zhuye_daojukuang.png");
	LayerColor *base = LayerColor::create(Color4B(100,100,100,255));
	base->setPosition(Director::getInstance()->getWinSize());

	RenderTexture* rt = RenderTexture::create(base->getContentSize().width, base->getContentSize().height);//创建纹理渲染范围  
	rt->begin();

	base->visit();   

	BlendFunc blendFunc;
	blendFunc.src = GL_DST_ALPHA;   
	blendFunc.dst = GL_ZERO;
	for (int i=0,nodeNum=lightNodes.size(); i<nodeNum; i++)
	{
		LayerColor *temp = LayerColor::create(Color4B(255,255,255,0), lightNodes[i]->getContentSize().width, lightNodes[i]->getContentSize().height);
		//Sprite * temp = Sprite::create("ui_zhuye_daojukuang.png");
		temp->setBlendFunc(blendFunc);
		temp->setPosition(Director::getInstance()->getWinSize()/2);
		temp->visit();
	}

	rt->end();

	return Sprite::createWithTexture(rt->getSprite()->getTexture());
}

}