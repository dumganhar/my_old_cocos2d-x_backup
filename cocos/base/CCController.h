//
//  CCController.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__CCController__
#define __cocos2d_libs__CCController__

#include "CCPlatformMacros.h"

#include <string>
#include <vector>
#include <functional>


NS_CC_BEGIN

class Gamepad;
class ControllerImpl;

class Controller
{
public:
    static const std::vector<Controller*>& getControllers();

    static void startDiscoveryController();
    static void stopDiscoveryController();

    const std::string& getVendorName();
    bool isConnected() const;

    static const int PLAYER_INDEX_UNSET = -1;

    int getPlayerIndex() const;
    void setPlayerIndex(int playerIndex);

    Gamepad* getGamepad();

    // For internal use only
    inline ControllerImpl* getImpl() const { return _impl; };

    Controller();
    virtual ~Controller();

private:
    static std::vector<Controller*> _controllers;

    std::string _vendorName;
	int _playerIndex;
    Gamepad* _gamepad;

    friend class ControllerImpl;
    ControllerImpl* _impl;
};


NS_CC_END

#endif /* defined(__cocos2d_libs__CCController__) */
