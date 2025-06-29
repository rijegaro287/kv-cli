cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(linked_list C)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(linked_list STATIC ${HEADERS} ${SOURCES})

target_include_directories(linked_list PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)
