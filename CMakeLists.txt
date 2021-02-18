cmake_minimum_required(VERSION 3.0)
project(create_raster)

find_package(PROJ)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")


file(
	GLOB_RECURSE
	source_files
	src/*.[ch]pp
)

add_executable(
	create_raster
	${source_files}
)

target_link_libraries(create_raster ${PROJ_LIBRARIES})

