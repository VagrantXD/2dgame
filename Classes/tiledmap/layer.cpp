/* layer.cpp */

#include "layer.h"

namespace xml = tinyxml2;

Layer::Layer(xml::XMLElement *layer_element)
    :isloading(false),
     name(""),
     width(0),
     height(0),
     sheet(nullptr)
{
    loadLayer(layer_element);
}

Layer::~Layer() {
    for(int i = 0; i < height; ++i)
        delete [] sheet[i];
    delete [] sheet;
}

void Layer::loadLayer(xml::XMLElement *layer_element) {
    if(layer_element == nullptr) {
        return;   // isloading == false
    }

    name = layer_element->Attribute("name");
    width = layer_element->IntAttribute("width");
    height = layer_element->IntAttribute("height"); 

    layer_element = layer_element->FirstChildElement("data");

    if(layer_element == nullptr) {
        return; // isloading == false
    }

    const char * csv = layer_element->GetText(); 

    if(csv == nullptr) {
        return; // isloading == false
    }

    sheet = csvParse(csv); 

    isloading = true;
}

int **Layer::csvParse(const char *csv) {
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

