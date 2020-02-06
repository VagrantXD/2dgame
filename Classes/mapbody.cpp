/* mapbody.cpp */

#include "mapbody.h"

namespace xml = tinyxml2;

MapBody::MapBody(xml::XMLElement *mapbody_element) 
    :points(nullptr),
     size(0), 
     xOffset(0.0f),
     yOffset(0.0f),
     isload(false)
{
    loadMapBody(mapbody_element);
}

void MapBody::loadMapBody(xml::XMLElement *mapbody_element) {
    if(mapbody_element == nullptr) {
        isload = false;
        return;
    }
    
    // Потенциально опасное место
    xOffset = mapbody_element->FloatAttribute("x");
    yOffset = mapbody_element->FloatAttribute("y");

    mapbody_element = mapbody_element->FirstChildElement("polygon");

    if(mapbody_element == nullptr) {
        isload = false;
        return;
    }

    // Потенциально опасное место
    std::string polygonStr = mapbody_element->Attribute("points");
     
    polygonParse(polygonStr);
    isload = true;
}

void MapBody::polygonParse(const std::string &polygonStr) {
    int start = 0;
    int pos = 0; 
    
    std::vector< float > n;

    while( ( pos = polygonStr.find_first_of(" ,", pos + 1) ) != std::string::npos ) {
        n.push_back( std::stof(polygonStr.substr(start, pos - start)) );
        start = pos + 1;
    }

    n.push_back( std::stof(polygonStr.substr(start)) );

    size = n.size() / 2;
    points = new cocos2d::Vec2[ size ];

    for(int i = 0; i < n.size(); ++i)
        if(i % 2 == 0)
            points[i / 2].x = n[i];
        else
            points[i / 2].y = n[i];
}
