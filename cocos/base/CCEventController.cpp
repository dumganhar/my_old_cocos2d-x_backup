//
//  EventController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventController.h"

NS_CC_BEGIN

EventController::EventController(ControllerElement* element)
: Event(Type::GAME_CONTROLLER)
, _element(element)
, _controller(nullptr)
, _isConnected(false)
{
    
}

EventController::EventController(Controller* controller, bool isConnected)
: Event(Type::GAME_CONTROLLER)
, _element(nullptr)
, _controller(controller)
, _isConnected(isConnected)
{
    
}


NS_CC_END
