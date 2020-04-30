cmake_minimum_required(VERSION 3.15)
project(Voxel_L_System)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# If compiling from scratch, the location to the Qt installation will have to be updated to match where it is installed
# on the machine being used for compilation

if(WIN32)
    set(CMAKE_PREFIX_PATH C:\\Qt\\Qt5.9.8\\5.9.8\\mingw53_32)
endif(WIN32)

if(UNIX)
    set(CMAKE_PREFIX_PATH /home/binybrion/Qt5.9.9/5.9.9/gcc_64)
endif(UNIX)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})

add_executable(Voxel_L_System main.cpp GUI/MainWindow.cpp GUI/MainWindow.h)

target_link_libraries(Voxel_L_System ${Qt5Widgets_LIBRARIES} ${Qt5Test_LIBRARIES})