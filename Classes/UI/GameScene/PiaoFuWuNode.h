#ifndef _PiaoFuWuNode_H_
#define _PiaoFuWuNode_H_

#include "cocos2d.h"
#include "BaseSprite.h"
#include "GameSceneConfigManager.h"

using namespace cocos2d;
using namespace std;

class PiaoFuWuNode : public Node
{
public:
	PiaoFuWuNode(void);
	~PiaoFuWuNode(void);

	static PiaoFuWuNode* createWithLayerInfo(const LayersBackground& LayersBackgroundInfo);

	inline void setMoveSpeed(float fMoveSpeed)
	{
		m_fPreMoveSpeed = m_fCurMoveSpeed;
		m_fCurMoveSpeed = fMoveSpeed;
	}

	inline float getMoveSpeed()
	{
		return m_fCurMoveSpeed;
	}

	inline float getDefaultMoveSpeed()
	{
		return m_fDefaultMoveSpeed;
	}

	inline LayersBackground getLayersBackgroundInfo()
	{
		return m_LayersBackgroundInfo;
	}

	void setScrollEnable(bool e){m_bScroll = e;};

protected:
	bool initWithInfo(const LayersBackground& LayersBackgroundInfo);
	void update(float dt);
	Node* addPiaoFuWuSprite(int iIndex);
	void removeUnSeeSpr();

	bool					m_bScroll;
	float					m_fCurMoveSpeed;
	float					m_fPreMoveSpeed;
	float					m_fDefaultMoveSpeed;

	vector<Node*>			m_vPiaoFuWuSprite;
	LayersBackground		m_LayersBackgroundInfo;
};
#endif /*_PiaoFuWuNode_H_*/
