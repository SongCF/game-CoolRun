#include "NetworkPackageDispatch.h"
#include "NetworkThreadProc.h"
#include "TimeoutMonitor.h"


NetworkPackageDispatch* NetworkPackageDispatch::m_packageDispatch = nullptr;

NetworkPackageDispatch::NetworkPackageDispatch()
:m_msgProc(nullptr)
{
}

NetworkPackageDispatch::~NetworkPackageDispatch()
{

}

NetworkPackageDispatch* NetworkPackageDispatch::getInstance()
{
	if (! m_packageDispatch) {
		m_packageDispatch = new NetworkPackageDispatch;
		m_packageDispatch->initNode();
	}
	return m_packageDispatch;
}

void NetworkPackageDispatch::setMsgProc(std::function<void(int, unsigned short, void*, unsigned int)> function)
{
	m_msgProc = function;
}

void NetworkPackageDispatch::initNode()
{
	Node::init();

	EventListenerCustom *listener = EventListenerCustom::create(MSG_WAIT_TIME_OUT, std::bind(&NetworkPackageDispatch::onTimeOut, this, std::placeholders::_1));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// Let's running
	this->onEnter();
	this->onEnterTransitionDidFinish();
}

void NetworkPackageDispatch::onEnter()
{
	Node::onEnter();
	this->scheduleUpdate();
}

void NetworkPackageDispatch::onExit()
{
	Node::onExit();
}

void NetworkPackageDispatch::update(float dt)
{
	RespondPackage* data = NetworkThreadProc::getInstance()->getRecvData(); //remember delete
	if (! data) return;

	switch (data->getType())
	{
		// none --- recv err
		// 全当作超时处理
	case RespondPackage::Type_None:
		break;
	case RespondPackage::Type_Conncet_Suc:
		CCLOG("%s ---> connect success", __FUNCTION__);
		break;
	case RespondPackage::Type_Conncet_Err:
		CCLOG("%s ---> connect error", __FUNCTION__);
		break;
	case RespondPackage::Type_Send_Err:
		CCLOG("%s ---> send error", __FUNCTION__);
		break;
	case RespondPackage::Type_Recv_Err:
		CCLOG("%s ---> recv error", __FUNCTION__);
		break;
	case RespondPackage::Type_Recv_OK:
		TimeoutMonitor::getInstance()->cancleMonitor();
		messageDispatch(data);
		break;
	default:
		CCLOG("%s ---> error branch", __FUNCTION__);
		break;
	}

	delete data;
}

void NetworkPackageDispatch::messageDispatch(RespondPackage* package)
{
	SPHead* pHead = (SPHead*)package->getHead();
	
	m_msgProc(0, pHead->cmd, package->getBody(), pHead->data_len-sizeof(unsigned int));  // 除去校验码
}

void NetworkPackageDispatch::onTimeOut(EventCustom* event)
{
	//网络请求错误
	m_msgProc(1, 0, 0, 0);
}