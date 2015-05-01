#include "HelloWorldScene.h"

USING_NS_CC;

bool moveLeft;
bool moveRight;
bool moveUp;
bool moveDown;
bool isShooting;
cocos2d::Sprite *sprite;

void initDefaultMove() {
    moveLeft = false;
    moveRight = false;
    moveUp = false;
    moveDown = false;
    isShooting = false;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    initDefaultMove();
    
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    sprite = Sprite::create("Spaceship.png");
    sprite->setScale(0.2, 0.2);
    
//    emissionParticle->setTotalParticles(77);
//    emissionParticle->setDuration(ParticleSystem::DURATION_INFINITY);
//    emissionParticle->setEmitterMode(ParticleSystem::Mode::GRAVITY);

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    auto emissionParticle = ParticleSystemQuad::create("particle_texture.plist");
    emissionParticle->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                       origin.y + visibleSize.height /2 - 20));

    


    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    this->addChild(emissionParticle);
    
    //creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [] (EventKeyboard::KeyCode keycode, Event* event) -> void {
        switch (keycode) {
            case EventKeyboard::KeyCode::KEY_W:
                moveUp = true;
                break;
            case EventKeyboard::KeyCode::KEY_S:
                moveDown = true;
                break;
            case EventKeyboard::KeyCode::KEY_A:
                moveLeft = true;
                break;
            case EventKeyboard::KeyCode::KEY_D:
                moveRight = true;
                break;
            case EventKeyboard::KeyCode::KEY_SPACE:
                isShooting = true;
            default:
                break;
        }
    };
    
    listener->onKeyReleased = [] (EventKeyboard::KeyCode keycode, Event* event) -> void {
                switch (keycode) {
                    case EventKeyboard::KeyCode::KEY_W:
                        moveUp = false;
                        break;
                    case EventKeyboard::KeyCode::KEY_S:
                        moveDown = false;
                        break;
                    case EventKeyboard::KeyCode::KEY_A:
                        moveLeft = false;
                        break;
                    case EventKeyboard::KeyCode::KEY_D:
                        moveRight = false;
                        break;
                    case EventKeyboard::KeyCode::KEY_SPACE:
                        isShooting = false;
                        break;
                    default:
                        break;
                }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt) {
    auto visisbleSize = Director::getInstance()->getVisibleSize();
    
    int playerSpeed = 7;
    
    if (moveLeft && moveDown) {
        sprite->runAction(MoveBy::create(dt, Vec2(-playerSpeed, -playerSpeed)));
    } else if (moveLeft && moveUp) {
        sprite->runAction(MoveBy::create(dt, Vec2(-playerSpeed, playerSpeed)));
    } else if (moveRight && moveUp) {
        sprite->runAction(MoveBy::create(dt, Vec2(playerSpeed, playerSpeed)));
    } else if (moveRight && moveDown) {
        sprite->runAction(MoveBy::create(dt, Vec2(playerSpeed, -playerSpeed)));
    } else if(moveLeft) {
        sprite->runAction(MoveBy::create(dt, Vec2(-playerSpeed, 0)));
    } else if(moveRight) {
        sprite->runAction(MoveBy::create(dt, Vec2(playerSpeed, 0)));
    } else if(moveUp) {
        sprite->runAction(MoveBy::create(dt, Vec2(0, playerSpeed)));
    } else if(moveDown) {
        sprite->runAction(MoveBy::create(dt, Vec2(0, -playerSpeed)));
    }
    
    if (isShooting) {
        auto bullet = Sprite::create("particle_texture.png");
        bullet->setPosition(sprite->getPosition());
        this->addChild(bullet);
        bullet->runAction(MoveTo::create(0.4, Vec2(bullet->getPositionX(), visisbleSize.height)));
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
