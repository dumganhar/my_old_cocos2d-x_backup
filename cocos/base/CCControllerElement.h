//
//  CCControllerElement.h
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#ifndef __cocos2d_libs__CCControllerElement__
#define __cocos2d_libs__CCControllerElement__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

class ControllerElement
{
public:
	bool isAnalog() const;
private:
	bool _isAnalog;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCControllerElement__) */
