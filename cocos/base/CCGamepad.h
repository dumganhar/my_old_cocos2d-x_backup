//
//  CCGamepad.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__CCGamepad__
#define __cocos2d_libs__CCGamepad__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

class ControllerButtonInput;
class ControllerDirectionPad;
class Controller;

class Gamepad
{
public:
    /**
     A profile keeps a reference to the controller that this profile is mapping input from.
     */
    Controller* getController();
    
	// API for Standard / Extended Gamepad
    
	ControllerDirectionPad* getDirectionPad() const;
    
    /**
     All face buttons are required to be analog in the Extended profile. These must be arranged
     in the diamond pattern given below:
      Y
     / \
    X   B
     \ /
      A
     */
	ControllerButtonInput* getButtonA() const;
	ControllerButtonInput* getButtonB() const;
	ControllerButtonInput* getButtonX() const;
	ControllerButtonInput* getButtonY() const;
    
    // Shoulder buttons
	ControllerButtonInput* getLeftShoulder() const;
	ControllerButtonInput* getRightShoulder() const;
    
	// API for Extended Gamepad only
    
    /**
     A thumbstick is a 2-axis control that is physically required to be analog. All the elements of this directional input are thus analog.
     */
	ControllerDirectionPad* getLeftThumbstick() const;
	ControllerDirectionPad* getRightThumbstick() const;
	
    /**
     If Trigger button supports analog inputs, it could be used as acceleration and decelleration in a driving game for example.
     */
	ControllerButtonInput* getLeftTrigger() const;
	ControllerButtonInput* getRightTrigger() const;
    
protected:
    
    Gamepad();
    virtual ~Gamepad();
    
    friend class Controller;
    Controller* _controller;
    
    ControllerDirectionPad* _directionPad;
    
    ControllerButtonInput* _buttonA;
    ControllerButtonInput* _buttonB;
    ControllerButtonInput* _buttonX;
    ControllerButtonInput* _buttonY;
    
    ControllerButtonInput* _leftShoulder;
    ControllerButtonInput* _rightShoulder;
    
    ControllerDirectionPad* _leftThumbstick;
    ControllerDirectionPad* _rightThumbstick;
    
    ControllerButtonInput* _leftTrigger;
    ControllerButtonInput* _rightTrigger;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCGamepad__) */
