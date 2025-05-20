add_library(test INTERFACE)

target_sources(test INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/test.c
)

target_include_directories(test INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usermod INTERFACE test)
