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

#include "CCNSLog.h"

NS_CC_BEGIN

/**
 Every controller element knows which collection it belongs to and whether its input value is analog or digital.
 */
class ControllerElement
{
public:
    /**
     Each element can be part of a wider collection of inputs that map to a single logical element. A directional pad (dpad)
     is a logical collection of two axis inputs and thus each axis belongs to the same collection element - the dpad.
     */
    ControllerElement * getCollection();
    
    /**
     Check if the element can support more than just digital values, such as decimal ranges between 0 and 1.
     */
	bool isAnalog() const;
    
protected:
    
    ControllerElement();
    virtual ~ControllerElement();
    
    void setAnalog(bool isAnalog);
    void setCollection(ControllerElement* collection);
    
    bool _isAnalog;
    ControllerElement* _collection;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__CCControllerElement__) */
