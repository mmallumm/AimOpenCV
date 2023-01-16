cmake_minimum_required(VERSION 3.0.0)
project(lab3 CXX)

find_package(OpenCV REQUIRED)

set(TARGET_NAME Aim)
set(HEADERS Aim/Aim.hpp)
set(SOURCES Aim/Aim.cpp)
add_library(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCV_LIBS})

set(TARGET_NAME Utilities)
set(HEADERS task/cvDirectory.h)
set(SOURCES task/cvDirectory.cpp)
add_library(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET_NAME} PUBLIC ${OpenCV_LIBS})

set(TARGET_NAME lab3_main)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC Aim Utilities)