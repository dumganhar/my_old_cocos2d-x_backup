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


extern void CCNSLog(const char* file, int line, const char* function, const char* format, ...);

#define MyLog(format, ...) CCNSLog(__FILE__, __LINE__, __FUNCTION__, format,  ##__VA_ARGS__, nullptr)


NS_CC_BEGIN

class Gamepad;
class ControllerImpl;

class Controller
{
public:
	static const std::vector<Controller*>& getControllers();
    static void releaseControllers();
    
	static void startDiscoveryController();
	static void stopDiscoveryController();
    
    void setPausedCallback(const std::function<void()>& pausedCallback);
	std::string getVendorName() const;
	bool isConnected() const;
    
    static const int PLAYER_INDEX_UNSET = -1;
    
	int getPlayerIndex() const;
	void setPlayerIndex(int playerIndex);
    
	Gamepad* getGamepad();
private:
    
    Controller();
    virtual ~Controller();
    
	static std::vector<Controller*> _controllers;
    
	int _playerIndex;
    Gamepad* _gamepad;
    
    ControllerImpl* _impl;
};


NS_CC_END

#endif /* defined(__cocos2d_libs__CCController__) */
