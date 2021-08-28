# cmake_minimum_required(VERSION <specify CMake version here>)
project(SSSVT_Ulohy_Pole_04)

set(CMAKE_CXX_STANDARD 14)

include_directories(SSSVT_Ulohy_Pole_04)
include_directories(SSSVT_Ulohy_Pole_04/utilities)

add_executable(SSSVT_Ulohy_Pole_04
        SSSVT_Ulohy_Pole_04/utilities/utilities.cpp
        SSSVT_Ulohy_Pole_04/utilities/utilities.h
        SSSVT_Ulohy_Pole_04/json.hpp
        SSSVT_Ulohy_Pole_04/NameDay.cpp
        SSSVT_Ulohy_Pole_04/NameDay.h
        SSSVT_Ulohy_Pole_04/SSSVT_Ulohy_Pole_04.cpp)
