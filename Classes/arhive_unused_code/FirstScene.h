/* FirstScene.h */

#ifndef FIRSTSCENE_H
#define FIRSTSCENE_H

#include "cocos2d.h"

class TiledMap;

class FirstScene : public cocos2d::Layer
{
    private:
        cocos2d::Sprite *hero;
        cocos2d::PhysicsBody *body;
        TiledMap *tm;

    public:
        static cocos2d::Scene *createScene();

        virtual bool init();

        void drawTrampoline();
    
        CREATE_FUNC(FirstScene);

    public:
        void menuCloseCallback(cocos2d::Ref *pSender);

        void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
        bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif // FIRSTSCENE_H
