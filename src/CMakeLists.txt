cmake_minimum_required(VERSION 3.10)

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

