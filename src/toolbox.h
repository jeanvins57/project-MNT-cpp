/**
 * @file toolbox.h
 * @author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * @brief Functions used widely for generation of both PPM and PGM file formats.
 * @version 0.1
 * @date 2021-01-14
 */

#ifndef __TOOLBOX_H__
#define __TOOLBOX_H__

#include <math.h> 
#include <iostream> 
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace std;

/**
 * @struct Coords   
 * @brief Structures contqining x,y,z values of data points
 * 
 */
struct Coords
{
    vector<double> x;
    vector<double> y;
    vector<double> z; 
};

int wgs84_to_L93(const string &myfile,Coords *coordinates);

double min(vector<double> &v);
double max(vector<double> &v);
void shift_data(Coords *coordinates);
vector<vector<double>> init_matrix(int height,int width);

void altitude_2_rgb(int rgb[3], float val);
void rgb_2_hsv(int rgb[3],double hsv[3]);
void hsv_2_rgb(double hsv[3], int rgb[3]);

int compute_hillshade(double a, double b, double c, double d, double f, double g, double h, double l, float azim, float alt);
void set_V(float hs, double hsv[3]);

#endif

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
void printProgress(double percentage);
