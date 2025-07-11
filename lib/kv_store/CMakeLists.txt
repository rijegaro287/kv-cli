set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/linked_list.c
            ${CMAKE_CURRENT_SOURCE_DIR}/hash_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/string_conversion.c
            ${CMAKE_CURRENT_SOURCE_DIR}/kv_parser.c
            ${CMAKE_CURRENT_SOURCE_DIR}/kv_controller.c
)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/constants.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_parser.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/kv_controller.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/string_conversion.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/linked_list.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/hash_table.h
)

add_library(kv_store STATIC ${SOURCES} ${HEADERS})

target_include_directories(kv_store PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(kv_store PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)

target_link_libraries(kv_store PRIVATE logger)
