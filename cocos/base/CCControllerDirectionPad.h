//
//  CCControllerDirectionPad.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__CCControllerDirectionPad__
#define __cocos2d_libs__CCControllerDirectionPad__

#include "CCPlatformMacros.h"
#include "CCControllerElement.h"

NS_CC_BEGIN

class ControllerAxisInput;
class ControllerButtonInput;

class ControllerDirectionPad : public ControllerElement
{
public:
	ControllerAxisInput* getAxisX() const;
	ControllerAxisInput* getAxisY() const;
    
	ControllerButtonInput* getUp() const;
	ControllerButtonInput* getDown() const;
	ControllerButtonInput* getLeft() const;
	ControllerButtonInput* getRight() const;
    ControllerButtonInput* getCenter() const;
    
protected:
    
    friend class Controller;
    friend class Gamepad;
    
    ControllerDirectionPad();
    virtual ~ControllerDirectionPad();
    
    ControllerAxisInput* _axisX;
    ControllerAxisInput* _axisY;
    
    ControllerButtonInput* _up;
    ControllerButtonInput* _down;
    ControllerButtonInput* _left;
    ControllerButtonInput* _right;
    ControllerButtonInput* _center;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCControllerDirectionPad__) */
