//
//  EventListenerController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventListenerController.h"

NS_CC_BEGIN

EventListenerController* EventListenerController::create(Controller* controller)
{
    auto ret = new EventListenerController();
    if (ret && ret->init(controller))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerController::init(Controller* controller)
{
    return false;
}

bool EventListenerController::checkAvailable()
{
    return true;
}

EventListenerController* EventListenerController::clone()
{
    return nullptr;
}

NS_CC_END