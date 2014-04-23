//
//  CCControllerThumbStick.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/23/14.
//
//

#include "CCControllerThumbStick.h"
#include "CCControllerAxisInput.h"
#include "CCControllerButtonInput.h"

NS_CC_BEGIN

ControllerThumbstick::ControllerThumbstick()
: _axisX(new ControllerAxisInput())
, _axisY(new ControllerAxisInput())
, _button(new ControllerButtonInput())
{
    _axisX->setCollection(this);
    _axisY->setCollection(this);
    _button->setCollection(this);
}

ControllerThumbstick::~ControllerThumbstick()
{
    CC_SAFE_DELETE(_axisX);
    CC_SAFE_DELETE(_axisY);
    CC_SAFE_DELETE(_button);
}

ControllerAxisInput* ControllerThumbstick::getAxisX() const
{
    return _axisX;
}

ControllerAxisInput* ControllerThumbstick::getAxisY() const
{
    return _axisY;
}

ControllerButtonInput* ControllerThumbstick::getButton() const
{
    return _button;
}



NS_CC_END
