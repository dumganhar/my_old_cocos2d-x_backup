//
//  CCControllerAxisInput.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__CCControllerAxisInput__
#define __cocos2d_libs__CCControllerAxisInput__

#include "CCPlatformMacros.h"
#include "CCControllerElement.h"

NS_CC_BEGIN

class ControllerAxisInput : public ControllerElement
{
public:
	float getValue() const;

protected:
    ControllerAxisInput();
    virtual ~ControllerAxisInput();
    
    void setValue(float value);
    
	float _value;
    
    friend class Controller;
    friend class ControllerDirectionPad;
    friend class ControllerThumbstick;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCControllerAxisInput__) */
