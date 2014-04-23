//
//  EventController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventController.h"
#include "CCControllerButtonInput.h"
#include "CCControllerDirectionPad.h"

NS_CC_BEGIN

EventController::EventController(ControllerEventType type, Controller* controller, ControllerButtonInput* button)
: Event(Type::GAME_CONTROLLER)
, _controllerEventType(type)
, _element(button)
, _controller(controller)
, _isConnected(true)
{
    
}

EventController::EventController(ControllerEventType type, Controller* controller, ControllerDirectionPad* dpad)
: Event(Type::GAME_CONTROLLER)
, _controllerEventType(type)
, _element(dpad)
, _controller(controller)
, _isConnected(true)
{
    
}

EventController::EventController(ControllerEventType type, Controller* controller, bool isConnected)
: Event(Type::GAME_CONTROLLER)
, _controllerEventType(type)
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
