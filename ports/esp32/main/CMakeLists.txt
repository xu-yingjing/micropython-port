# Set location of base MicroPython directory.
if(NOT MICROPY_DIR)
    get_filename_component(MICROPY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../components/micropython ABSOLUTE)
endif()

# Set location of the ESP32 port directory.
if(NOT MICROPY_PORT_DIR)
    get_filename_component(MICROPY_PORT_DIR ${MICROPY_DIR}/ports/esp32 ABSOLUTE)
endif()

if(NOT CUSTOM_PORT_DIR)
    get_filename_component(CUSTOM_PORT_DIR ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)
endif()

include(${CUSTOM_PORT_DIR}/esp32_common.cmake)
