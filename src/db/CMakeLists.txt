cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Information Kruncher Custom Data Server")

add_executable(
	kruncherdb 
	Data.cpp
	DataKruncher.cpp
	DataTools.cpp
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






add_executable(
	krunchdbunittester 
	ut.cpp
)

target_include_directories( 
	krunchdbunittester 
	PRIVATE
	../../../exexml
	../../include
	include
	/usr/local/include/db4/
)


target_link_directories( 
	krunchdbunittester 
	PRIVATE
	/usr/local/lib/db4/
)

target_link_libraries( 
	krunchdbunittester 
	pthread
)


add_executable(
	dbtests 
	dbtests.cpp
)

target_include_directories( 
	dbtests 
	PRIVATE
	../../include
	include
	/usr/local/include/db4/
)


target_link_directories( 
	dbtests 
	PRIVATE
	/usr/local/lib/db4/
)

target_link_libraries( 
	dbtests 
	db_cxx
)

