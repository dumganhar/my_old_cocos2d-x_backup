//
//  EventController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventController.h"

NS_CC_BEGIN

EventController::EventController(Controller* controller, ControllerElement* element)
: Event(Type::GAME_CONTROLLER)
, _controllerEventType(ControllerEventType::VALUE_CHANGED)
, _element(element)
, _controller(controller)
, _isConnected(true)
{
    
}

EventController::EventController(Controller* controller, bool isConnected)
: Event(Type::GAME_CONTROLLER)
, _controllerEventType(ControllerEventType::CONNECTION)
, _element(nullptr)
, _controller(controller)
, _isConnected(isConnected)
{
    
}

EventController::ControllerEventType EventController::getControllerEventType() const
{
    return _controllerEventType;
}

Controller* EventController::getController() const
{
    return _controller;
}

ControllerElement* EventController::getControllerElement() const
{
    return _element;
}

bool EventController::isConnected() const
{
    return _isConnected;
}

NS_CC_END
