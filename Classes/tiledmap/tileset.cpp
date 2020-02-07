/* tileset.cpp */

#include "tileset.h"

namespace xml = tinyxml2;

Tileset::Tileset(xml::XMLElement *tileset_element, const std::string &tmxParentDirectory) 
    : isloading(false),
      textureFilePath(""),
      source(""),
      width(0),
      height(0),
      tilewidth(0),
      tileheight(0)
{
    loadTileset(tileset_element, tmxParentDirectory);
}

Tileset::~Tileset() {

}

const cocos2d::Rect Tileset::textureRect(int gid) const {
    int width_texture_count = width / tilewidth;
    int height_texture_count = height / tileheight;

    gid--;

    int row = gid / width_texture_count;
    int col = gid % width_texture_count;

    int start_x = col * tilewidth;
    int start_y = row * tileheight;

    return cocos2d::Rect(start_x, start_y, tilewidth, tileheight);
} 

void Tileset::loadTileset(xml::XMLElement *tileset_element, const std::string &tmxParentDirectory) {
    if(tileset_element == nullptr) {
        return; // isloading == false
    }

    source = tileset_element->Attribute("source");

    xml::XMLDocument *tilesetDocument = new xml::XMLDocument;

    if( tilesetDocument->LoadFile( (tmxParentDirectory + "/" + source).c_str() ) ) {
        return;  // tileset tsx file not find      isloading == false
    }

    auto tsx_tileset_element = tilesetDocument->FirstChildElement("tileset");

    if(tsx_tileset_element == nullptr) {
        return; // isloading == false
    }
    
    //  load tileset property
    tilewidth = tsx_tileset_element->IntAttribute("tilewidth");
    tileheight = tsx_tileset_element->IntAttribute("tileheight");

    tsx_tileset_element = tsx_tileset_element->FirstChildElement("image");

    if(tsx_tileset_element == nullptr) {
        return; // isloading == false
    }

    std::string image_source = tsx_tileset_element->Attribute("source");
    width = tsx_tileset_element->IntAttribute("width");
    height = tsx_tileset_element->IntAttribute("height");

    textureFilePath = tmxParentDirectory + "/" + image_source;

    isloading = true;

    delete tilesetDocument;
}
