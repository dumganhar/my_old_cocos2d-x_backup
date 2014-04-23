//
//  CCControllerDirectionPad.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCControllerDirectionPad.h"
#include "CCControllerAxisInput.h"
#include "CCControllerButtonInput.h"

NS_CC_BEGIN

ControllerDirectionPad::ControllerDirectionPad()
: _up(new ControllerButtonInput())
, _down(new ControllerButtonInput())
, _left(new ControllerButtonInput())
, _right(new ControllerButtonInput())
{
    _up->setCollection(this);
    _down->setCollection(this);
    _left->setCollection(this);
    _right->setCollection(this);
}

ControllerDirectionPad::~ControllerDirectionPad()
{
    CC_SAFE_DELETE(_up);
    CC_SAFE_DELETE(_down);
    CC_SAFE_DELETE(_left);
    CC_SAFE_DELETE(_right);
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