//
//  CCControllerButtonInput.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__CCControllerButtonInput__
#define __cocos2d_libs__CCControllerButtonInput__

#include "CCPlatformMacros.h"
#include "CCControllerElement.h"

NS_CC_BEGIN

class ControllerButtonInput : public ControllerElement
{
public:
	float getValue() const;
	bool isPressed() const;
    
protected:
    friend class EventListenerController;
    
    void setValue(float value);
    void setPressed(bool isPressed);
    
	float _value;
	bool _isPressed;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCControllerButtonInput__) */
