cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project( magisterka )
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} ./src )
set(HEADER_FILES ./src/strLines.h)
set(SOURCES ./src/strLines.cpp) #${PROJECT_SOURCE_DIR}/
add_executable( launcher ./src/main.cpp ${HEADER_FILES} ${SOURCES})
target_link_libraries( launcher ${OpenCV_LIBS})
