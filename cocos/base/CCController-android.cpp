#include "CCController.h"
#include "CCGamepad.h"
#include "CCControllerDirectionPad.h"
#include "CCControllerButtonInput.h"
#include "CCControllerAxisInput.h"
#include "CCControllerThumbstick.h"

#include "ccMacros.h"
#include "CCEventDispatcher.h"
#include "CCEventController.h"
#include "CCEventListenerController.h"
#include "CCDirector.h"

#include "jni/JniHelper.h"

void CCNSLog(const char* file, int line, const char* function, const char* format, ...)
{

}

NS_CC_BEGIN

enum AndroidControllerCode
{
    LEFT_THUMBSTICK_X = 101,
    LEFT_THUMBSTICK_Y,
    RIGHT_THUMBSTICK_X,
    RIGHT_THUMBSTICK_Y,
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_LEFT_SHOULDER,
    BUTTON_RIGHT_SHOULDER,
    BUTTON_DPAD_UP,
    BUTTON_DPAD_DOWN,
    BUTTON_DPAD_LEFT,
    BUTTON_DPAD_RIGHT,
    BUTTON_START,
    BUTTON_SELECT,
    BUTTON_LEFT_THUMBSTICK,
    BUTTON_RIGHT_THUMBSTICK,
};

class ControllerImpl
{
public:
    ControllerImpl(Controller* controller)
            : _controller(controller)
            , _controllerID(-1)
    {
    }

    static std::vector<Controller*>::iterator findController(const std::string& vendorName, int controllerID)
    {
        auto iter = std::find_if(Controller::_controllers.begin(), Controller::_controllers.end(), [&](Controller* controller){
                return (vendorName == controller->getVendorName()) && (controllerID == controller->_impl->_controllerID);
            });
    }

    static void onConnected(const std::string& vendorName, int controllerID)
    {
        // Check whether the controller is already connected.
        auto iter = findController(vendorName, controllerID);
        if (iter != Controller::_controllers.end())
            return;

        // It's a new controller being connected.
        auto controller = new cocos2d::Controller();
        controller->_vendorName = vendorName;
        Controller::_controllers.push_back(controller);
        controller->_impl->_controllerID = controllerID;
        EventController evt(EventController::ControllerEventType::CONNECTION, controller, true);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
    }

    static void onDisconnected(const std::string& vendorName, int controllerID)
    {
        auto iter = findController(vendorName, controllerID);
        if (iter == Controller::_controllers.end())
        {
            CCLOGERROR("Could not find the controller!");
            return;
        }

        EventController evt(EventController::ControllerEventType::CONNECTION, *iter, false);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);

