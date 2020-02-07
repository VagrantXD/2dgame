/* tileset.h */

#ifndef TILESET_H
#define TILESET_H

#include <string>

#include "cocos2d.h"

#include "../tinyxml2/tinyxml2.h"

class Tileset {

    public:
        const std::string &getTextureFilePath() const { return textureFilePath; }
        const std::string &getSource() const { return source; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        int getTileWidth() const { return tilewidth; }
        int getTileHeight() const { return tileheight; }

        bool isLoading() const { return isloading; }

    public:
        Tileset(tinyxml2::XMLElement *tileset_element, const std::string &tmxParentDirectory);
        ~Tileset();

        const cocos2d::Rect textureRect(int gid) const;

    private:
        void loadTileset(tinyxml2::XMLElement *tileset_element, const std::string &tmxParentDirectory);

    private:
        std::string textureFilePath;
        std::string source;

        int width;
        int height;

        int tilewidth;
        int tileheight;

        bool isloading;
};

#endif // TILESET_H
