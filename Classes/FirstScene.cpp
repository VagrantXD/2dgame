/* FirstScene.cpp */

#include "FirstScene.h"
#include "tiledmap/tiledmap.h"

#include "ui/CocosGUI.h"

#include <iostream>

USING_NS_CC;

Scene* FirstScene::createScene()
{
    auto scene = Scene::createWithPhysics();

    //auto size = Director::getInstance()->getVisibleSize();
    //scene->setContentSize(size);

    scene->getPhysicsWorld()->setGravity( Vec2(0, -300) );
    scene->getPhysicsWorld()->setDebugDrawMask(0xffff);

    //
    auto layer = FirstScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool FirstScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    /////////////////////////////////////////////////////////////////////////

    tm = TiledMap::create("map3/map.tmx");

    tm->loadObjects();



    tm->loadVisibleArea();

    if( tm->isVisibleAreaLoading() ) {
        auto r = tm->getVisibleArea();

        float scale = visibleSize.height / r.size.height;
        //std::cout << std::endl << scale << std::endl;
        tm->setScale(scale);

        tm->setPosition( -r.origin * tm->getScale() );
    }


    auto mb = tm->getMapBody();

    if(mb.isLoading()) {
        auto physics = PhysicsBody::createEdgePolygon( mb.getPolygon(), mb.getSize() );
        physics->setPositionOffset( mb.getOffset() * tm->getScale() );
        tm->setPhysicsBody( physics );
    } else {
        std::cout << std::endl << "Map body not loading" << std::endl;
    }






    addChild(tm, 50);

    ///////////////////////////////////
    

    auto spritecache = SpriteFrameCache::getInstance();

    spritecache->addSpriteFramesWithFile("sprites/textures.plist");
    spritecache->addSpriteFramesWithFile("trampoline/trampoline.plist");

    auto mySprite = Sprite::createWithSpriteFrameName("1.png");
    mySprite->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
    mySprite->setAnchorPoint(Vec2(0.5, 1));

    auto move = MoveBy::create(1, Vec2(0, visibleSize.height * 0.5));
    auto move_back = move->reverse();

    auto move_ease = EaseBounceIn::create(move->clone());
    auto move_ease_back = EaseBounceOut::create(move_back->clone());
    auto delay = DelayTime::create(0.05f);

    auto seq = Sequence::create(delay, move_ease, delay->clone(), move_ease_back, nullptr);

    mySprite->runAction(RepeatForever::create(seq));

    addChild(mySprite);
    drawTrampoline();

    Vector<SpriteFrame *> frames;
    frames.pushBack(spritecache->getSpriteFrameByName("1.png"));
    frames.pushBack(spritecache->getSpriteFrameByName("2.png"));
    frames.pushBack(spritecache->getSpriteFrameByName("3.png"));
    frames.pushBack(spritecache->getSpriteFrameByName("4.png"));

    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    animation->setLoops(6.0f);
    auto animate = Animate::create(animation);

    mySprite->runAction( RepeatForever::create(animate) );

    auto closeItem = MenuItemImage::create( "UI/menu_close_normal.png", "UI/menu_close_selected.png", 
            CC_CALLBACK_1(FirstScene::menuCloseCallback, this) );
    closeItem->setScale(1.5);
    closeItem->setAnchorPoint( Vec2(1, 1) );
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition( Vec2(visibleSize.width * 0.98, visibleSize.height * 0.98) );
    this->addChild(menu, 1);

    //
    //int count = 4;
    //this->setAnchorPoint( Vec2(0, 0) );

            //body->setVelocity( Vec2(0, 1000) );
            //body->applyImpulse( Vec2(300, 0) );
            //body->applyForce( Vec2(1000, 1000) );
            //auto sh = body->getFirstShape();
            //sh->setMass(8484);
            //body->applyForce( Vec2(-1000, -1000), Vec2(200, 10) );
            //
    auto contactListener = EventListenerPhysicsContact::create();
    //contactListener->onContactBegin = CC_CALLBACK_1(FirstScene::onContactBegin, this);
    contactListener->onContactBegin = CC_CALLBACK_1(FirstScene::onContactBegin, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void FirstScene::drawTrampoline() { 
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto spritecache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame *> trampolineFrames;
    trampolineFrames.pushBack( spritecache->getSpriteFrameByName("sprite_0.png") );
    trampolineFrames.pushBack( spritecache->getSpriteFrameByName("sprite_1.png") );
    trampolineFrames.pushBack( spritecache->getSpriteFrameByName("sprite_2.png") );
    trampolineFrames.pushBack( spritecache->getSpriteFrameByName("sprite_3.png") );
    trampolineFrames.pushBack( spritecache->getSpriteFrameByName("sprite_4.png") );

    auto trampolineSprite = Sprite::createWithSpriteFrameName("sprite_0.png");

    auto trampolineAnimation = Animation::createWithSpriteFrames(trampolineFrames, 0.1f);

    auto trampolineAnimate = Animate::create(trampolineAnimation);

    auto trampolineAnimateBack = trampolineAnimate->reverse();

    //auto delay_prev = DelayTime::create(0.05f);
    auto delay = DelayTime::create(1.3f);

    auto seq = Sequence::create(trampolineAnimate->clone(), trampolineAnimateBack, delay, nullptr);

    trampolineSprite->setPosition( Vec2(500, 130) );
    trampolineSprite->setScale(5);

    trampolineSprite->runAction( RepeatForever::create(seq) );

    addChild(trampolineSprite, -10);

    auto button = ui::Button::create("Button/button-1.png", "Button/button-3.png", "Button/button-2.png");
    button->setTitleText("My Button");
    button->setPosition( Vec2(visibleSize.width * 0.2, visibleSize.height * 0.8) );
    button->setScale(2.5f);

    button->addTouchEventListener( 
        [&](Ref *sender, ui::Widget::TouchEventType type) {
            if(ui::Widget::TouchEventType::ENDED == type) {
                auto dir = Director::getInstance();
                if(dir->isPaused())
                    dir->resume();
                else
                    dir->pause();
            }
        } 
    );

    addChild(button);

    /////////////////////////////////////////////
    hero = Sprite::create("dv.png"); 
    //hero->setScale(0.1);
    body = PhysicsBody::createBox( Size(200, 200) );
    body->addMass(80);
    body->setContactTestBitmask(0x1);
    //body->setCategoryBitmask(0x04);
    hero->setPosition(300, 300);
    hero->setPhysicsBody(body);
    //hero->setRotation(180);
    hero->setFlippedX(true);
    addChild(hero, 1000);

    auto shturm = Sprite::create("shturm.png");
    auto body2 = PhysicsBody::createBox( Size(1000, 30) );
    body2->setContactTestBitmask(0x1);
    body2->setDynamic(false);
    shturm->setPosition( Vec2(visibleSize.width * 0.5, 50) );
    shturm->setPhysicsBody(body2);
    addChild(shturm, -20);

    /////////////////////////////////
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(FirstScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(FirstScene::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void FirstScene::menuCloseCallback(Ref *pSender) {
    Director::getInstance()->end();
}

void FirstScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    const float speed = 200;

    switch( keyCode ) {
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW : {
            hero->setFlippedX(true);
            auto rightMove = RepeatForever::create( MoveBy::create(1, Vec2(-speed, 0)) );
            rightMove->setTag((int)keyCode);
            tm->runAction(rightMove);
            break;
        }
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW : {
            hero->setFlippedX(false);
            auto leftMove = RepeatForever::create( MoveBy::create(1, Vec2(speed, 0)) );
            leftMove->setTag((int)keyCode);
            tm->runAction(leftMove);
            break;
        }
        case EventKeyboard::KeyCode::KEY_UP_ARROW : {
            //hero->getPhysicsBody()->setVelocity( Vec2(0, 400) );

            auto upMove = RepeatForever::create( MoveBy::create(1, Vec2(0, speed)) );
            upMove->setTag((int)keyCode);
            tm->runAction(upMove);
            break;
        }
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW : {
            auto downMove = RepeatForever::create( MoveBy::create(1, Vec2(0, -speed)) );
            downMove->setTag((int)keyCode);
            tm->runAction(downMove);
            break;
        }
        case EventKeyboard::KeyCode::KEY_ESCAPE : {
            Director::getInstance()->end();
        }
        case EventKeyboard::KeyCode::KEY_SPACE : {
            auto skew = SkewBy::create(0.2, 0, 180);
            hero->setSkewY(0.0);
            hero->runAction(skew);
        }
    }
}

void FirstScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
        hero->stopActionByTag((int)keyCode);
        tm->stopActionByTag((int)keyCode);
} 

bool FirstScene::onContactBegin(PhysicsContact &contact) {
    log("collision");

    return true;
}
