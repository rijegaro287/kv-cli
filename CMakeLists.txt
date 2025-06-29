cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(kv_store C)

set(HEADERS
  include/kv_parser.h
  include/constants.h
  include/utilities/string_conversion.h
)

set(SOURCES
  src/main.c
  src/parser/kv_parser.c
  src/utilities/string_conversion.c
)

add_executable(kv_store ${HEADERS} ${SOURCES})

target_include_directories(kv_store PRIVATE 
  include
  include/utilities
)
