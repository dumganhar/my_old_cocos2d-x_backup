#include "HelloWorldScene.h"
#include "AppMacros.h"

#include "CCGameController.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
    _eventDispatcher->removeEventListener(_listener);
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
//    auto closeItem = MenuItemImage::create(
//                                        "CloseNormal.png",
//                                        "CloseSelected.png",
//                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
//    
//    closeItem->setPosition(origin + Point(visibleSize) - Point(closeItem->getContentSize() / 2));
//
//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Point::ZERO);
//    this->addChild(menu, 1);
//    
//    /////////////////////////////
//    // 3. add your codes below...
//
//    // add a label shows "Hello World"
//    // create and initialize a label
//    
//    auto label = LabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
//    
//    // position the label on the center of the screen
//    label->setPosition(Point(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - label->getContentSize().height));
//
//    // add the label as a child to this layer
//    this->addChild(label, 1);
//
//    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    sprite->setPosition(Point(visibleSize / 2) + origin);
//
//    // add the sprite as a child to this layer
//    this->addChild(sprite);
    
    _player1 = nullptr;
    
    auto statusLabel = Label::createWithSystemFont("status:", "", 20);
    statusLabel->setPosition(Point(visibleSize / 2) + origin + Point(0, 50));
    this->addChild(statusLabel, 0, 100);
    
    _actor = Sprite::create("CloseNormal.png");
    _actor->setPosition(Point(visibleSize / 2) + origin);
    this->addChild(_actor);
    
    Controller::startDiscoveryController();
//        auto controllers = Controller::getControllers();
//        if (!controllers.empty())
//        {
//            _player1 = controllers[0];
//            statusLabel->setString("Connected");
//        }
//    });

    MyLog("layer: %p", this);
    
    _listener = EventListenerController::create();
    _listener->onConnected = [=](Controller* controller, Event* event){
        MyLog("%p connected", controller);
        _player1 = controller;
        statusLabel->setString("controller connected!");
    };
    
    _listener->onDisconnected = [=](Controller* controller, Event* event){
        MyLog("%p disconnected", controller);
        _player1 = nullptr;
        statusLabel->setString("controller disconnected!");
    };
    
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
    
    scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float dt)
{
    if (_player1 && _player1->isConnected())
    {
        const int MOVE_DELTA = dt * 100;
        Label* statusLabel = static_cast<Label*>(this->getChildByTag(100));
        
        Point newPos = _actor->getPosition();

        if (_player1->getGamepad()->getDirectionPad()->getDown()->isPressed())
        {
            statusLabel->setString("Dpad: down pressed");
            newPos.y -= MOVE_DELTA;
        }
        
        if (_player1->getGamepad()->getDirectionPad()->getUp()->isPressed())
        {
            statusLabel->setString("Dpad: up pressed");
            newPos.y += MOVE_DELTA;
        }
        
        if (_player1->getGamepad()->getDirectionPad()->getLeft()->isPressed())
        {
            statusLabel->setString("Dpad: left pressed");
            newPos.x -= MOVE_DELTA;
        }
        
        if (_player1->getGamepad()->getDirectionPad()->getRight()->isPressed())
        {
            statusLabel->setString("Dpad: right pressed");
            newPos.x += MOVE_DELTA;
        }
        
        _actor->setPosition(newPos);
    }
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
