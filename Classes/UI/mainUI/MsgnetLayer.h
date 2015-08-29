#pragma once

#include "BaseLayer.h"
#include "BaselisttableLayer.h"


class MsgnetLayer :public Layer
{
public:
	MsgnetLayer(void);
	~MsgnetLayer(void);

public:

	CREATE_FUNC(MsgnetLayer);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
      
	float Actual_x;
	float Actual_y;

	float getDelwithint(float number,int iswidth);

	Scale9Sprite* scal9;
	Label* text;
	void jiemian();
	void laodData();
	void inintNetmsg(bool bk);


	BaselisttableLayer* list;
	vector<MailInfo_layer> mails;

	void Regist_return(int tag);
	bool isRegist;

private:
	
	int currentrsp;// 0 请求消息数据 1为 读取消息数据


};

