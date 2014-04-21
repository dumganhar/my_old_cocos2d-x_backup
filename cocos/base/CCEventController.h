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

class EventController : public Event
{
public:
	enum class ButtonType
	{
		A,
		B,
		X,
		Y,
		LEFT_SHOULDER,
		RIGHT_SHOULDER,
		LEFT_TRIGGER,
		RIGHT_TRIGGER
	};
    
	enum class DirectionPadType
	{
		DIRECTION_PAD = 0,
		LEFT_THUMB_STICK,
		RIGHT_THUMB_STICK
	};
    
	EventController(ButtonType btnType, bool isPressed, float value);
	EventController(DirectionPadType dirType, float value);
};

NS_CC_END

#endif /* defined(__cocos2d_libs__EventController__) */
