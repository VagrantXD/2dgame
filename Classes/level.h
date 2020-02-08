/* level.h */

#ifndef LEVEL_H
#define LEVEL_H

#include "cocos2d.h"

class TiledMap;

class Level : public cocos2d::Layer {

    public:
        static cocos2d::Scene *createScene();

        virtual bool init();

        CREATE_FUNC(Level);

    private:  // слоты
        void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

    private:
        TiledMap *map; 


        cocos2d::Sprite *hero; // Временное решение, нужно разрабатывать класс персонажа
};

#endif //LEVEL_H
