//
//  EventListenerController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCEventListenerController.h"
#include "CCEventController.h"
#include "CCControllerButtonInput.h"
#include "CCControllerAxisInput.h"
#include "ccMacros.h"

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
            switch (evtController->getControllerEventType()) {
                case EventController::ControllerEventType::BUTTON_STATUS_CHANGED:
                    {
                        auto button = static_cast<ControllerButtonInput*>(evtController->getControllerElement());

                        MyLog("button event: %d, %d, %f", button->isPressed(), button->isPrevStatusPressed(), button->getValue());
                        if (this->onButtonPressed && button->isPressed() && !button->isPrevStatusPressed())
                        {
                            this->onButtonPressed(evtController->getController(), button, event);
                        }
                        else if (this->onButtonReleased && !button->isPressed() && button->isPrevStatusPressed())
                        {
                            this->onButtonReleased(evtController->getController(), button, event);
                        }
                        
                        if (this->onButtonValueChanged)
                        {
                            this->onButtonValueChanged(evtController->getController(), button, event);
                        }
                    }
                    break;
                case EventController::ControllerEventType::AXIS_STATUS_CHANGED:
                    {
                        if (this->onAxisValueChanged)
                        {
                            auto axis = static_cast<ControllerAxisInput*>(evtController->getControllerElement());
                            this->onAxisValueChanged(evtController->getController(), axis, event);
                        }
                    }
                    break;
                default:
                    CCASSERT(false, "Invalid EventController type");
                    break;
            }
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