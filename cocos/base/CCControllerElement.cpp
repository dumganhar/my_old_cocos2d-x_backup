//
//  CCControllerElement.cpp
//  cocos2d_libs
//
//  Created by James Chen on 4/21/14.
//
//

#include "CCControllerElement.h"

NS_CC_BEGIN

ControllerElement::ControllerElement()
: _isAnalog(false)
, _collection(nullptr)
{
    
}

ControllerElement::~ControllerElement()
{
    
}

bool ControllerElement::isAnalog() const
{
    return _isAnalog;
}

void ControllerElement::setAnalog(bool isAnalog)
{
    _isAnalog = isAnalog;
}

ControllerElement* ControllerElement::getCollection()
{
    return _collection;
}

void ControllerElement::setCollection(ControllerElement* collection)
{
    _collection = collection;
}


NS_CC_END