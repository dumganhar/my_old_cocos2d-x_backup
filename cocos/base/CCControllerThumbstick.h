//
//  CCControllerThumbStick.h
//  cocos2d_libs
//
//  Created by James Chen on 4/23/14.
//
//

#ifndef __cocos2d_libs__CCControllerThumbStick__
#define __cocos2d_libs__CCControllerThumbStick__

#include "CCPlatformMacros.h"
#include "CCControllerElement.h"

NS_CC_BEGIN

class ControllerAxisInput;
class ControllerButtonInput;

class ControllerThumbstick : public ControllerElement
{
public:
	ControllerAxisInput* getAxisX() const;
	ControllerAxisInput* getAxisY() const;
    
    ControllerButtonInput* getButton() const;
    
protected:
    
    friend class Controller;
    friend class Gamepad;
    
    ControllerThumbstick();
    virtual ~ControllerThumbstick();
    
    ControllerAxisInput* _axisX;
    ControllerAxisInput* _axisY;
    
    ControllerButtonInput* _button;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCControllerThumbStick__) */
