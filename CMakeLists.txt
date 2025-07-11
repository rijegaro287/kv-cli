cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(kv_cli C)

add_subdirectory(lib/logger)
add_subdirectory(lib/kv_store)

set(HEADERS include/kv_cli.h
            include/constants.h)
  
set(SOURCES src/main.c
            src/kv_cli.c)

add_executable(kv_cli ${HEADERS} ${SOURCES})

target_include_directories(kv_cli PRIVATE include)

target_link_libraries(kv_cli PRIVATE kv_store)
target_link_libraries(kv_cli PRIVATE logger)

file(COPY "${CMAKE_SOURCE_DIR}/tests/data/test.db" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/tests/data/")
