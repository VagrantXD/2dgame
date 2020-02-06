/* tiledmap.cpp */

#include <iostream>

#include "tiledmap.h"

using namespace tinyxml2;

USING_NS_CC;


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

    if( visibleArea.equals( Rect::ZERO ) )
        return false;

    return true;
}

bool TiledMap::loadMapBody() {
    XMLElement *mapbody_element = findObject("map_body");

    if(mapbody_element == nullptr)
        return false;
    
    mapBody.xOffset = mapbody_element->FloatAttribute("x");
    mapBody.yOffset = mapbody_element->FloatAttribute("y");

    mapbody_element = mapbody_element->FirstChildElement("polygon");

    if(mapbody_element == nullptr)
        return false;

    // Потенциально опасное место
    std::string polygonStr = mapbody_element->Attribute("points");
     
    polygonParse(polygonStr);

    return true;
}

TiledMap::TiledMap() 
    : visibleArea( Rect::ZERO )
{

}

TiledMap::~TiledMap() {
    delete doc;
  //  delete [] mapBody.points;
}

void TiledMap::initWithFileName(const std::string &tmxFileName) {
    tmxPath = FileUtils::getInstance()->fullPathForFilename(tmxFileName);

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
    std::string tileset_source = tileset_element->Attribute("source"); 

    XMLDocument *tilesetDocument = new XMLDocument;

    if( tilesetDocument->LoadFile( (parentPath(tmxPath) + "/" + tileset_source).c_str() ) )
        std::cout << tmxPath + "/" + tileset_source << std::endl; // тсключение

    auto tsx_tileset_element = tilesetDocument->FirstChildElement("tileset");

    auto image_element = tsx_tileset_element->FirstChildElement("image");

    tileset.source = image_element->Attribute("source");
    tileset.width = image_element->IntAttribute("width");
    tileset.height = image_element->IntAttribute("height");

    delete tilesetDocument;
}

void TiledMap::loadLayersSettings() {
    XMLElement *layer = doc->FirstChildElement("map")->FirstChildElement("layer"); 
    XMLElement *data = layer->FirstChildElement("data");

    this->layer.name = layer->Attribute("name");
    const char *csv = data->GetText();
    this->layer.sheet = csvParse(csv);
}

void TiledMap::loadMapUsingSettings() {
    std::string parentDirectoryPath = parentPath(tmxPath);
    std::string tilesetPath = parentDirectoryPath + "/" + tileset.source;

    auto texture_cache = Director::getInstance()->getTextureCache();
    auto tileset_texture = texture_cache->addImage( tilesetPath );

    for(int i = 0; i < width; ++i) {
        for(int j = 0; j < height; ++j) {
            int gid = layer.sheet[j][i];
            if( gid != 0 ) {
                auto sprite = Sprite::createWithTexture( tileset_texture, textureRect(gid) );
                sprite->setAnchorPoint( Vec2(0, 0) );
                sprite->setPosition( Vec2( i * tilewidth,  (height - j - 1) * tileheight) );  // пререворот карты

                addChild(sprite);

                layer.sprites.push_back( sprite );
            }
        }
    }

}

int **TiledMap::csvParse(const char *csv) {
    int **result = new int *[height];
    for(int i = 0; i < height; ++i)
        result[i] = new int[width];

    int count = 0;
    std::string number;
    
    while(*csv) {
        if( isdigit(*csv) ) {
            number.push_back(*csv);
        } else {
            if( !number.empty() ) {
                result[ count / width ][ count % width ] = std::stoi(number);
                count++;
                number.clear();
            }
        }
            
        csv++;
    }
    
    return result;
}

const Rect TiledMap::textureRect(int gid) {
    int width_texture_count = tileset.width / tilewidth;
    int height_texture_count = tileset.height / tileheight;

    gid--;

    int row = gid / width_texture_count;
    int col = gid % width_texture_count;

    int start_x = col * tilewidth;
    int start_y = row * tileheight;

    return Rect(start_x, start_y, tilewidth, tileheight);
} 

void TiledMap::polygonParse(const std::string &polygonStr) {
    int start = 0;
    int pos = 0; 
    
    std::vector< float > n;

    while( ( pos = polygonStr.find_first_of(" ,", pos + 1) ) != std::string::npos ) {
        n.push_back( std::stof(polygonStr.substr(start, pos - start)) );
        start = pos + 1;
    }

    n.push_back( std::stof(polygonStr.substr(start)) );

    mapBody.points = new Vec2[ n.size() / 2 ];
    mapBody.size = n.size() / 2;

    for(int i = 0; i < n.size(); ++i)
        if(i % 2 == 0)
            mapBody.points[i / 2].x = n[i];
        else
            mapBody.points[i / 2].y = n[i];
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
