/* level.cpp */

#include "level.h"
#include "tiledmap/tiledmap.h"

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

    map = TiledMap::create("map3/map.tmx");
    
    addChild(map);

    hero = cocos2d::Sprite::create("dv.png");

    addChild(hero);

    return true;
}
