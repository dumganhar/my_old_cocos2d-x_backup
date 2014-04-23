//
//  EventController.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__EventController__
#define __cocos2d_libs__EventController__

#include "CCPlatformMacros.h"
#include "CCEvent.h"

NS_CC_BEGIN

class ControllerButtonInput;
class ControllerDirectionPad;
class ControllerElement;
class Controller;

class EventController : public Event
{
public:
    enum class ControllerEventType
    {
        CONNECTION,
        BUTTON_STATUS_CHANGED,
        AXIS_STATUS_CHANGED,
    };
    
	EventController(ControllerEventType type, Controller* controller, ControllerButtonInput* button);
    EventController(ControllerEventType type, Controller* controller, ControllerDirectionPad* dpad);
    EventController(ControllerEventType type, Controller* controller, bool isConnected);

    ControllerEventType getControllerEventType() const;
    Controller* getController() const;
    ControllerElement* getControllerElement() const;
    bool isConnected() const;
    
protected:
    ControllerEventType _controllerEventType;
    ControllerElement* _element;
    Controller* _controller;
    bool _isConnected;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__EventController__) */
