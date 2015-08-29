//
//  EditBoxTest.h
//  TestCpp
//
//  Created by James on 8/14/12.
//
//

#ifndef __TestCpp__CCEditBoxTest__
#define __TestCpp__CCEditBoxTest__

#include "cocos2d.h"
#include "SimpleAudioEngine.h" 
#include "extensions/cocos-ext.h"
#include "string.h"
#include "StringsMap.h"
USING_NS_CC;
USING_NS_CC_EXT; 

#define Design_width 960
#define Design_hight 640

class EditBoxTest : public cocos2d::Layer, public EditBoxDelegate
{
public:


    EditBoxTest();
    virtual ~EditBoxTest();
    CREATE_FUNC(EditBoxTest);

	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	float getDelwithint(float number,int iswidth);


    void toExtensionsMainLayer(cocos2d::Ref *sender);
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);

	float Actual_x;
	float Actual_y;
	std::string  context;


	void regist_box();
	void update_namebox();
	void shuru_name(std::string tisi);

	void cs_box();



    cocos2d::Label* _TTFShowEditReturn;
    EditBox* _editName;
    EditBox* _editPassword;
    EditBox* _editEmail;


};



#endif /* defined(__TestCpp__CCEditBoxTest__) */
