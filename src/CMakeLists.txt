cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Information Kruncher Custom Web Server")

# executable to compile
add_executable(
	infokruncher 
	infokruncher.cpp
)

target_include_directories(
	infokruncher
	PRIVATE
	../include
)

add_subdirectory( db )

