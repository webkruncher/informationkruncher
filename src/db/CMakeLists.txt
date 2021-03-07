cmake_minimum_required(VERSION 3.10)

project("Information Kruncher Custom Data Server")

#DB_INCLUDE=../libdb/build_unix
#LIBS= -L../libdb/build_unix -L../libdb/build_unix/.libs/ -ldb_cxx -lpthread

#INC= -I. -I$(DB_INCLUDE)
#CFLAGS= $(INC) -W -Wall


# executable to compile
add_executable(
	kruncherdb 
	Data.cpp
	DataKruncher.cpp
)

target_include_directories( 
	kruncherdb
	PRIVATE
	../../../exexml
	../../../libdb/build_unix
)


target_link_directories( 
	kruncherdb
	PRIVATE
	../../../libdb/build_unix/.libs
)

target_link_libraries( 
	kruncherdb
	pthread
	db_cxx 
)

