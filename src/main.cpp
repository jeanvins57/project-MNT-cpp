/**
 * \file main.cpp
 * \brief Programme principal.
 * \author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * \version 0.1
 * \date 12 january 2021
 *
 * Program generating an image 2D (raster) of different altitudes measure of a 2.5D terrain. 
 *
 */

#include "toolbox.h"
#include "PPM.h"
#include "PGM.h"

using namespace std;

/**
 * \fn int main (void)
 * \brief Start program.
 *
 * \return EXIT_SUCCESS - Correct stop of program.
 */
int main (int argc,char const *argv[]) {
    if((argc=3)){
        cout << "\nProjet MNT -- Jean-Vincent KLEIN";
        cout << "\nFISE 2022\n\n";
        string myfile = argv[1];  /*!< Nom du fichier */
        int width = stoi(argv[2]);  /*!< Largeur souahitée */
        Coords coordinates;
        wgs84_to_L93(myfile, &coordinates); // read WGS84 coordinates, convert to L93, shift to scale.
        shift_data(&coordinates);
        write_2_PGM(width, width, &coordinates, "raster_b&w.pgm");
        PPM ppm1(width,width); 
        ppm1.write_2_PPM(width,width,&coordinates, "raster_colorized.ppm");
    }
    return EXIT_SUCCESS;
}

