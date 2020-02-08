/* visiblearea.cpp */

#include "visiblearea.h"

VisibleArea::VisibleArea(tinyxml2::XMLElement *visiblearea_element, float ySize) 
    : area( cocos2d::Rect::ZERO )
{
    isloading = loadVisibleArea(visiblearea_element, ySize);
}

bool VisibleArea::loadVisibleArea(tinyxml2::XMLElement *visiblearea_element, float ySize) {
    if(visiblearea_element == nullptr)
        return false;

    float x = visiblearea_element->FloatAttribute("x");
    float y = visiblearea_element->FloatAttribute("y");
    float width = visiblearea_element->FloatAttribute("width");
    float height = visiblearea_element->FloatAttribute("height");

    float transformY = ySize - (height + y);   // Трансформация y координаты

    area.setRect(x, transformY, width, height);

    if( area.equals( cocos2d::Rect::ZERO ) )
        return false;

    return true;
}
