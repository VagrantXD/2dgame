/* level.cpp */

#include <iostream>

#include "tiledmap/tiledmap.h"
#include "tiledmap/visiblearea.h"

#include "level.h"


cocos2d::Scene *Level::createScene() {
    auto scene = cocos2d::Scene::createWithPhysics(); 
    
    scene->getPhysicsWorld()->setGravity( cocos2d::Vec2(0, -300) );
    scene->getPhysicsWorld()->setDebugDrawMask(0xffff);

    auto layer = Level::create();
    scene->addChild(layer);

    return scene;
}

bool Level::init() {

    if( !cocos2d::Layer::init() ) {
        return false;
    }

    auto visible = cocos2d::Director::getInstance()->getVisibleSize();

    map = TiledMap::create("map3/map.tmx");

    if( !map->isLoading() ) {
        cocos2d::log("Map not loading!!!");
        return false;
    }

    map->setAnchorPoint( cocos2d::Vec2(0, 0) );

    addChild(map);

    auto va = map->loadVisibleArea();

    if( va->isLoading() ) {
        float scale = visible.height / va->height();
        map->setScale( scale );
        map->setPosition( -va->getOrigin() * map->getScale() );
    }
    else {
        cocos2d::log("\nVisible area not loading!");
    } 

    auto mb = map->loadMapBody();

    if( mb->isLoading() ) {
        auto body = cocos2d::PhysicsBody::createEdgePolygon( mb->getPolygon(), mb->getSize() ); 
        body->setPositionOffset( mb->getOffset() * map->getScale() );
        map->setPhysicsBody(body);
        
    } 
    else {
        cocos2d::log("\nMap body not loading!");
    }

    /*hero = cocos2d::Sprite::create("dv.png");

    addChild(hero); */

    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Level::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Level::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Level::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

    typedef cocos2d::EventKeyboard::KeyCode KEY_CODE;

    const float step = 10;
    
    switch(keyCode) {
        case KEY_CODE::KEY_UP_ARROW : {
                map->setPositionY(map->getPositionY() - step);
            break;
        }

        case KEY_CODE::KEY_DOWN_ARROW : {
                map->setPositionY(map->getPositionY() + step);
            break;
        }

        case KEY_CODE::KEY_LEFT_ARROW : {
                map->setPositionX(map->getPositionX() + step);
            break;
        }

        case KEY_CODE::KEY_RIGHT_ARROW : {
                map->setPositionX(map->getPositionX() - step);
            break;
        }

    }

}

void Level::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

}

