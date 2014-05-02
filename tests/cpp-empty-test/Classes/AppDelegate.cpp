#include "AppDelegate.h"

#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "ConvertUTF/ConvertUTF.h"
USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void foo() {
    ConversionResult result = sourceIllegal;
    UTF16 utf16_buf[8] = {0};
    utf16_buf[0] = 0x48;
    utf16_buf[1] = 0x49;
    utf16_buf[2] = 0;
    utf16_buf[3] = 0;
    utf16_buf[4] = 0;
    utf16_buf[5] = 0;
    utf16_buf[6] = 0;
    utf16_buf[7] = 0;
    UTF16 *utf16Start = utf16_buf;
    UTF8 utf8_buf[16] = {0};
    UTF8* utf8Start = utf8_buf;
    
    result = ConvertUTF16toUTF8((const UTF16 **) &utf16Start, &(utf16_buf[6]), &utf8Start, &(utf8_buf[16]), strictConversion);
    switch (result) {
        default: fprintf(stderr, "Test02B fatal error: result %d for input %08x/n", result, utf16_buf[0]); exit(1);
        case conversionOK: break;
        case sourceExhausted: printf("sourceExhausted/t"); exit(0);
        case targetExhausted: printf("targetExhausted/t"); exit(0);
        case sourceIllegal: printf("sourceIllegal/t"); exit(0);
    }
    
    // 清空缓存，以确定以后的值的确是转换得来
    memset(utf16_buf, 0, sizeof(utf16_buf));
    
    // 由于转换中利用了这两个start，所以需要重新为start定位,并且保存住End值
    UTF8* utf8End = utf8Start;
    utf8Start = utf8_buf;
    utf16Start = utf16_buf;
    
    result = ConvertUTF8toUTF16((const UTF8 **) &utf8Start, utf8End, &utf16Start, &(utf16_buf[6]), strictConversion);
    switch (result) {
        default: fprintf(stderr, "Test02B fatal error: result %d for input %08x/n", result, utf16_buf[0]); exit(1);
        case conversionOK: break;
        case sourceExhausted: printf("sourceExhausted/t");
        case targetExhausted: printf("targetExhausted/t");
        case sourceIllegal: printf("sourceIllegal/t");
    }
    
    for (int i = 0; i < 7; ++i) {
        log("%x", utf16_buf[i]);
    }
    
    log("%s", utf8_buf);
    
    std::u16string utf16 = cc_utf8_to_utf16("你好");
    for (int i = 0; i < 2; ++i) {
        log("%x", utf16[i]);
    }
    
    std::string hello = cc_utf16_to_utf8(utf16);
    log("%s", hello.c_str());
    
    
}
bool AppDelegate::applicationDidFinishLaunching() {
    foo();
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Cpp Empty Test");
        director->setOpenGLView(glview);
    }

    director->setOpenGLView(glview);

    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // a bug in DirectX 11 level9-x on the device prevents ResolutionPolicy::NO_BORDER from working correctly
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#else
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
#endif

	Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        director->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        director->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::scene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
