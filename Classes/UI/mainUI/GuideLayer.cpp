#include "GuideLayer.h"


GuideLayer::GuideLayer(void)
	: m_CurMaskType(MaskType_Circle)
	, m_fRadius(100)
	, m_Size(Size(100, 100))
	, m_Rect(Rect(0, 0, 100, 100))
	, m_DrawNode(nullptr)
{
}


GuideLayer::~GuideLayer(void)
{
}

GuideLayer* GuideLayer::createWithPrama(MaskType masktype, Vec2 pos)
{
	GuideLayer* pGuideLayer = new GuideLayer();
	if (pGuideLayer && pGuideLayer->initWithPrama(masktype, pos))
	{
		return pGuideLayer;
	}

	CC_SAFE_DELETE(pGuideLayer);
	return nullptr;
}

bool GuideLayer::initWithPrama(MaskType masktype, Vec2 pos)
{
	if (!Layer::init())
	{
		return false;
	}

	m_CurMaskType = masktype;
	m_Pos = pos;
	LayerColor* pLayer = LayerColor::create(Color4B(0.0f, 0.0f, 0.0f, 125.0f));

	m_ClippingNode = ClippingNode::create();
	m_ClippingNode->setInverted(true);
	this->addChild(m_ClippingNode);

	m_ClippingNode->addChild(pLayer);

	m_DrawNode = DrawNode::create();
	m_DrawNode->setPosition(m_Pos);

	m_ClippingNode->setStencil(m_DrawNode);

	this->setSize(m_Size);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](Touch* pTouch, Event* pEvent){
		Vec2 touchPos = pTouch->getLocation();
		if (MaskType_Circle == m_CurMaskType)
		{
			if (m_Pos.getDistance(touchPos) <= m_fRadius)
			{
				return false;
			}
		}
		else if (MaskType_Rectangle == m_CurMaskType)
		{
			if (m_Rect.containsPoint(touchPos))
			{
				return false;
			}
		}
		return true;
	};
	listener->onTouchEnded = [&](Touch*, Event*){

	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void GuideLayer::setMaskType(MaskType masktype)
{
	if (masktype == m_CurMaskType)
	{
		return;
	}
	m_CurMaskType = masktype;
	updateMask();
}

void GuideLayer::setMaskPos(Vec2 pos)
{
	m_Pos = pos;
	m_DrawNode->setPosition(m_Pos);
	m_Rect = Rect(m_Pos.x - m_Size.width/2, m_Pos.y - m_Size.height/2, m_Size.width, m_Size.height);
	updateMask();
}

void GuideLayer::setSize(Size size)
{
	m_Size = size;
	m_Rect = Rect(m_Pos.x - m_Size.width/2, m_Pos.y - m_Size.height/2, m_Size.width, m_Size.height);
	updateMask();
}

void GuideLayer::setRadius(float fRadius)
{
	m_fRadius = fRadius;
	updateMask();
}

void GuideLayer::updateMask()
{
	m_DrawNode->clear();
	switch (m_CurMaskType)
	{
	case GuideLayer::MaskType_Circle:
		{
			const int iCount = 256;
			Vec2 verts[iCount];
			const float angel = 2.0f*M_PI/iCount;
			for (int i = 0; i < iCount; i++)
			{
				float radian = i*angel;
				verts[i].x = m_fRadius*cosf(radian);
				verts[i].y = m_fRadius*sinf(radian);
			}
			//m_DrawNode->drawDot(m_Pos, m_fRadius, Color4F::WHITE);
			m_DrawNode->drawPolygon(verts, iCount, Color4F::WHITE, 0.1f, Color4F::WHITE);
		}
		break;
	case GuideLayer::MaskType_Rectangle:
		{
			Vec2 verts[4];
			verts[0] = Vec2(-m_Size.width/2 , -m_Size.height/2);
			verts[1] = Vec2(verts[0].x + m_Size.width, verts[0].y);
			verts[2] = Vec2(verts[0].x + m_Size.width, verts[0].y + m_Size.height);
			verts[3] = Vec2(verts[0].x , verts[0].y + m_Size.height);

			m_DrawNode->drawPolygon(verts, 4, Color4F::WHITE, 0.1f, Color4F::WHITE);
		}
		break;
// 	case GuideLayer::MaskType_Ellipse:
// 		{
// 
// 		}
// 		break;

	default:
		break;
	}

}