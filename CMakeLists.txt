cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project( magisterka )
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 3)
#valgrid memtest
#find_program(VALGRIND "valgrind")
#include (CTest)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ./src )
set(HEADER_FILES ./src/vecOfMeanLns.h ./src/usefulFun.h ./src/mainFun.h)
set(SOURCES ./src/vecOfMeanLns.cpp ./src/usefulFun.cpp ./src/mainFun.cpp) #${PROJECT_SOURCE_DIR}/
add_executable( launcher ./src/main.cpp ${HEADER_FILES} ${SOURCES})
#valgrid memtest
#add_test(example_test launcher)
#check compiler support for C++11
set_property(TARGET launcher PROPERTY CXX_STANDARD 11)
set_property(TARGET launcher PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries( launcher ${OpenCV_LIBS})

#check compiler support for C++11
#message("Your CMake version supports these C++ features:")
#get_property(known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
#foreach(i ${known_features})
#  message("${i}")
#endforeach()


