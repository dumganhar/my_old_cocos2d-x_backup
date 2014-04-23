//
//  CCControllerButtonInput.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCControllerButtonInput.h"

NS_CC_BEGIN

ControllerButtonInput::ControllerButtonInput()
: _value(0.0f)
, _isPressed(false)
, _isPrevStatusPressed(false)
{
    
}

ControllerButtonInput::~ControllerButtonInput()
{
    
}

float ControllerButtonInput::getValue() const
{
    return _value;
}

bool ControllerButtonInput::isPressed() const
{
    return _isPressed;
}

bool ControllerButtonInput::isPrevStatusPressed() const
{
    return _isPrevStatusPressed;
}

void ControllerButtonInput::setValue(float value)
{
    _value = value;
}

void ControllerButtonInput::setPressed(bool isPressed)
{
    _isPrevStatusPressed = _isPressed;
    _isPressed = isPressed;
}

NS_CC_END