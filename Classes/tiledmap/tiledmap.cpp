/* tiledmap.cpp */

#include <iostream>

#include "tiledmap.h"
#include "layer.h"
#include "tileset.h"


using namespace tinyxml2;

TiledMap *TiledMap::create(const std::string &tmxFileName) {
    TiledMap *ret = new TiledMap();
    ret->initWithFileName(tmxFileName);
    ret->autorelease();
    return ret;
}

bool TiledMap::loadObjects() {
    auto objectgroup_element = doc->FirstChildElement("map")->FirstChildElement("objectgroup");;
    if(!objectgroup_element) 
        return false;

    while(objectgroup_element) {
        auto object_element = objectgroup_element->FirstChildElement("object"); 

        while(object_element) {
            objects.push_back(object_element); 

            object_element = object_element->NextSiblingElement("object");
        }
         
        objectgroup_element = objectgroup_element->NextSiblingElement("objectgroup");
    }

    return true;
}

bool TiledMap::loadVisibleArea() {
    XMLElement *visiblearea_element = findObject("visible_area");

    if(visiblearea_element == nullptr)
        return false;

    float x = visiblearea_element->DoubleAttribute("x");
    float y = visiblearea_element->DoubleAttribute("y");
    float width = visiblearea_element->DoubleAttribute("width");
    float height = visiblearea_element->DoubleAttribute("height");

    float transformY = getSizeY() - (height + y);

    visibleArea.setRect(x, transformY, width, height);

    if( visibleArea.equals( cocos2d::Rect::ZERO ) )
        return false;

    return true;
}

const MapBody TiledMap::getMapBody() {
    XMLElement *mapbody_element = findObject("map_body");

    MapBody mb(mapbody_element);
    mb.transformMapBody( getSizeY() );

    return mb;
}

TiledMap::TiledMap() 
    : visibleArea( cocos2d::Rect::ZERO ),
      isloading(false)
{

}

TiledMap::~TiledMap() {
    delete doc;
    delete tileset;
}

void TiledMap::initWithFileName(const std::string &tmxFileName) {
    tmxPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(tmxFileName);

    doc = new tinyxml2::XMLDocument;

        if( doc->LoadFile(tmxPath.c_str()) ) {
            return;  // isloading == false
        }

    if( !loadMapSettings() )
        return;

    if( !loadTilesetSettings() )
        return;

    if( !loadLayersSettings() )
        return;

    isloading = true;
}

bool TiledMap::loadMapSettings() {
    XMLElement *map_element = doc->FirstChildElement("map");

    if(map_element == nullptr) {
        return false;
    }

    width = map_element->IntAttribute("width");
    height = map_element->IntAttribute("height");
    tilewidth = map_element->IntAttribute("tilewidth");
    tileheight = map_element->IntAttribute("tileheight");

    return true;
}

bool TiledMap::loadTilesetSettings() {
    XMLElement *tileset_element = doc->FirstChildElement("map")->FirstChildElement("tileset");

    tileset = new Tileset( tileset_element, parentPath(tmxPath) );

    if( tileset->isLoading() ) {
        auto texturecache = cocos2d::Director::getInstance()->getTextureCache();
        texturecache->addImage( tileset->getTextureFilePath() );
    }
    else {
        return false;     // Что сделать?
    }

    return true;
}

bool TiledMap::loadLayersSettings() {
    XMLElement *layer_element = doc->FirstChildElement("map")->FirstChildElement("layer");

    if(layer_element == nullptr) {
        return false;
    }

    while(layer_element) {
        auto layer = Layer::create(layer_element); 

        if( layer->isLoading() ) {
            layer->createSpritesLayer(tileset);

            addChild(layer);

        }
        else {
            return false;
        }

        layer_element = layer_element->NextSiblingElement("layer");
    }

    return true;
}

std::string TiledMap::parentPath(const std::string &path) {
    int pos = path.find_last_of("/");
    return path.substr(0, pos);
}

std::string TiledMap::removeExtension(const std::string &path) {
    int pos = path.find_last_of(".");
    return path.substr(0, pos);
}

XMLElement *TiledMap::findObject(const std::string &name) {
    for( auto &object : objects ) {
        auto name_attribute = object->FindAttribute("name");
        if(!name_attribute)
            return nullptr;

        std::string attributeName = name_attribute->Value();
        if(attributeName == name) {
            return object;
        }
    }

    return nullptr;
}
