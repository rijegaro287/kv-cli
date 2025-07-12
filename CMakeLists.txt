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

FetchContent_Declare(unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        origin/master
)

FetchContent_MakeAvailable(logger)
FetchContent_MakeAvailable(kv_store)
FetchContent_MakeAvailable(unity)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_cli.h)
  
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/kv_cli.c)

add_executable(kv_cli ${HEADERS} ${SOURCES} src/main.c)

target_compile_definitions(kv_cli PUBLIC
  KV_CLI_MAX_OPEN_DATABASES=128
)

target_include_directories(kv_cli PRIVATE include)

target_link_libraries(kv_cli PRIVATE kv_store logger)


set(TEST_CLI ${CMAKE_CURRENT_SOURCE_DIR}/test/test_kv_cli.c
             ${CMAKE_CURRENT_SOURCE_DIR}/test/include/test_kv_cli.h)

add_executable(test_kv_cli ${TEST_CLI} ${HEADERS} ${SOURCES})

target_include_directories(test_kv_cli PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                               ${CMAKE_CURRENT_SOURCE_DIR}/test/include)

target_compile_definitions(unity PUBLIC
  UNITY_INCLUDE_DOUBLE
  UNITY_DOUBLE_PRECISION=1e-15
)
target_compile_definitions(test_kv_cli PUBLIC
  KV_CLI_MAX_OPEN_DATABASES=2
)

target_link_libraries(test_kv_cli PRIVATE kv_store logger unity)

enable_testing()

add_test(NAME test_kv_cli 
         COMMAND test_kv_cli
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) 

file(COPY "${CMAKE_SOURCE_DIR}/test/data/test_1.db" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/data/test_1.db")

file(COPY "${CMAKE_SOURCE_DIR}/test/data/test_2.db"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/data/test_2.db")

file(COPY "${CMAKE_SOURCE_DIR}/test/data/test_3.db"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/test/data/test_3.db")
