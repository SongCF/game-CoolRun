#ifndef _ScrollForeverNode_H_
#define _ScrollForeverNode_H_

#include "cocos2d.h"
#include "BaseSprite.h"
#include "GameSceneConfigManager.h"

using namespace cocos2d;
using namespace std;

class ScrollForeverNode : public Node
{
public:
	ScrollForeverNode(void);
	~ScrollForeverNode(void);

	static ScrollForeverNode* createWithInfo(const Background& info);
	float getDefaultMoveSpeed(){
		return m_fDefaultMoveSpeed;
	}
	inline void setMoveSpeed(float fMoveSpeed)
	{
		m_fMoveSpeed = fMoveSpeed;
	}

	inline float getMoveSpeed()
	{
		return m_fMoveSpeed;
	}

	inline Background getBackGroundInfo()
	{
		return m_BackGroundInfo;
	}

//	void refreshImagVec(vector<string> imageVec, float fMoveSpeed);
	void refreshImagVec(const Background& info, EventCustom* pMsg);
	void setScrollEnable(bool e){m_bScroll = e;};

protected:
	bool initWithInfo(const Background& info);
	void update(float dt);
	void changeDisplayFrame(Sprite* curSprite, Sprite* preSprite);
	void updateTexture1();
	void updateTexture2();
	void updateTexture3();

	Background				m_BackGroundInfo;
	vector<string>			m_ImageVec;
	
	BaseSprite*					m_pFirstSprite;
	BaseSprite*					m_pSecondSprite;
	BaseSprite*					m_pThirdSprite;

	int						m_iImageVecCount;
	bool					m_bScroll;
	float					m_fMoveSpeed;
	float					m_fDefaultMoveSpeed; //默认的移动速度  --- 方便更改速度后还原

};
#endif /*_ScrollForeverNode_H_*/
