//
//  EditBoxTest.cpp
//  TestCpp
//
//  Created by James on 8/14/12.
//
//

#include "EditBoxTest.h"


USING_NS_CC;
USING_NS_CC_EXT;


EditBoxTest::EditBoxTest()
{
    
}

EditBoxTest::~EditBoxTest()
{
    
}


bool EditBoxTest::init(){

	if(!Layer::init()){
		return false;
	}

	Size visibleSize = Director::getInstance()->getWinSize();
	Actual_x=visibleSize.width;
	Actual_y=visibleSize.height;
	return true;
}
void EditBoxTest::onEnter(){

	Layer::onEnter();
}
void EditBoxTest::onExit(){

	Layer::onExit();
}


float EditBoxTest::getDelwithint(float number,int iswidth){

	float dealnumber;
	if(iswidth==0){
		dealnumber=number*(Actual_x/Design_width);
	}else{
		dealnumber=number*(Actual_y/Design_hight);
	}
    return dealnumber;

}


void EditBoxTest::toExtensionsMainLayer(cocos2d::Ref *sender)
{
    
}

void EditBoxTest::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidBegin !", editBox);
}

void EditBoxTest::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
    log("editBox %p DidEnd !", editBox);
}

void EditBoxTest::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text)
{
    log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void EditBoxTest::editBoxReturn(EditBox* editBox)
{
   
	context=editBox->getText();

	auto cs_namesize=Label::createWithSystemFont(context,"Arial",24);
	int size=cs_namesize->getContentSize().width;

	auto ctname=Label::createWithSystemFont(FX::StringsMap::getValue("Bzszzm"),"Arial",24);
	int bzsize=ctname->getContentSize().width;

	if(ctname->getContentSize().width<cs_namesize->getContentSize().width){
		_editName->setText(FX::StringsMap::getValue("zuiduoliugehanzi").c_str());
	}else if(cs_namesize->getContentSize().width==0){
		_editName->setPlaceHolder(FX::StringsMap::getValue("mingzibunengweikong").c_str());
	}
}


//单独页面

void EditBoxTest::regist_box(){





}


void EditBoxTest::shuru_name(std::string tisi){

	 
	this->setContentSize(Size(220,45));
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5,0.5));
	auto editBoxSize = Size(getContentSize().width,getContentSize().height);
    // top
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create("green_edit.png"));
	_editName->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
    _editName->setFontName("Paint Boy");
    _editName->setFontSize(24);
    _editName->setFontColor(Color3B::RED);
	_editName->setPlaceHolder(FX::StringsMap::getValue(tisi).c_str());
    _editName->setPlaceholderFontColor(Color3B::WHITE);
    _editName->setMaxLength(8);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    addChild(_editName);


}

void EditBoxTest::update_namebox(){

	this->setContentSize(Size(220,45));
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(0.5,0.5));
	auto editBoxSize = Size(getContentSize().width,getContentSize().height);
    // top
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create("green_edit.png"));
	_editName->setPosition(Vec2(getContentSize().width/2,getContentSize().height/2));
    _editName->setFontName("Paint Boy");
    _editName->setFontSize(24);
    _editName->setFontColor(Color3B::RED);
    _editName->setPlaceHolder("NickName:");
    _editName->setPlaceholderFontColor(Color3B::WHITE);
    _editName->setMaxLength(8);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    addChild(_editName);

}


void EditBoxTest::cs_box(){

	auto glview = Director::getInstance()->getOpenGLView();
    auto visibleOrigin = glview->getVisibleOrigin();
    auto visibleSize = glview->getVisibleSize();
    
    auto pBg = Sprite::create("HelloWorld.png");
    pBg->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height/2));
    addChild(pBg);
    
    _TTFShowEditReturn = Label::createWithSystemFont("No edit control return!", "", 30);
    _TTFShowEditReturn->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y + visibleSize.height - 50));
    addChild(_TTFShowEditReturn);
    
    // Back Menu
    auto itemBack = MenuItemFont::create("Back", CC_CALLBACK_1(EditBoxTest::toExtensionsMainLayer, this));
    itemBack->setPosition(Vec2(visibleOrigin.x+visibleSize.width - 50, visibleOrigin.y+25));
	auto menuBack = Menu::create(itemBack,nullptr);
    menuBack->setPosition(Vec2::ZERO);
    addChild(menuBack);
    
    auto editBoxSize = Size(visibleSize.width - 100, 60);

    // top
    _editName = EditBox::create(editBoxSize, Scale9Sprite::create("green_edit.png"));
    _editName->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*3/4));
    _editName->setFontName("Paint Boy");
    _editName->setFontSize(25);
    _editName->setFontColor(Color3B::RED);
    _editName->setPlaceHolder("Name:");
    _editName->setPlaceholderFontColor(Color3B::WHITE);
    _editName->setMaxLength(8);
    _editName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editName->setDelegate(this);
    addChild(_editName);
    
    // middle
    _editPassword = EditBox::create(editBoxSize, Scale9Sprite::create("green_edit.png"));
    _editPassword->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height/2));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_editPassword->setFont("American Typewriter", 30);
#else
    _editPassword->setFont("American Typewriter", 80);
    _editPassword->setPlaceholderFont("American Typewriter", 80);
#endif
    _editPassword->setFontColor(Color3B::GREEN);
    _editPassword->setPlaceHolder("Password:");
    _editPassword->setMaxLength(6);
    _editPassword->setInputFlag(EditBox::InputFlag::PASSWORD);
    _editPassword->setInputMode(EditBox::InputMode::SINGLE_LINE);
    _editPassword->setDelegate(this);
    addChild(_editPassword);
    
    // bottom
    _editEmail = EditBox::create(Size(editBoxSize.width, editBoxSize.height), Scale9Sprite::create("green_edit.png"));
    _editEmail->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height/4));
    _editEmail->setAnchorPoint(Vec2(0.5, 1.0f));
    _editEmail->setPlaceHolder("Email:");
    _editEmail->setInputMode(EditBox::InputMode::EMAIL_ADDRESS);
    _editEmail->setDelegate(this);
    addChild(_editEmail);
    
    this->setPosition(Vec2(10, 20));


}
