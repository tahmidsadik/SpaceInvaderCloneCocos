#include "HelloWorldScene.h"

USING_NS_CC;

bool moveLeft;
bool moveRight;
bool moveUp;
bool moveDown;
bool visibility;
cocos2d::Sprite *sprite;

void initDefaultMove() {
    moveLeft = false;
    moveRight = false;
    moveUp = false;
    moveDown = false;
    visibility = false;
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
    log("Visible width: %f\nVisible Height: %f\nOrigin.x: %f\nOrigin.y %f", visibleSize.width, visibleSize.height, origin.x, origin.y);

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
    sprite->setScale(0.33, 0.33);
    
//    emissionParticle->setTotalParticles(77);
//    emissionParticle->setDuration(ParticleSystem::DURATION_INFINITY);
//    emissionParticle->setEmitterMode(ParticleSystem::Mode::GRAVITY);

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));

    auto emissionParticle = ParticleSystemQuad::create("FireEmission.plist");
    auto emitParticle2 = ParticleSystemQuad::create("FireEmission.plist");
    emissionParticle->setPosition(Vec2(visibleSize.width / 2 - sprite->getContentSize().width, visibleSize.height / 2 - sprite->getContentSize().height / 0.88));
    emitParticle2->setPosition(Vec2(visibleSize.width / 2 - sprite->getContentSize().width / 1.1, visibleSize.height / 2 - sprite->getContentSize().height / 0.88));
    sprite->addChild(emissionParticle);
    sprite->addChild(emitParticle2);

    


    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    //creating a keyboard event listener
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=] (EventKeyboard::KeyCode keycode, Event* event) -> void {
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
                
                //weird c++ error switch case in protected scope so the next case is surrounded with braces.
            {case EventKeyboard::KeyCode::KEY_SPACE:
                auto bullet = Sprite::create("bullet.png");
                bullet->setPosition(sprite->getPosition());
                bullet->setScale(2.0, 2.0);
                this->addChild(bullet);
                bullet->runAction(MoveTo::create(0.4, Vec2(sprite->getPositionX(), visibleSize.height + 100)));
                this->schedule(schedule_selector(HelloWorld::shoot), 0.2f);
                break;}
            default:
                break;
        }
    };
    
    listener->onKeyReleased = [=] (EventKeyboard::KeyCode keycode, Event* event) -> void {
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
                        this->unschedule(schedule_selector(HelloWorld::shoot));
                        break;
                    default:
                        break;
                }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->scheduleUpdate();
    this->schedule(schedule_selector(HelloWorld::spawn_enemy), 2.0f);
    
    return true;
}

void HelloWorld::shoot(float dt) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto bullet = Sprite::create("bullet.png");
    bullet->setPosition(sprite->getPosition());
    bullet->setScale(2.0, 2.0);
    this->addChild(bullet);
    bullet->runAction(MoveTo::create(0.4, Vec2(sprite->getPositionX(), visibleSize.height + 100)));
}

void HelloWorld::spawn_enemy(float dt) {
    
    auto scrn_size = Director::getInstance()->getVisibleSize();
    int space_for_enymy_plane = scrn_size.width / 5;
    for(int i = 0; i < 5; i++) {
        auto enemy = Sprite::create("cactus.png");
        enemy->setPosition(Vec2(space_for_enymy_plane * i + 80, Director::getInstance()->getVisibleSize().height + 100));
        enemy->setScale(0.8, 0.8);
        enemy->runAction(MoveTo::create(8, Vec2(enemy->getPositionX(), -100)));
        this->addChild(enemy, 1);
    }
}

void HelloWorld::update(float dt) {
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
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
