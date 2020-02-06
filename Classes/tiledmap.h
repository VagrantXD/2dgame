/* tiledmap.h */

#ifndef TILEDMAP_H
#define TILEDMAP_H

#include <string>

#include "mapbody.h"

class TiledMap : public cocos2d::Node {

    public:
        const cocos2d::Vec2 getMapSize() { return cocos2d::Vec2(width, height); }
        const cocos2d::Vec2 getTileSize() { return cocos2d::Vec2(tilewidth, tileheight); }

        float getSizeX() { return width * tilewidth; }
        float getSizeY() { return height * tileheight; }

        const cocos2d::Rect getVisibleArea() { return visibleArea; }
        bool isVisibleArea() { return !visibleArea.equals( cocos2d::Rect::ZERO ); }
        
        const cocos2d::Vec2 *getMapBody() { return mapBody.points; }
        int getMapBodySize() { return mapBody.size; }
        const cocos2d::Vec2 getMapBodyOffset() { return cocos2d::Vec2(mapBody.xOffset, mapBody.yOffset); }
        
    public:
        static TiledMap *create(const std::string &tmxFileName);

        // Дополнительные загружаемые параметры
        //
        bool loadObjects();
        bool loadVisibleArea();
        bool loadMapBody();
         

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
        void polygonParse(const std::string &polygonStr);

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

        struct Layer {
            std::string name;
            int **sheet;
            std::vector< cocos2d::Sprite * > sprites;
        } layer;

        struct MapBody {
            cocos2d::Vec2 *points;
            int size;
            float xOffset;
            float yOffset;
        } mapBody;

        tinyxml2::XMLDocument *doc;
        std::vector< tinyxml2::XMLElement *> objects;

        std::string tmxPath;
};

#endif // TILEDMAP_H
