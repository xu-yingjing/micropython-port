add_library(bsp INTERFACE)

target_sources(bsp INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/modbsp.c
    ${CMAKE_CURRENT_LIST_DIR}/bsp_xl9555.c
    ${CMAKE_CURRENT_LIST_DIR}/bsp_led.c
    ${MICROPY_BOARD_DIR}/drivers/drv_xl9555.c
)

target_include_directories(bsp INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${MICROPY_BOARD_DIR}/drivers
)

target_link_libraries(usermod INTERFACE bsp)
