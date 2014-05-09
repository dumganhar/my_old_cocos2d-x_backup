#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "external/nslog/CCNSLog.h"

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
//    closeItem->setPosition(origin + Vector2(visibleSize) - Vector2(closeItem->getContentSize() / 2));
//
//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vector2::ZERO);
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
//    label->setPosition(Vector2(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - label->getContentSize().height));
//
//    // add the label as a child to this layer
//    this->addChild(label, 1);
//
//    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    sprite->setPosition(Vector2(visibleSize / 2) + origin);
//
//    // add the sprite as a child to this layer
//    this->addChild(sprite);
    
    _player1 = nullptr;
    
    _statusLabel = Label::createWithTTF("status:", "fonts/Marker Felt.ttf", 20);
    _statusLabel->setPosition(Vector2(visibleSize / 2) + origin + Vector2(0, 50));
    this->addChild(_statusLabel, 0, 100);
    
    _actor = Sprite::create("CloseNormal.png");
    _actor->setPosition(Vector2(visibleSize / 2) + origin);
    this->addChild(_actor);
    
    Controller::startDiscoveryController();
//        auto controllers = Controller::getControllers();
//        if (!controllers.empty())
//        {
//            _player1 = controllers[0];
//            statusLabel->setString("Connected");
//        }
//    });

    CCNSLOG("layer: %p", this);
    
    _listener = EventListenerController::create();
    _listener->onConnected = [=](Controller* controller, Event* event){
        CCNSLOG("%p connected", controller);
        _player1 = controller;
        _statusLabel->setString("controller connected!");
    };
    
    _listener->onDisconnected = [=](Controller* controller, Event* event){
        CCNSLOG("%p disconnected", controller);
        _player1 = nullptr;
        _statusLabel->setString("controller disconnected!");
    };
    
    _listener->onButtonPressed = CC_CALLBACK_3(HelloWorld::onButtonPressed, this);
    _listener->onButtonReleased = CC_CALLBACK_3(HelloWorld::onButtonReleased, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
    
    auto bullet = Sprite::create("CloseSelected.png");
    bullet->setPosition(_actor->getPosition() + Vector2(0, _actor->getContentSize().height/2 + 20));
    this->addChild(bullet);
    bullet->setColor(Color3B::BLUE);
    
    scheduleUpdate();
    
    srand(time(nullptr));
    
    return true;
}

void HelloWorld::onButtonPressed(cocos2d::Controller *controller, cocos2d::ControllerButtonInput *button, cocos2d::Event *event)
{
    log("HelloWorld::onButtonPressed: %p, %d, %f", button, button->isPressed(), button->getValue());
    if (_player1 == nullptr)
        return;
    
    if (button == _player1->getGamepad()->getButtonA())
    {
        _statusLabel->setString("button A pressed!");
    }
    
    if (button == _player1->getGamepad()->getButtonB())
    {
        _statusLabel->setString("button B pressed!");
    }
    
    if (button == _player1->getGamepad()->getButtonX())
    {
        _statusLabel->setString("button X pressed!");
    }
    
    if (button == _player1->getGamepad()->getButtonY())
    {
        _statusLabel->setString("button Y pressed!");
    }
    
    if (button == _player1->getGamepad()->getDirectionPad()->getUp())
    {
        _statusLabel->setString("Dpad up pressed!");
    }
    
    if (button == _player1->getGamepad()->getDirectionPad()->getDown())
    {
        _statusLabel->setString("Dpad down pressed!");
    }
    
    if (button == _player1->getGamepad()->getDirectionPad()->getLeft())
    {
        _statusLabel->setString("Dpad left pressed!");
    }
    
    if (button == _player1->getGamepad()->getDirectionPad()->getRight())
    {
        _statusLabel->setString("Dpad right pressed!");
    }
    
    
    Size winSize = Director::getInstance()->getWinSize();
    
    int randX = rand() % static_cast<int>(winSize.width);

    auto bullet = Sprite::create("CloseSelected.png");
    bullet->setPosition(_actor->getPosition() + Vector2(0, _actor->getContentSize().height/2));
    this->addChild(bullet);
    bullet->setColor(Color3B::BLUE);
    bullet->runAction(Sequence::create(MoveTo::create(3.0f, Vector2(randX, winSize.height)), RemoveSelf::create(),  NULL));
}

void HelloWorld::onButtonReleased(cocos2d::Controller *controller, cocos2d::ControllerButtonInput *button, cocos2d::Event *event)
{
    CCNSLOG("HelloWorld::onButtonReleased: %p, %d, %f", button, button->isPressed(), button->getValue());
    if (_player1 == nullptr)
        return;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    int randX = rand() % static_cast<int>(winSize.width);
    
    auto bullet = Sprite::create("CloseSelected.png");
    bullet->setPosition(_actor->getPosition() + Vector2(0, _actor->getContentSize().height/2));
    bullet->setColor(Color3B::RED);
    this->addChild(bullet);
    bullet->runAction(Sequence::create(MoveTo::create(3.0f, Vector2(randX, winSize.height)), RemoveSelf::create(),  NULL));
}

void HelloWorld::update(float dt)
{
    if (_player1 && _player1->isConnected())
    {
        const int MOVE_DELTA = dt * 100;
        
        Vector2 newPos = _actor->getPosition();

        if (_player1->getGamepad()->getDirectionPad()->getDown()->isPressed())
        {
            log("Dpad: down pressed");
            _statusLabel->setString("Dpad: down pressed");
            newPos.y -= MOVE_DELTA;
        }
        
        if (_player1->getGamepad()->getDirectionPad()->getUp()->isPressed())
        {
            log("Dpad: up pressed");
            _statusLabel->setString("Dpad: up pressed");
            newPos.y += MOVE_DELTA;
        }
        
        if (_player1->getGamepad()->getDirectionPad()->getLeft()->isPressed())
        {
            log("Dpad: left pressed");
            _statusLabel->setString("Dpad: left pressed");
            newPos.x -= MOVE_DELTA;
        }
        
        if (_player1->getGamepad()->getDirectionPad()->getRight()->isPressed())
        {
            log("Dpad: right pressed");
            _statusLabel->setString("Dpad: right pressed");
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
