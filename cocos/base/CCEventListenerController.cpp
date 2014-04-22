//
//  EventListenerController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventListenerController.h"
#include "CCEventController.h"

NS_CC_BEGIN

const std::string EventListenerController::LISTENER_ID = "__cc_controller";

EventListenerController* EventListenerController::create()
{
    auto ret = new EventListenerController();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EventListenerController::init()
{
    auto listener = [this](Event* event){
        auto evtController = static_cast<EventController*>(event);
        if (evtController->getControllerEventType() == EventController::ControllerEventType::CONNECTION)
        {
            if (evtController->isConnected())
            {
                if (this->onConnected)
                    this->onConnected(evtController->getController(), event);
            }
            else
            {
                if (this->onDisconnected)
                    this->onDisconnected(evtController->getController(), event);
            }
        }
        else
        {
            if (this->onValueChanged)
                this->onValueChanged(evtController->getController(), evtController->getControllerElement(), event);
        }
    };
    
    if (EventListener::init(EventListener::Type::GAME_CONTROLLER, LISTENER_ID, listener))
    {
        return true;
    }
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