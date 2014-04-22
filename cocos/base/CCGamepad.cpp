//
//  CCGamepad.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCGamepad.h"
#include "CCControllerDirectionPad.h"
#include "CCControllerButtonInput.h"

NS_CC_BEGIN

Gamepad::Gamepad()
: _controller(nullptr)
, _directionPad(new ControllerDirectionPad())
, _buttonA(new ControllerButtonInput())
, _buttonB(new ControllerButtonInput())
, _buttonX(new ControllerButtonInput())
, _buttonY(new ControllerButtonInput())
, _leftShoulder(new ControllerButtonInput())
, _rightShoulder(new ControllerButtonInput())
, _leftThumbstick(new ControllerDirectionPad())
, _rightThumbstick(new ControllerDirectionPad())
, _leftTrigger(new ControllerButtonInput())
, _rightTrigger(new ControllerButtonInput())
{
    
}

Gamepad::~Gamepad()
{
    CC_SAFE_DELETE(_directionPad);
    CC_SAFE_DELETE(_buttonA);
    CC_SAFE_DELETE(_buttonB);
    CC_SAFE_DELETE(_buttonX);
    CC_SAFE_DELETE(_buttonY);
    CC_SAFE_DELETE(_leftShoulder);
    CC_SAFE_DELETE(_rightShoulder);
    CC_SAFE_DELETE(_leftThumbstick);
    CC_SAFE_DELETE(_rightThumbstick);
    CC_SAFE_DELETE(_leftTrigger);
    CC_SAFE_DELETE(_rightTrigger);
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