        Controller::_controllers.erase(iter);
    }

    void sendEventButton(ControllerButtonInput* button, bool isPressed, float value, bool isAnalog)
    {
        button->setPressed(isPressed);
        button->setValue(value);
        button->setAnalog(isAnalog);
        EventController evt(EventController::ControllerEventType::BUTTON_STATUS_CHANGED, _controller, button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
    }

    void sendEventAxis(ControllerAxisInput* axis, float value, bool isAnalog)
    {
        axis->setValue(value);
        axis->setAnalog(isAnalog);
        EventController evt(EventController::ControllerEventType::AXIS_STATUS_CHANGED, _controller, axis);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
    }

    static void onButtonEvent(const std::string& vendorName, int controllerID, AndroidControllerCode btnCode, bool isPressed, float value, bool isAnalog)
    {
        auto iter = findController(vendorName, controllerID);
        auto gamepad = (*iter)->getGamepad();
        auto thiz = (*iter)->getImpl();
        switch(btnCode)
        {
            case AndroidControllerCode::BUTTON_A:
            {
                thiz->sendEventButton(gamepad->getButtonA(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_B:
            {
                thiz->sendEventButton(gamepad->getButtonB(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_X:
            {
                thiz->sendEventButton(gamepad->getButtonX(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_Y:
            {
                thiz->sendEventButton(gamepad->getButtonY(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_LEFT_SHOULDER:
            {
                thiz->sendEventButton(gamepad->getLeftShoulder(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_RIGHT_SHOULDER:
            {
                thiz->sendEventButton(gamepad->getRightShoulder(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_DPAD_UP:
            {
                thiz->sendEventButton(gamepad->getDirectionPad()->getUp(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_DPAD_DOWN:
            {
                thiz->sendEventButton(gamepad->getDirectionPad()->getDown(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_DPAD_LEFT:
            {
                thiz->sendEventButton(gamepad->getDirectionPad()->getLeft(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_DPAD_RIGHT:
            {
                thiz->sendEventButton(gamepad->getDirectionPad()->getRight(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_START:
            {
                thiz->sendEventButton(gamepad->getButtonStart(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_SELECT:
            {
                thiz->sendEventButton(gamepad->getButtonSelect(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_LEFT_THUMBSTICK:
            {
                thiz->sendEventButton(gamepad->getLeftThumbstick()->getButton(), isPressed, value, isAnalog);
            }
            break;
            case AndroidControllerCode::BUTTON_RIGHT_THUMBSTICK:
            {
                thiz->sendEventButton(gamepad->getRightThumbstick()->getButton(), isPressed, value, isAnalog);
            }
            break;
            default:
                CCASSERT(false, "Invalid controller button code!");
                break;
        }
    }

    static void onAxisEvent(const std::string& vendorName, int controllerID, AndroidControllerCode axisCode, float value, bool isAnalog)
    {
        auto iter = findController(vendorName, controllerID);
        auto gamepad = (*iter)->getGamepad();
        auto thiz = (*iter)->getImpl();
        switch (axisCode)
        {
            case AndroidControllerCode::LEFT_THUMBSTICK_X:
                thiz->sendEventAxis(gamepad->getLeftThumbstick()->getAxisX(), value, isAnalog);
                break;
            case AndroidControllerCode::LEFT_THUMBSTICK_Y:
                thiz->sendEventAxis(gamepad->getLeftThumbstick()->getAxisY(), value, isAnalog);
                break;
            case AndroidControllerCode::RIGHT_THUMBSTICK_X:
                thiz->sendEventAxis(gamepad->getRightThumbstick()->getAxisX(), value, isAnalog);
                break;
            case AndroidControllerCode::RIGHT_THUMBSTICK_Y:
                thiz->sendEventAxis(gamepad->getRightThumbstick()->getAxisY(), value, isAnalog);
                break;
            default:
                CCASSERT(false, "Invalid controller axis code!");
                break;
        }
    }

private:
    Controller* _controller;
    int _controllerID;
};


std::vector<Controller*> Controller::_controllers;

const std::vector<Controller*>& Controller::getControllers()
{
    return _controllers;
}

void Controller::startDiscoveryController()
{
    // Empty implementation on Android
}

void Controller::stopDiscoveryController()
{
    // Empty implementation on Android
}

const std::string& Controller::getVendorName()
{
    return _vendorName;
}

bool Controller::isConnected() const
{
    // If there is a controller instance, it means that the controller is connected.
    // If a controller is disconnected, the instance will be destroyed.
    // So always returns true for this method.
    return true;
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
    return _gamepad;
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

NS_CC_END

extern "C" {

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxGameController_nativeControllerConnected(JNIEnv*  env, jobject thiz, jstring vendorName, jint controllerID)
    {
        CCLOG("controller id: %d connected!", controllerID);
        cocos2d::ControllerImpl::onConnected(cocos2d::JniHelper::jstring2string(vendorName), controllerID);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxGameController_nativeControllerDisconnected(JNIEnv*  env, jobject thiz, jstring vendorName, jint controllerID)
    {
        CCLOG("controller id: %d disconnected!", controllerID);
        cocos2d::ControllerImpl::onDisconnected(cocos2d::JniHelper::jstring2string(vendorName), controllerID);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxGameController_nativeControllerButtonEvent(JNIEnv*  env, jobject thiz, jstring vendorName, jint controllerID, jint button, jboolean isPressed, jfloat value, jboolean isAnalog)
    {
        CCLOG("controller id: %d, btn code: %d, isPressed: %d, value: %f, isAnalog:%d", controllerID, button, (int)isPressed, value, (int)isAnalog);
        cocos2d::ControllerImpl::onButtonEvent(cocos2d::JniHelper::jstring2string(vendorName), controllerID, static_cast<cocos2d::AndroidControllerCode>(button), isPressed, value, isAnalog);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxGameController_nativeControllerAxisEvent(JNIEnv*  env, jobject thiz, jstring vendorName, jint controllerID, jint axis, jfloat value, jboolean isAnalog)
    {
        CCLOG("controller id: %d, axis code: %d, value: %f, isAnalog:%d", controllerID, axis, value, (int)isAnalog);
        cocos2d::ControllerImpl::onAxisEvent(cocos2d::JniHelper::jstring2string(vendorName), controllerID, static_cast<cocos2d::AndroidControllerCode>(axis), value, isAnalog);
    }

} // extern "C" {
