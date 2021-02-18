/**
 * @file PGM.cpp
 * @author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * @brief Functions generating raster in PGM file format. 
 * @version 0.1
 * @date 2021-01-14
 */
#include <fstream>
#include <iostream>
#include <cmath>
#include "PGM.h"
#include "toolbox.h"

/**
 * \fn void write_2_PGM(int width, int height, Coords *coordinates, string name)
 * \brief Write scaled data to a Portable Greyscale Map (PGM) image.
 *  
 * \param width Width of desired PGM file.
 * \param height Height of desired PGM file.
 * \param coordinates Structure Coordinates containing x,y,z in L93 system.
 * \param name Name of the name. 
 */
void write_2_PGM(int width, int height, Coords *coordinates, string name)
{
	cout << "\nGenerating PGM file" << endl;
	ofstream ofst(name);
	if(!ofst.is_open())
	{
		cout << "Unable to open" << name << endl;				
	}
	else
	{
		double max_x = max(coordinates->x);	// Getting max values of x, y, z.
		double max_y = max(coordinates->y);	
		double max_z = max(coordinates->z);

		std::vector<vector<double>> data_matrix = init_matrix(height,width);	// Initialize Matrix to fill (first filled with zeros)

		float p = 0;
		for(unsigned int k = 0; k < coordinates->x.size(); k++) 				// Filling the matix
		{
			int i = floor(coordinates->x[k]*width/max_x);			// using cross-multiplication.
			int j = floor(coordinates->y[k]*height/max_y);
			data_matrix[i][j] = floor(coordinates->z[k]*(255/max_z)); 
			p++;
            printProgress(p/2681721);
		}
		ofst <<"P2" << endl;										// PGM id.
		ofst << width << " " << height << endl;
		ofst << 255 << " " << endl;
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				ofst << data_matrix[i][j] << " " ;					// write matrix to PGM.
			}
			ofst << endl;
		}
		cout << " Done !" << endl;
		ofst.close();												// end. 
	}
}