//
//  CCGamepad.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCGamepad.h"

NS_CC_BEGIN

Gamepad::Gamepad()
: _controller(nullptr)
, _directionPad(nullptr)
, _buttonA(nullptr)
, _buttonB(nullptr)
, _buttonX(nullptr)
, _buttonY(nullptr)
, _leftShoulder(nullptr)
, _rightShoulder(nullptr)
, _leftThumbstick(nullptr)
, _rightThumbstick(nullptr)
, _leftTrigger(nullptr)
, _rightTrigger(nullptr)
{
    
}

Gamepad::~Gamepad()
{
    
}

Controller* Gamepad::getController()
{
    return _controller;
}

ControllerDirectionPad* Gamepad::getDirectionPad() const
{
    return _directionPad;
}

ControllerButtonInput* Gamepad::getButtonA() const
{
    return _buttonA;
}

ControllerButtonInput* Gamepad::getButtonB() const
{
    return _buttonB;
}

ControllerButtonInput* Gamepad::getButtonX() const
{
    return _buttonX;
}

ControllerButtonInput* Gamepad::getButtonY() const
{
    return _buttonY;
}

ControllerButtonInput* Gamepad::getLeftShoulder() const
{
    return _leftShoulder;
}

ControllerButtonInput* Gamepad::getRightShoulder() const
{
    return _rightShoulder;
}

ControllerDirectionPad* Gamepad::getLeftThumbstick() const
{
    return _leftThumbstick;
}

ControllerDirectionPad* Gamepad::getRightThumbstick() const
{
    return _rightThumbstick;
}

ControllerButtonInput* Gamepad::getLeftTrigger() const
{
    return _leftTrigger;
}

ControllerButtonInput* Gamepad::getRightTrigger() const
{
    return _rightTrigger;
}

NS_CC_END
