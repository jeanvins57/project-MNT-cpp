Jean-Vincent KLEIN - FISE2022
see gitlab rep : https://gitlab.ensta-bretagne.fr/kleinje/cpp__projet_mnt

Project for M. ROHOU's C++ class.
Please find instructions here : http://simon-rohou.fr/cours/c++/doc/06/cpp_06_projet_carte.pdf (french)

External library required : PROJ. visit proj.org/install.html

The program reads a MNT file using WGS84 coordinates. It converts them in the Lambert 93 coordinates (popular in France).

It then generates two rasters: one in black and white, another one in colours. 

Problems identified: 
- A Delaunay's triangulation should be implemented in order to have a raster with matching coordinates. I did not do it in this version as the data was giving a clear result already. Assumption: height = width.
- The progress bar slows down considerably the program. 
- The HillShade has been implemented but is not working properly yet.
- Core dumps can occur for width = 1080
- No unitary test


