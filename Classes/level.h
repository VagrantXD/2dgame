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

    private:
        TiledMap *map; 
        cocos2d::Sprite *hero;
};

#endif //LEVEL_H
