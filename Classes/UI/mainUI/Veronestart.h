#pragma once

#include "cocos2d.h"
#include "PlayerInfoHelper.h"
#include "StringsMap.h"
#include "Uiscene.h"
#include "EditBoxTest.h"

#include <string>
#include <vector>
USING_NS_CC;
struct Timecotrol{
	   
	float biaoti_time;
	float start_time;
	float guangxiao_time;
	float time;
	int mark[3];
	vector<Armature*> armats;
	vector<string>  actioname; 

};



class Veronestart :public Layer
{
public:
	Veronestart(void);
	~Veronestart(void);
public:

	CREATE_FUNC(Veronestart);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	//页面数据
	float getDelwithint(float number,int iswidth);
	//事件
    void Shieldtouched();
	//回调函数
	void callback(Ref* ref);
	void spcall(Node* node,bool mr);
	void setcall(Node* node);
	void bsetcall(Node* node);
    void write_Button(Ref* ref);
	//界面
	void initlayer();
	void drawLayout();
	void drawOne();
	void drawTwo();
	//功能
	bool isContactpoint(Vec2 p,Sprite* sp);
	//姓名输入框
	void write_nameBox();
	//抖动
	void showShake(Node* sp);
	void setTextb(MenuItemImage* node,string str,int fontsize,Color3B b);
	//c++11  函数指针
	int getTag();
	int getTag2(int parent,int (Veronestart::*p)());
	void MovebackAction();
	void Movedone(Node* node);
	void timeupdate(float dt);

	void playAnimationByname(Node *node,std::string jsonfile,std::string filename,std::string animationName,bool isloop);
	void callfunc_AnimateEndGk(Armature *ar, MovementEventType movementType, const std::string& movementID);
	void playerTag(int tag);
	void allcallback(Ref* ref);
private:

	Layer* alayer;
	Layer* blayer;
	unsigned int state;
	unsigned int call_mark;
	unsigned int mk_tag;
	bool isdoneactive;
	unsigned int isend;
    float Actual_x;
	float Actual_y;

	EditBoxTest *write_name;
	Scale9Sprite* scal9;
	Vector<Sprite*> runsp;
	float jianbu;
	Timecotrol timectol;

	
};

