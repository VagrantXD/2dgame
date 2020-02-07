/* tiledmap.h */

#ifndef TILEDMAP_H
#define TILEDMAP_H

#include <string>

#include "mapbody.h"
#include "layer.h"

class TiledMap : public cocos2d::Node {

    public:
        const cocos2d::Vec2 getMapSize() { return cocos2d::Vec2(width, height); }
        const cocos2d::Vec2 getTileSize() { return cocos2d::Vec2(tilewidth, tileheight); }

        float getSizeX() { return width * tilewidth; }
        float getSizeY() { return height * tileheight; }

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
        void loadMapSettings();
        void loadTilesetSettings();
        void loadLayersSettings();

        // Загрузка карты с обязательными параметрами
        void loadMapUsingSettings();

        // Парсинг и прочии вспомогательные функции
        int **csvParse(const char *csv);
        const cocos2d::Rect textureRect(int gid);

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
 
    private:     // Для внутреннего использования
        struct Tileset {
            std::string source;
            int width;
            int height;
        } tileset; 

        Layer *layer;
        //std::vector < cocos2d::Sprite * > sprites;

        tinyxml2::XMLDocument *doc;
        std::vector< tinyxml2::XMLElement *> objects;

        std::string tmxPath;
};

#endif // TILEDMAP_H
