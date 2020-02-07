/* layer.h */

#ifndef LAYER_H
#define LAYER_H

#include "cocos2d.h"
#include "../tinyxml2/tinyxml2.h"

class Layer {
    public:
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
