/* tiledmap.h */

#ifndef TILEDMAP_H
#define TILEDMAP_H

#include <string>

#include "mapbody.h"

class Layer;
class Tileset;

class TiledMap : public cocos2d::Node {

    public:
        const cocos2d::Vec2 getMapSize() const { return cocos2d::Vec2(width, height); }
        const cocos2d::Vec2 getTileSize() const { return cocos2d::Vec2(tilewidth, tileheight); }

        float getSizeX() const { return width * tilewidth; }
        float getSizeY() const { return height * tileheight; }

        bool isLoading() const { return isloading; }

        const cocos2d::Rect getVisibleArea() { return visibleArea; }
        bool isVisibleAreaLoading() { return !visibleArea.equals( cocos2d::Rect::ZERO ); }
        
        const MapBody getMapBody();
        
    public:
        static TiledMap *create(const std::string &tmxFileName);

        // Дополнительные загружаемые параметры
        //
        bool loadObjects();
        bool loadVisibleArea();


    private:
        TiledMap();
        virtual ~TiledMap();
        void initWithFileName(const std::string &tmxFileName);

    private:
        // Обязательно должно быть загружено
        bool loadMapSettings();
        bool loadTilesetSettings();
        bool loadLayersSettings();

        // методы для работы с файловыми путями
        std::string parentPath(const std::string &path);
        std::string removeExtension(const std::string &path);

        // методы для поиска объектов в objects
        tinyxml2::XMLElement *findObject(const std::string &name);

    private:    // Для свойств
        int width;
        int height;
        int tilewidth;
        int tileheight;

        cocos2d::Rect visibleArea;

        bool isloading;
 
    private:     // Для внутреннего использования

        Tileset *tileset;

        Layer *layer;

        tinyxml2::XMLDocument *doc;
        std::vector< tinyxml2::XMLElement *> objects;

        std::string tmxPath;
};

#endif // TILEDMAP_H
