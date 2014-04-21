//
//  CCControllerDirectionPad.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCControllerDirectionPad.h"

NS_CC_BEGIN

ControllerAxisInput* ControllerDirectionPad::getAxisX() const
{
    return _axisX;
}

ControllerAxisInput* ControllerDirectionPad::getAxisY() const
{
    return _axisY;
}

ControllerButtonInput* ControllerDirectionPad::getUp() const
{
    return _up;
}

ControllerButtonInput* ControllerDirectionPad::getDown() const
{
    return _down;
}

ControllerButtonInput* ControllerDirectionPad::getLeft() const
{
    return _left;
}

ControllerButtonInput* ControllerDirectionPad::getRight() const
{
    return _right;
}



NS_CC_END