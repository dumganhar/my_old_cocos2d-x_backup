//
//  CCControllerAxisInput.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCControllerAxisInput.h"

NS_CC_BEGIN

float ControllerAxisInput::getValue() const
{
    return _value;
}

void ControllerAxisInput::setValue(float value)
{
    _value = value;
}


NS_CC_END