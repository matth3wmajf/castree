cmake_minimum_required(VERSION 3.10)
project(Beta C)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/source/*.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(beta ${SOURCES})