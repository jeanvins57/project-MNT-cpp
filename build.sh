#!/bin/bash

rm -rf build
rm -rf other
mkdir build
mkdir other
cd build
cmake ..
make

# Insert MNT(WGS84) txt file in /src folder
./create_raster "../src/Guerledan_Feb19_50cm_wgs84.txt" 2000
cd ..

