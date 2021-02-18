/**
 * @file toolbox.cpp
 * @author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * @brief Functions used widely for generation of both PPM and PGM file formats.
 * @version 0.1
 * @date 2021-01-14
 */

#include <stdio.h>
#include <proj.h>
#include <iostream>
#include <fstream>
#include <string>

#include "toolbox.h"

/**
 * \fn void wgs84_to_L93(Coords *coordinates)
 * \brief Function reading WGS84 data from MNT file and converting values to Lambert 93.
 *  
 * \param myfile File to read.
 * \param coordinates Structures Coords containing x,y and z values.
 */
int wgs84_to_L93(const string &myfile,Coords *coordinates) {
    PJ_CONTEXT *C;
    PJ *P;
    PJ* P_for_GIS;
    PJ_COORD b;
    C = proj_context_create();
    P = proj_create_crs_to_crs (C,
                                "EPSG:4326", // WGS84 system
                                "EPSG:2154", // L93 system
                                NULL);
    if (0==P) {
        fprintf(stderr, "Oops\n");
        return 0;
    }
    P_for_GIS = proj_normalize_for_visualization(C, P);
    if( 0 == P_for_GIS )  {
        fprintf(stderr, "Oops\n");
        return 1;
    }
    proj_destroy(P);
    P = P_for_GIS;

    string line;
    ifstream ifst(myfile); //("../Guerledan_Feb19_50cm_wgs84.txt");
    ofstream myfile2 ("../other/Guerledan_Feb19_50cm_L93.txt");
    float p = 0;
    std::cout << "Reading file" << endl;
    if (ifst.is_open() && myfile2.is_open()) {
        while(!ifst.eof()) {
            p++;
            printProgress(p/2681721);
            getline(ifst,line);
            istringstream iss(line);    // conversion from str to int
            double lat, lon, alt;
            iss >> lat >> lon >> alt;
            PJ_COORD a00 = proj_coord (lon, lat, 0, 0);
            b = proj_trans (P, PJ_FWD, a00);
            myfile2 << b.enu.e << " "<< b.enu.n <<"\n";
            coordinates->x.push_back(b.enu.e);
            coordinates->y.push_back(b.enu.n); 
            coordinates->z.push_back(alt);
        }
    cout << " Done !" << endl;
    ifst.close();
    myfile2.close();
    }
    else cout << "Unable to open file"; 
    proj_destroy (P);
    return EXIT_SUCCESS;
}

/**
 * \fn double min(vector<double> &v)
 * \brief Function returning minimum value of a vector.
 *  
 * \param v Vector with double elements.
 */
double min(vector<double> &v)
{
	return *std::min_element(v.begin(), v.end());
}

/**
 * \fn double max(vector<double> &v)
 * \brief Function returning maximum value of a vector.
 *  
 * \param v Vector with double elements.
 */
double max(vector<double> &v)
{
	return *std::max_element(v.begin(), v.end());	
}

/**
 * \fn void altitude_2_rgb(int rgb[3], float val)
 * \brief Function converting float value to table of 3 int values RGB. (short Rainbow RGB algorithm)
 * \author particleincell.com/2014/colormap/ (edited)
 * \param rgb Table of 3 int elements.
 * \param val Table of 3 float elements.
 */
void altitude_2_rgb(int rgb[3], float val)
{
	rgb[0] = 255; rgb[1] = 255; rgb[2] = 255;
	double f = float(val)/float(255); // from 0 to 1
	double a = f/0.25; // invert and group, 0 coressponds to blue and 1 to red. 
	int X = floor(a); // this is the integer part
	int Y = floor(255*(a-X)); //Y part from 0 to 255
	switch(X)
	{
		case 0: // Red is maximum (255), blue is zero, and green varies linearly from 0 to 255.
			rgb[0] = 255;
			rgb[1] = Y;
			rgb[2] = 0;
			break;
		case 1: // Green is kept maximum and blue is still zero. Red varies linearly from 255 to 0.
			rgb[0] = 255-Y;
			rgb[1] = 255;
			rgb[2] = 0;
			break;
		case 2: // Keep red at zero and green and maximum. Blue varies linearly from 0 to 255.
			rgb[0] = 0;
			rgb[1] = 255;
			rgb[2] = Y;
			break;
		case 3: // Red is kept at zero and blue at maximum. Green varies linearly from 255 to 0.
			rgb[0] = 0;
			rgb[1] = 255-Y;
			rgb[2] = 255;
			break;
		case 4: // Blue is maximum.
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 255;
			break;
	}
}

/**
 * \fn void shift_data(Coords *coordinates) 
 * \brief Shift data for cross-multiplication 
 * 
 * \param coordinates Structures Coords containing x,y and z values.
 */
void shift_data(Coords *coordinates) 
{
	double min_x = min(coordinates->x);
	double min_y = min(coordinates->y);
	double min_z = min(coordinates->z);
	for(unsigned int i = 0; i < coordinates->x.size(); i++)
	{
		coordinates->x[i] = coordinates->x[i] - min_x;
		coordinates->y[i] = coordinates->y[i] - min_y;
		coordinates->z[i] = coordinates->z[i] - min_z;
	}
}

/**
 * @fn void rgb_2_hsv(int rgb[3],double hsv[3])
 * @brief Change color system from RGB to HSV.
 * 
 * @param rgb Table of 3 int elements.
 * @param hsv Table of 3 double elements.
 */
