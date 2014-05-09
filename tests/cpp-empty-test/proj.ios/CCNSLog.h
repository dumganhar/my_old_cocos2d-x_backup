//
//  MyLog.h
//  cocos2d_libs
//
//  Created by James Chen on 4/23/14.
//
//

#ifndef __cocos2d_libs__MyLog__
#define __cocos2d_libs__MyLog__

extern void CCNSLog(const char* file, int line, const char* function, const char* format, ...);

#define MyLog(format, ...) CCNSLog(__FILE__, __LINE__, __FUNCTION__, format,  ##__VA_ARGS__, nullptr)

#endif /* defined(__cocos2d_libs__MyLog__) */
