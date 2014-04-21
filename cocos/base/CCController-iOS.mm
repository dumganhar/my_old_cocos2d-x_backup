//
//  CCController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCController.h"
#include "CCGamepad.h"
#include "ccMacros.h"
#include "CCEventDispatcher.h"
#include "CCEventController.h"
#include "CCEventListenerController.h"
#include "CCDirector.h"

#import <GameController/GameController.h>

NS_CC_BEGIN

class ControllerImpl
{
public:
    GCController* _gcController;
};

std::vector<Controller*> Controller::_controllers;

const std::vector<Controller*>& Controller::getControllers()
{
    return _controllers;
}

void Controller::releaseControllers()
{
    for (auto& e : _controllers)
    {
        delete e;
    }
    _controllers.clear();
}

void Controller::startDiscovery(const std::function<void()>& completeCallback/* = nullptr*/)
{
    [GCController startWirelessControllerDiscoveryWithCompletionHandler:^{
        NSArray* gcControllers = [GCController controllers];
        auto controllers = Controller::getControllers();

        for (GCController* gcController in gcControllers)
        {
            if (!controllers.empty())
            {
                // If the controller has been added, skip it.
                if (_controllers.end() != std::find_if(_controllers.begin(), _controllers.end(), [gcController](Controller* c){ return c->_impl->_gcController == gcController; }))
                    continue;
            }
            auto controller = new Controller();
            controller->_impl->_gcController = gcController;
            _controllers.push_back(controller);
        }
        completeCallback();
    }];
}

void Controller::stopDiscovery()
{
    [GCController stopWirelessControllerDiscovery];
}

Controller::Controller()
{
    _playerIndex = PLAYER_INDEX_UNSET;
    _isConnected = false;
    _gamepad = nullptr;
    _impl = new ControllerImpl();
}

Controller::~Controller()
{
    CC_SAFE_DELETE(_impl);
    CC_SAFE_DELETE(_gamepad);
}

std::string Controller::getVendorName() const
{
    return [_impl->_gcController.vendorName UTF8String];
}

bool Controller::isConnected() const
{
    return _impl->_gcController.isAttachedToDevice == YES;
}

int Controller::getPlayerIndex() const
{
    return _playerIndex;
}

void Controller::setPlayerIndex(int playerIndex)
{
    _playerIndex = playerIndex;
}

Gamepad* Controller::getGamepad()
{
    if (_gamepad)
        return _gamepad;
    
    if (_impl->_gcController.gamepad != nil || _impl->_gcController.extendedGamepad != nil)
    {
        _gamepad = new Gamepad();
        _gamepad->_controller = this;
        
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        
        if (_impl->_gcController.gamepad != nil)
        {
            _impl->_gcController.gamepad.valueChangedHandler = ^(GCGamepad *gamepad, GCControllerElement *element){
                
                if (element == gamepad.dpad)
                {
                    EventController::DirectionPadType type = EventController::DirectionPadType::DIRECTION_PAD;
                    EventController evt(type);
                    dispatcher->dispatchEvent(&evt);
                }
            };
        }
        else
        {
            _impl->_gcController.extendedGamepad.valueChangedHandler = ^(GCExtendedGamepad *gamepad, GCControllerElement *element){
                
            };
        }
    }
    else
    {
        CCASSERT(false, "No gamepad was found!");
    }
    
    return _gamepad;
}

NS_CC_END
