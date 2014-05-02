#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;

// codecvt_utf8_utf16 example
#include <iostream>
#include <locale>
#include <string>

int mymain ()
{
//    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
//    std::string mbs = conversion.to_bytes( "\u4f60\u597d" );  // ni hao (你好)
//    
//    // print out hex value of each byte:
//    std::cout << std::hex;
//    for (int i=0; i<mbs.length(); ++i)
//        std::cout << mbs[i] << ' ';
//    std::cout << '\n';
//    log(mbs.c_str());
    
//    std::u32string str32 ( U"\U00004f60\U0000597d" );  // ni hao (你好)
//    std::string str8;
//    
//    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;
//    
//    str8 = cv.to_bytes(str32);
//    log(str8.c_str());
//    // print contents (as their hex representations):
//    std::cout << std::hex;
//    
//    std::cout << "UTF-32: ";
//    for (char32_t c : str32)
//        std::cout << '[' << std::uint_least32_t(c) << ']';
//    std::cout << '\n';
//    
//    std::cout << "UTF-8 : ";
//    for (char c : str8)
//        std::cout << '[' << int(static_cast<unsigned char>(c)) << ']';
//    std::cout << '\n';
    
    ///
    std::string str8 (u8"你好");    // UTF-8 for "en español"
    
//    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> cv;
    
    std::u16string str32;// = cv.from_bytes(str8);
    
    std::cout << std::hex;
    
    for (char16_t c : str32) {
        std::cout << '[' << (c) << ']';
    }
    std::cout << '\n';
    
    return 0;
}

Scene* HelloWorld::scene()
{
    
    mymain();
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vector2(visibleSize) - Vector2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vector2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    label->setPosition(Vector2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vector2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite);
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
