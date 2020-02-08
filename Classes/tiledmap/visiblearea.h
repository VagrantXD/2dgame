/* visiblearea.h */

#include "cocos2d.h"
#include "../tinyxml2/tinyxml2.h"

#ifndef VISIBLEAREA_H
#define VISIBLEAREA_H

class VisibleArea {

    public:
        bool isLoading() const { return isloading; }

        const cocos2d::Rect &getArea() const { return area; }

        const cocos2d::Vec2 getOrigin() const { return area.origin; }
        const cocos2d::Vec2 getSize() const { return area.size; }
        
        float x() const { return area.origin.x; }
        float y() const { return area.origin.y; }
        float width() const { return area.size.width; }
        float height() const { return area.size.height; }

    public:
        VisibleArea(tinyxml2::XMLElement *visibleara_element, float ySize);

    private:
        bool loadVisibleArea(tinyxml2::XMLElement *visiblearea_element, float ySize);

    private:
        cocos2d::Rect area;

        bool isloading;


};

#endif // VISIBLEAREA_H
