cmake_minimum_required(VERSION 3.28)
project(PPass)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(PPass
        main.cpp
        xtea.h
        passdata.h
        fileworker.h)
