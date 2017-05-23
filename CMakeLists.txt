cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project( mag )
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 4)
#valgrid memtest
#find_program(VALGRIND "valgrind")
#include (CTest)
set(Boost_INCLUDE_DIR /usr/local/lib/boost_1_64_0)
find_package( OpenCV REQUIRED )
find_package( Boost COMPONENTS program_options REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ./src )
set(HEADER_FILES ./src/main.h ./src/vecOfMeanLns.h ./src/usefulFun.h ./src/mainFun.h)
set(SOURCES ./src/vecOfMeanLns.cpp ./src/usefulFun.cpp ./src/mainFun.cpp) #${PROJECT_SOURCE_DIR}/
add_executable( launcher ./src/main.cpp ${HEADER_FILES} ${SOURCES})
#valgrid memtest
#add_test(example_test launcher)
#check compiler support for C++11
set_property(TARGET launcher PROPERTY CXX_STANDARD 11)
set_property(TARGET launcher PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries( launcher ${OpenCV_LIBS} ${Boost_LIBRARIES})

#check compiler support for C++11
#message("Your CMake version supports these C++ features:")
#get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
#foreach(i ${known_features})
#  message("${i}")
#endforeach()