void rgb_2_hsv(int rgb[3],double hsv[3]) 
{ 
    double r = rgb[0];
    double g = rgb[1];
    double b = rgb[2];

    // R, G, B values are divided by 255 
    // to change the range from 0..255 to 0..1 
    r = r / 255.0; 
    g = g / 255.0; 
    b = b / 255.0; 

    // h, s, v = hue, saturation, value 
    double cmax = max(r, max(g, b)); // maximum of r, g, b 
    double cmin = min(r, min(g, b)); // minimum of r, g, b 
    double diff = cmax - cmin; // diff of cmax and cmin. 
    double h = -1;
    double s = -1; 
        
    // if cmax and cmax are equal then h = 0 
    if (cmax == cmin) 
        h = 0; 

    // if cmax equal r then compute h 
    else if (cmax == r) 
        h = fmod( (60 * ((g - b) / diff) + 360) , 360); 

    // if cmax equal g then compute h 
    else if (cmax == g) 
        h = fmod( (60 * ((b - r) / diff) + 120) , 360); 

    // if cmax equal b then compute h 
    else if (cmax == b) 
        h = fmod( (60 * ((r - g) / diff) + 240) , 360); 

    // if cmax equal zero 
    if (cmax == 0) 
        s = 0; 
    else
        s = (diff / cmax) * 100; 

    // compute v 
    double v = cmax * 100;

    hsv[0] = h;hsv[1] = s;hsv[2] = v;
} 

/**
 * @fn hsv_2_rgb(double hsv[3],int rgb[3])
 * @brief Change color system from HSV to RGB.
 * 
 * @param hsv Table of 3 double elements.
 * @param rgb Table of 3 int elements.
 */
void hsv_2_rgb(double hsv[3],int rgb[3]){    
    double H = hsv[0];
    double S = hsv[1];
    double V = hsv[2];
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        cout<<"The given HSV values are not in valid range"<<endl;
        return;
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    rgb[0] = (r+m)*255;
    rgb[1] = (g+m)*255;
    rgb[2] = (b+m)*255;
}

/**
 * @fn vector<vector<double>> init_matrix(int height,int width)
 * @brief Initialize a matrix 2D of size heightxwidth - with zeros. 
 * 
 * @param height 
 * @param width 
 * @return vector<vector<double>> 
 */
vector<vector<double>> init_matrix(int height,int width)
{
    vector< vector<double>> matrix;
    for(int i = 0; i <= height; i++)
    {
        vector<double> matrix2;
        for(int j = 0; j < width; j++)
        {
            matrix2.push_back(0);
        }
        matrix.push_back(matrix2);
    }
    return matrix;
}

/**
 * @fn void set_V(float hs, double hsv[3])
 * @brief Setter for V value in a table HSV and a corresponding hillShade computed previously.
 * 
 * @param hs HillShade
 * @param hsv Table of 3 double elements.
 */
void set_V(float hs, double hsv[3])
{
	hsv[2] = (hs/255+0.6)*100;
	if(hsv[2]>100)
	{
		hsv[2] = 100;
	}
}

/**
 * @fn int compute_hillshade(double a, double b, double c, double d, double e, double f, double g, double h, double i, float azim, float alt)
 * @brief Computes hillshade based on Pixel colors around actual Pixel.
 * Based on : https://pro.arcgis.com/en/pro-app/latest/tool-reference/3d-analyst/how-hillshade-works.htm
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 * @param e 
 * @param f 
 * @param g 
 * @param h 
 * @param i 
 * @param azim 
 * @param alt 
 * @return int 
 */
int compute_hillshade(double a, double b, double c, double d, double f, double g, double h, double i, float azim, float alt)
{
	float zenith_deg = 90 - alt;
	float zenith_rad = (zenith_deg*M_PI)/180;
	float azimuth_math = 360 - azim + 90;
	if(azimuth_math>=360) {azimuth_math = azimuth_math - 360;}
	float azimuth_rad = (azimuth_math*M_PI)/180;
	int cellSize = 30; 
	float dzdx = ((c+2*f+i)-(a+2*d+g))/(8*cellSize);
	float dzdy = ((g+2*h+i)-(a+2*b+c))/(8*cellSize);
	float slope_rad = atan(1*sqrt((dzdx*dzdx)+(dzdy*dzdy)));
	float Aspect_rad = 0;
	if(dzdx!=0)
	{
		Aspect_rad = atan2(dzdy, -dzdx);
		if(Aspect_rad<0)
		{
			Aspect_rad = 2*M_PI + Aspect_rad;
		}
	}
	else
	{
		if(dzdy>0) {Aspect_rad = M_PI/2;}
		else if(dzdy<0) {Aspect_rad = 2*M_PI - M_PI/2;}
		else {Aspect_rad = Aspect_rad;}
	}
	float hs = 255*(cos(zenith_rad)*cos(slope_rad) + sin(zenith_rad)*sin(slope_rad)*cos(azimuth_rad-Aspect_rad));
	return hs;
}

/**
 * @fn void printProgress(double percentage)
 * @brief Status/Progress bar to make user wait while work in progress
 * Caution: Slows down the processes. Needs to be optimized.  
 * 
 * @param percentage 
 */
void printProgress(double percentage) 
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}