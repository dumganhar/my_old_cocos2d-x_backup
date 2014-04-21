//
//  EventController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventController.h"

NS_CC_BEGIN

EventController::EventController(ButtonType btnType, bool isPressed, float value)
: Event(Type::GAME_CONTROLLER)
{
    
}

EventController::EventController(DirectionPadType dirType, float value)
: Event(Type::GAME_CONTROLLER)
{
    
}

NS_CC_END
