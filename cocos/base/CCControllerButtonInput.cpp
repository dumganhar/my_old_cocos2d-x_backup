//
//  CCControllerButtonInput.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCControllerButtonInput.h"

NS_CC_BEGIN

float ControllerButtonInput::getValue() const
{
    return _value;
}

bool ControllerButtonInput::isPressed() const
{
    return _isPressed;
}

void ControllerButtonInput::setValue(float value)
{
    _value = value;
}

void ControllerButtonInput::setPressed(bool isPressed)
{
    _isPressed = isPressed;
}

NS_CC_END