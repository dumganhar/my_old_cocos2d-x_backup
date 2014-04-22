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

class ControllerElement;
class Controller;

class EventController : public Event
{
public:
    
	EventController(ControllerElement* element);
    EventController(Controller* controller, bool isConnected);
    
protected:
    ControllerElement* _element;
    Controller* _controller;
    bool _isConnected;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__EventController__) */
