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
#import "NSLogger.h"

#import <GameController/GameController.h>

void CCNSLog(const char* file, int line, const char* function, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    char buf[1024 * 16] = {0};
    vsprintf(buf, format, args);
    
    va_end(args);
    
    LogMessageF(file, line, function, @"cocos2d-x", 0, [NSString stringWithUTF8String:buf], nullptr);
    
    LoggerFlush(LoggerGetDefaultLogger(), YES);
}

@interface GCControllerConnectionEventHandler : NSObject

typedef void (^GCControllerConnectionBlock)(GCController* controller);
@property (copy) GCControllerConnectionBlock _connectionBlock;

typedef void (^GCControllerDisconnectionBlock)(GCController* controller);
@property (copy) GCControllerDisconnectionBlock _disconnectionBlock;

+(GCControllerConnectionEventHandler*) getInstance;
+(void) destroyInstance;
@end

@implementation GCControllerConnectionEventHandler


static GCControllerConnectionEventHandler* __instance = nil;

+(GCControllerConnectionEventHandler*) getInstance {
    
    if (__instance == nil)
    {
        __instance = [[GCControllerConnectionEventHandler alloc] init];
    }
    return __instance;
}

+(void) destroyInstance {
    if (__instance)
    {
        [__instance release];
        __instance = nil;
    }
}

-(void) observerConnection: (GCControllerConnectionBlock) connectBlock disconnection: (GCControllerDisconnectionBlock) disconnectBlock {
    self._connectionBlock = connectBlock;
    self._disconnectionBlock = disconnectBlock;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onControllerConnected:) name:GCControllerDidConnectNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onControllerDisconnected:) name:GCControllerDidDisconnectNotification object:nil];
}

-(void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	[super dealloc];
}

-(void) onControllerConnected :(NSNotification *)connectedNotification {
    GCController *controller =(GCController *)[connectedNotification object];
    
    self._connectionBlock(controller);
}

-(void) onControllerDisconnected :(NSNotification *)connectedNotification {

    GCController *controller =(GCController *)[connectedNotification object];
    self._disconnectionBlock(controller);
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

    }
    
    ~ControllerImpl()
    {

    }
    
    Controller* _controller;
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

void Controller::startDiscoveryController()
{
    MyLog("startDiscoveryController...: %s", "hello");
    
    [GCController startWirelessControllerDiscoveryWithCompletionHandler: nil];

    [[GCControllerConnectionEventHandler getInstance] observerConnection: ^(GCController* gcController) {
        auto controller = new Controller();
        controller->_impl->_gcController = gcController;
        MyLog("1controller %p was connnected!", gcController);
        _controllers.push_back(controller);
        
        MyLog("2controller %p was connnected!", gcController);
        
        EventController evt(controller, true);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
        
        MyLog("3controller %p was connnected!", gcController);
        
    } disconnection: ^(GCController* gcController) {
        MyLog("controller %p was disconnected!", gcController);
        
        auto iter = std::find_if(_controllers.begin(), _controllers.end(), [gcController](Controller* c){ return c->_impl->_gcController == gcController; });
        
        CCASSERT(iter != _controllers.end(), "Could not find the controller");
        
        EventController evt(*iter, false);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
        
        delete (*iter);
        _controllers.erase(iter);
    }];
}

void Controller::stopDiscoveryController()
{
    [GCController stopWirelessControllerDiscovery];
}

Controller::Controller()
{
    _playerIndex = PLAYER_INDEX_UNSET;
    _gamepad = new Gamepad();
    _gamepad->_controller = this;
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
dstID->setPressed(srcID.isPressed); \
dstID->setValue(srcID.value); \
dstID->setAnalog(srcID.isAnalog); \
EventController evt(this, dstID); \
Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);


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
EventController evt(this, _gamepad->getDirectionPad()); \
Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);

Gamepad* Controller::getGamepad()
{
    if (_impl->_gcController == nil)
        return nullptr;
    
    if (_impl->_gcController.gamepad != nil || _impl->_gcController.extendedGamepad != nil)
    {
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
