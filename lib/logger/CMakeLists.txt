set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/logger.c)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/logger.h
            ${CMAKE_CURRENT_SOURCE_DIR}/../constants.h)

add_library(logger STATIC ${SOURCES} ${HEADERS})

target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(logger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
