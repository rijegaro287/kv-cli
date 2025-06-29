cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(kv_store C)

set(HEADERS
  include/constants.h
  include/kv_parser.h
  include/kv_controller.h
  include/utilities/string_conversion.h
  include/utilities/linked_list.h
)

set(SOURCES
  src/main.c
  src/kv_parser.c
  src/kv_controller.c
  src/utilities/string_conversion.c
  src/utilities/linked_list.c
)

add_executable(kv_store ${HEADERS} ${SOURCES})

target_include_directories(kv_store PRIVATE 
  include/
  include/utilities
)
