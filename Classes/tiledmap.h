/* tiledmap.h */

#ifndef TILEDMAP_H
#define TILEDMAP_H

#include <string>

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"


class TiledMap : public cocos2d::Node {

    public:
        const cocos2d::Vec2 getMapSize() { return cocos2d::Vec2(width, height); }
        const cocos2d::Vec2 getTileSize() { return cocos2d::Vec2(tilewidth, tileheight); }

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

        int **csvParse(const char *csv);
        const cocos2d::Rect textureRect(int gid);

        std::string parentPath(const std::string &path);
        std::string removeExtension(const std::string &path);

    private:
        int width;
        int height;
        int tilewidth;
        int tileheight;

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

        tinyxml2::XMLDocument *doc;
        std::vector< tinyxml2::XMLElement *> objects;

        std::string tmxPath;
};

#endif // TILEDMAP_H
