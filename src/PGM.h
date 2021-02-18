/**
 * @file PGM.h
 * @author Jean-Vincent KLEIN (jean-vincent.klein@ensta-bretagne.org)
 * @brief  Functions generating raster in PGM file format.
 * @version 0.1
 * @date 2021-01-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __PGM_H__
#define __PGM_H__

#include "toolbox.h"

void write_2_PGM(int width, int height, Coords *coordinates, string image);

#endif