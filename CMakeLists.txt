cmake_minimum_required(VERSION 3.26)
project(imageResize)

set(CMAKE_CXX_STANDARD 17)

add_executable(imageResize main.cpp
        cs1300bmp.cpp
        cs1300bmp.h
)
