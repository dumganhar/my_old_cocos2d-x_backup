//
//  CCController.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCController.h"
#include "CCGamepad.h"
#include "CCControllerDirectionPad.h"
#include "CCControllerButtonInput.h"
#include "CCControllerAxisInput.h"

#include "ccMacros.h"
#include "CCEventDispatcher.h"
#include "CCEventController.h"
#include "CCEventListenerController.h"
#include "CCDirector.h"

#import <GameController/GameController.h>

@interface GCControllerEventHandler : NSObject

typedef void (^GCControllerConnectionBlock)();
@property (copy) GCControllerConnectionBlock _connectionBlock;

typedef void (^GCControllerDisconnectionBlock)();
@property (copy) GCControllerDisconnectionBlock _disconnectionBlock;

@end

@implementation GCControllerEventHandler

-(void) onControllerConnected :(NSNotification *)note {
    [self _connectionBlock];
}

-(void) onControllerDisconnected :(NSNotification *)note {
    [self _disconnectionBlock];
}

@end

NS_CC_BEGIN

class ControllerImpl
{
public:
    ControllerImpl(Controller* controller)
    : _controller(controller)
    , _gcController(nil)
    {
        _gcControllerEventHandler = [[GCControllerEventHandler alloc] init];
        _gcControllerEventHandler._connectionBlock = ^(){
            if ([[GCController controllers] count] > 0) {
                for (GCController* c in [GCController controllers])
                {
//                    if (_controllers.end() != std::find_if(_controllers.begin(), _controllers.end(), [gcController](Controller* c){ return c->_impl->_gcController == gcController; }))
//                    {
//                        
//                    }
//                        
//                    if (c == _gcController)
//                    {
//                        EventController evt(_controller, true);
//                        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
//                    }
                }
            }
        };
        _gcControllerEventHandler._disconnectionBlock = ^(){
        
        };
    }
    
    ~ControllerImpl()
    {
        [_gcControllerEventHandler release];
    }
    
    Controller* _controller;
    GCController* _gcController;
    GCControllerEventHandler* _gcControllerEventHandler;
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
            
            [[NSNotificationCenter defaultCenter] addObserver:controller->_impl->_gcControllerEventHandler selector:@selector(onControllerConnected) name:GCControllerDidConnectNotification object:nil];
            [[NSNotificationCenter defaultCenter] addObserver:controller->_impl->_gcControllerEventHandler selector:@selector(onControllerDisconnected) name:GCControllerDidDisconnectNotification object:nil];
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
    _gamepad = nullptr;
    _impl = new ControllerImpl(this);
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

#define sendEventButton(dstID, srcID) \
auto dispatcher = Director::getInstance()->getEventDispatcher(); \
dstID->setPressed(srcID.isPressed); \
dstID->setValue(srcID.value); \
dstID->setAnalog(srcID.isAnalog); \
EventController evt(dstID); \
dispatcher->dispatchEvent(&evt);


#define sendEventDirectionPad(dstID, srcID) \
dstID->getAxisX()->setValue(srcID.xAxis.value); \
dstID->getAxisX()->setAnalog(srcID.xAxis.isAnalog); \
dstID->getAxisY()->setValue(srcID.yAxis.value); \
dstID->getAxisY()->setAnalog(srcID.yAxis.isAnalog); \
\
dstID->getUp()->setValue(srcID.up.value); \
dstID->getUp()->setPressed(srcID.up.isPressed); \
dstID->getUp()->setAnalog(srcID.up.isAnalog); \
\
dstID->getDown()->setValue(srcID.down.value); \
dstID->getDown()->setPressed(srcID.down.isPressed); \
dstID->getDown()->setAnalog(srcID.down.isAnalog); \
\
dstID->getLeft()->setValue(srcID.left.value); \
dstID->getLeft()->setPressed(srcID.left.isPressed); \
dstID->getLeft()->setAnalog(srcID.left.isAnalog); \
\
dstID->getRight()->setValue(srcID.right.value); \
dstID->getRight()->setPressed(srcID.right.isPressed); \
dstID->getRight()->setAnalog(srcID.right.isAnalog); \
\
EventController evt(_gamepad->getDirectionPad()); \
Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);

Gamepad* Controller::getGamepad()
{
    if (_gamepad)
        return _gamepad;
    
    if (_impl->_gcController == nil)
        return nullptr;
    
    if (_impl->_gcController.gamepad != nil || _impl->_gcController.extendedGamepad != nil)
    {
        _gamepad = new Gamepad();
        _gamepad->_controller = this;
        
        if (_impl->_gcController.gamepad != nil)
        {
            _impl->_gcController.gamepad.valueChangedHandler = ^(GCGamepad *gamepad, GCControllerElement *element){
                
                if (element == gamepad.dpad)
                {
                    sendEventDirectionPad(_gamepad->getDirectionPad(), gamepad.dpad);
                }
                else if (element == gamepad.buttonA)
                {
                    sendEventButton(_gamepad->getButtonA(), gamepad.buttonA);
                }
                else if (element == gamepad.buttonB)
                {
                    sendEventButton(_gamepad->getButtonB(), gamepad.buttonB);
                }
                else if (element == gamepad.buttonX)
                {
                    sendEventButton(_gamepad->getButtonX(), gamepad.buttonX);
                }
                else if (element == gamepad.buttonY)
                {
                    sendEventButton(_gamepad->getButtonY(), gamepad.buttonY);
                }
                else if (element == gamepad.leftShoulder)
                {
                    sendEventButton(_gamepad->getLeftShoulder(), gamepad.leftShoulder);
                }
                else if (element == gamepad.rightShoulder)
                {
                    sendEventButton(_gamepad->getRightShoulder(), gamepad.rightShoulder);
                }
            };
        }
        else
        {
            _impl->_gcController.extendedGamepad.valueChangedHandler = ^(GCExtendedGamepad *gamepad, GCControllerElement *element){
                if (element == gamepad.dpad)
                {
                    sendEventDirectionPad(_gamepad->getDirectionPad(), gamepad.dpad);
                }
                else if (element == gamepad.buttonA)
                {
                    sendEventButton(_gamepad->getButtonA(), gamepad.buttonA);
                }
                else if (element == gamepad.buttonB)
                {
                    sendEventButton(_gamepad->getButtonB(), gamepad.buttonB);
                }
                else if (element == gamepad.buttonX)
                {
                    sendEventButton(_gamepad->getButtonX(), gamepad.buttonX);
                }
                else if (element == gamepad.buttonY)
                {
                    sendEventButton(_gamepad->getButtonY(), gamepad.buttonY);
                }
                else if (element == gamepad.leftShoulder)
                {
                    sendEventButton(_gamepad->getLeftShoulder(), gamepad.leftShoulder);
                }
                else if (element == gamepad.rightShoulder)
                {
                    sendEventButton(_gamepad->getRightShoulder(), gamepad.rightShoulder);
                }
                else if (element == gamepad.leftThumbstick)
                {
                    sendEventDirectionPad(_gamepad->getLeftThumbstick(), gamepad.leftThumbstick);
                }
                else if (element == gamepad.rightThumbstick)
                {
                    sendEventDirectionPad(_gamepad->getRightThumbstick(), gamepad.rightThumbstick);
                }
                else if (element == gamepad.leftTrigger)
                {
                    sendEventButton(_gamepad->getLeftTrigger(), gamepad.leftTrigger);
                }
                else if (element == gamepad.rightTrigger)
                {
                    sendEventButton(_gamepad->getRightTrigger(), gamepad.rightTrigger);
                }
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
