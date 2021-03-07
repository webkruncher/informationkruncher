cmake_minimum_required(VERSION 3.10)

project("Information Kruncher Custom Data Server")


add_executable(
	kruncherdb 
	Data.cpp
	DataKruncher.cpp
)

target_include_directories( 
	kruncherdb
	PRIVATE
	../../../exexml
	../../include
	include
	/usr/local/include/db4/
)


target_link_directories( 
	kruncherdb
	PRIVATE
	/usr/local/lib/db4/
)

target_link_libraries( 
	kruncherdb
	pthread
	db_cxx
)

