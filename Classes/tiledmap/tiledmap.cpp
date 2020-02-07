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
        auto object_element = objectgroup_element->FirstChildElement(); 

        while(object_element) {
            objects.push_back(object_element); 

            object_element = object_element->NextSiblingElement();
        }
         
        objectgroup_element = objectgroup_element->NextSiblingElement();
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
    : visibleArea( cocos2d::Rect::ZERO )
{

}

TiledMap::~TiledMap() {
    delete doc;
    delete tileset;
}

void TiledMap::initWithFileName(const std::string &tmxFileName) {
    tmxPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(tmxFileName);

    doc = new tinyxml2::XMLDocument;

        if( doc->LoadFile(tmxPath.c_str()) )
            ; // исключение

    loadMapSettings();
    loadTilesetSettings();
    loadLayersSettings();

    loadMapUsingSettings();
}

void TiledMap::loadMapSettings() {
    XMLElement *map_element = doc->FirstChildElement("map");

    width = map_element->IntAttribute("width");
    height = map_element->IntAttribute("height");
    tilewidth = map_element->IntAttribute("tilewidth");
    tileheight = map_element->IntAttribute("tileheight");
}

void TiledMap::loadTilesetSettings() {
    XMLElement *tileset_element = doc->FirstChildElement("map")->FirstChildElement("tileset");

    tileset = new Tileset( tileset_element, parentPath(tmxPath) );

    /*std::string tileset_source = tileset_element->Attribute("source"); 

    XMLDocument *tilesetDocument = new XMLDocument;

    if( tilesetDocument->LoadFile( (parentPath(tmxPath) + "/" + tileset_source).c_str() ) )
        std::cout << tmxPath + "/" + tileset_source << std::endl; // тсключение

    auto tsx_tileset_element = tilesetDocument->FirstChildElement("tileset");

    auto image_element = tsx_tileset_element->FirstChildElement("image");

    tileset.source = image_element->Attribute("source");
    tileset.width = image_element->IntAttribute("width");
    tileset.height = image_element->IntAttribute("height");

    delete tilesetDocument; 
    */
}

void TiledMap::loadLayersSettings() {
    XMLElement *layer_element = doc->FirstChildElement("map")->FirstChildElement("layer");
    layer = Layer::create(layer_element); 
}

void TiledMap::loadMapUsingSettings() {
    std::string parentDirectoryPath = parentPath(tmxPath);
    std::string tilesetPath = tileset->getTextureFilePath();

    auto texture_cache = cocos2d::Director::getInstance()->getTextureCache();
    auto tileset_texture = texture_cache->addImage( tilesetPath );

    for(int i = 0; i < layer->getWidth(); ++i) {
        for(int j = 0; j < layer->getHeight(); ++j) {
            int gid = layer->getSheet()[j][i];
            if( gid != 0 ) {
                auto sprite = cocos2d::Sprite::createWithTexture( tileset_texture, textureRect(gid) );
                sprite->setAnchorPoint( cocos2d::Vec2(0, 0) );
                sprite->setPosition( cocos2d::Vec2( i * tilewidth,  (layer->getHeight() - j - 1) * tileheight) );// пререворот карты

                addChild(sprite);

                //sprites.push_back( sprite );
            }
        }
    }

}

const cocos2d::Rect TiledMap::textureRect(int gid) {
    int width_texture_count = tileset->getWidth() / tilewidth;
    int height_texture_count = tileset->getHeight() / tileheight;

    gid--;

    int row = gid / width_texture_count;
    int col = gid % width_texture_count;

    int start_x = col * tilewidth;
    int start_y = row * tileheight;

    return cocos2d::Rect(start_x, start_y, tilewidth, tileheight);
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
