/* layer.h */

#ifndef LAYER_H
#define LAYER_H

#include "cocos2d.h"
#include "../tinyxml2/tinyxml2.h"

class Tileset;

class Layer : public cocos2d::Node {

    public:
        const std::string &getName() const { return name; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        int **getSheet() const { return sheet; }

        bool isLoading() const { return isloading; }

    public:
        static Layer *create(tinyxml2::XMLElement *layer_element);

        void createSpritesLayer(const Tileset *tileset);

    private:
        Layer(tinyxml2::XMLElement *layer_element);
        ~Layer();

    private:
        void loadLayer(tinyxml2::XMLElement *layer_element);

        int **csvParse(const char *csv);

    private:
        std::string name;
        int width;
        int height;

        int **sheet;
        std::vector < cocos2d::Sprite * > sprites;

        bool isloading;
};

#endif //LAYER_H
