/* mapbody.h */

#ifndef MAPBODY_H
#define MAPBODY_H

#include "tinyxml2/tinyxml2.h"
#include "cocos2d.h"

class MapBody {

    public:    // property
        bool isLoading() const { return isloading; }
        int getSize() const { return size; }
        cocos2d::Vec2 getOffset() const { return cocos2d::Vec2(xOffset, yOffset); }
        const cocos2d::Vec2 *getPolygon() const { return points; }

    public:
        MapBody(tinyxml2::XMLElement *mapbody_element);
        ~MapBody();

        void transformMapBody(int mapHeight);

    private:
        void loadMapBody(tinyxml2::XMLElement *mapbody_element);
        void polygonParse(const std::string &polygonStr);

    private:
        cocos2d::Vec2 *points;
        int size;
        float xOffset;
        float yOffset;

        bool isloading;
};

#endif // MAPBODY_H
