#ifndef __GuideLayer__H__
#define __GuideLayer__H__

#include "cocos2d.h"

USING_NS_CC;

class GuideLayer : public Layer
{
public:
	enum MaskType
	{
		MaskType_Circle = 1,			//圆形
		MaskType_Rectangle,				//矩形
//		MaskType_Ellipse,				//椭圆
	};

public:
	GuideLayer(void);
	~GuideLayer(void);

//	CREATE_FUNC(GuideLayer);
//	virtual bool init();

	bool initWithPrama(MaskType masktype, Vec2 pos);

	static GuideLayer* createWithPrama(MaskType masktype, Vec2 pos);

	void setMaskType(MaskType masktype);

	void setSize(Size size);
	void setRadius(float fRadius);
	void setMaskPos(Vec2 pos);

protected:
	void updateMask();
protected:

	float				m_fRadius;
	Size				m_Size;
	Rect				m_Rect;
	Vec2				m_Pos;
	ClippingNode*		m_ClippingNode;
	DrawNode*			m_DrawNode;
	MaskType			m_CurMaskType;
};


#endif // __GuideLayer__H__
