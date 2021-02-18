/**
 * @file PPM.cpp
 * @author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * @brief Functions generating raster in PPM file format.
 * @version 0.1
 * @date 2021-01-14
 */
#include <fstream>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "PPM.h"
#include "toolbox.h"

using namespace std;

/**
 * @fn PPM::~PPM()
 * @brief Destroy the PPM::PPM object
 * 
 */
PPM::~PPM(){
    delete_image();
}

/**
 * @fn PPM::PPM(int height, int width)
 * @brief Construct a new PPM::PPM object
 * 
 * @param height 
 * @param width 
 */
PPM::PPM(int height, int width){
    set_height(height);
    set_width(width);
    create_image();
}

/**
 * @fn void PPM::delete_image()
 * @brief delete content of PPM file.
 * 
 */
void PPM::delete_image(){
    if (image != nullptr){
        for (int i=0; i < height; i ++)
            delete image[i];
        delete image;
    }
}

/**
 * @fn void PPM::save(string name_file)
 * @brief Save the image structre in a PPM file format.
 * 
 * @param name_file name of the file.
 */
void PPM::save(string name_file){
    ofstream output(name_file,ios::binary);
    if (output.is_open()){
        output << version << endl;
        output << width << " " << height << endl;
        output << 255 << endl;

        for (int i=0; i < height; i++)
            for (int j=0; j < width; j++)
                output.write((char*) &image[i][j],sizeof(RGB));
        
        cout << " Done !\n" << endl;
        output.close();
    }
}

/**
 * @fn void PPM::create_image()
 * @brief Create blank PPM file
 * 
 */
void PPM::create_image(){

    if (image != nullptr)
        delete_image();

    image = new RGB*[height];
    for (int i = 0; i < height; i++){
        image[i] = new RGB[width];
        for (int j = 0;j<width;j++){
            image[i][j].r = 255;
            image[i][j].g = 255;
            image[i][j].b = 255;
        }
    }
} 

/**
 * @fn void PPM::write_2_PPM(int width,int heigth,Coords *coordinates)
 * @brief Write data to the Portable Pixmap file.
 * 
 * @param width Width of desired PGM file.
 * @param heigth Height of desired PGM file.
 * @param coordinates Structure Coordinates containing x,y,z in L93 system.
 */
void PPM::write_2_PPM(int width,int heigth,Coords *coordinates, string name_file){
    
    double max_x = max(coordinates->x);
    double max_y = max(coordinates->y);
    double max_z = max(coordinates->z);
    float azimuth = 315;    // set sun position
    float altitude = 45;    
    float p = 0;
    vector<vector<double>> data_matrix = init_matrix(height,width);

    cout << "\nGenerating PPM file" << endl;
    
    for(unsigned long int k = 0; k < coordinates->x.size(); k++)
    {
        p++; printProgress(p/2681721);
        int rgb0[3] = {0};
        double hsv0[3] = {0};

        int i = int(coordinates->x[k]*width/max_x)-1;       // need to optimize.
        if(i<=0) {i=1;}                                     // those conditions prevent core dump occurences.
        if(i>=width){i=(width-1);}  
        int j = int(coordinates->y[k]*heigth/max_y);
        if(j<=0){j=1;}
        if(j>=height){j=height;}

        data_matrix[i][j] = coordinates->z[k];

        altitude_2_rgb(rgb0, coordinates->z[k]*(255/max_z)); // setting up color map using short rainbow algorithm.        
        rgb_2_hsv(rgb0,hsv0); // RGB to HSV color system 
        
        // Hillshade computing
        float a = data_matrix[i-1][j-1]; // getting z value of elements for computing corresponding hillshade
        float b = data_matrix[i-1][j];
        float c = data_matrix[i-1][j+1];
        float d = data_matrix[i][j-1];
        float f = data_matrix[i][j+1];
        float g = data_matrix[i+1][j-1];
        float h = data_matrix[i+1][j];
        float l = data_matrix[i+1][j+1];
        float hillShade = (compute_hillshade(a, b, c, d, f, g, h, l, azimuth, altitude)); // compute hillshade value depending on neighbors pixels.
        if (hillShade<0) {hillShade = 0;} // hilshade remains > 0.
        set_V(hillShade,hsv0); // set V with corresponding hillshade.
        
        int rgb1[3] ={0};
        hsv_2_rgb(hsv0,rgb1); // going back to RGB colour system to fill in RGB structure. 
        image[i][j].r = rgb1[0]; // filling pointer image (struct RGB)
        image[i][j].g = rgb1[1]; // to be saved in a PPM file.
        image[i][j].b = rgb1[2];
    }
    set_version("P6");
    save(name_file);
}