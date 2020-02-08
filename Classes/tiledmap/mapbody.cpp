/* mapbody.cpp */

#include <iostream>

#include "mapbody.h"

namespace xml = tinyxml2;

MapBody::MapBody(xml::XMLElement *mapbody_element, float ySize) 
    :points(nullptr),
     size(0), 
     xOffset(0.0f),
     yOffset(0.0f),
     isloading(false)
{
    loadMapBody(mapbody_element, ySize);
}

MapBody::~MapBody() {
    delete [] points;
}

void MapBody::loadMapBody(xml::XMLElement *mapbody_element, float ySize) {
    if(mapbody_element == nullptr) {
        isloading = false;
        return;
    }
    
    // Потенциально опасное место
    xOffset = mapbody_element->FloatAttribute("x");
    yOffset = mapbody_element->FloatAttribute("y");

    mapbody_element = mapbody_element->FirstChildElement("polygon");

    if(mapbody_element == nullptr) {
        isloading = false;
        return;
    }

    // Потенциально опасное место
    std::string polygonStr = mapbody_element->Attribute("points");
     
    polygonParse(polygonStr);
    transformMapBody(ySize);

    isloading = true;
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

void MapBody::transformMapBody(float ySize) {
    float height = 0.0f; 

    for(int i = 0; i < size; ++i) {
        if(points[i].y > height) {
            height = points[i].y;
        }
    }

    yOffset = ySize - yOffset - height;

    for(int i = 0; i < size; ++i) {
      points[i].y = height - points[i].y;
    }
}

