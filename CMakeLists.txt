cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)

project(kv_cli C)

include(FetchContent)

FetchContent_Declare(logger
  GIT_REPOSITORY https://github.com/rijegaro287/logger.git
  GIT_TAG        origin/main
)

FetchContent_Declare(kv_store
GIT_REPOSITORY https://github.com/rijegaro287/kv-store.git
GIT_TAG        origin/main
)

FetchContent_MakeAvailable(logger)
FetchContent_MakeAvailable(kv_store)

set(HEADERS include/kv_cli.h)
  
set(SOURCES src/main.c
            src/kv_cli.c)

add_executable(kv_cli ${HEADERS} ${SOURCES})

target_include_directories(kv_cli PRIVATE include)

target_link_libraries(kv_cli PRIVATE kv_store)
target_link_libraries(kv_cli PRIVATE logger)

file(COPY "${CMAKE_SOURCE_DIR}/tests/data/test.db" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/tests/data/")
