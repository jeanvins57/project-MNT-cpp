/**
 * @file PPM.h
 * @author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * @brief  Functions generating raster in PPM file format.
 * @version 0.1
 * @date 2021-01-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PPM_H__
#define __PPM_H__

#include <fstream>
#include <iostream>
#include <cmath>
#include "toolbox.h"

using namespace std;

/**
 * \class PPM
 * \brief Object PPM.
 * \author Jean-Vincent KLEIN
 * \version 0.1
 * \date 2021
 *
 */
class PPM
/**
 * \brief creates PPM image.
 */

{
    public:
        ~PPM();
        PPM(int height, int width);
        PPM() {};

        /**
         * \struct RGB
         * \brief Objet char.
         *
         */
        struct RGB{
            unsigned char r;
            unsigned char g;
            unsigned char b;
        };
            
        void set_width(int width) {this->width = width;}
        void set_height(int height) {this->height = height;}
        void set_version(string version) {this->version = version;}

        void save(string name_file);
        void delete_image();

        RGB **image = nullptr;
        void write_2_PPM(int width,int heigth,Coords *coordinates, string name_file);

    private:
        int width = 0;
        int height = 0;
        string version = "P6";
        void create_image();
};

#endif