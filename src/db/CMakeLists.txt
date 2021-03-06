cmake_minimum_required(VERSION 3.10)

project("Information Kruncher Custom Data Server")

# executable to compile
add_executable(kruncherdb "Data.cpp")

target_include_directories( 
	kruncherdb
	PRIVATE
	../../../exexml
)

