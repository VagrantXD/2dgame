/* mapbody.h */

#ifndef MAPBODY_H
#define MAPBODY_H

#include "tinyxml2/tinyxml2.h"
#include "cocos2d.h"

class MapBody {

    public:    // property
        bool isLoad() { return isload; }
        int getSize() { return size; }
        cocos2d::Vec2 getOffset() { return cocos2d::Vec2(xOffset, yOffset); }
        const cocos2d::Vec2 *getPolygon() { return points; }

    public:
        MapBody(tinyxml2::XMLElement *mapbody_element);


    private:
        void loadMapBody(tinyxml2::XMLElement *mapbody_element);
        void polygonParse(const std::string &polygonStr);

    private:
        cocos2d::Vec2 *points;
        int size;
        float xOffset;
        float yOffset;

        bool isload;
};

#endif // MAPBODY_H
