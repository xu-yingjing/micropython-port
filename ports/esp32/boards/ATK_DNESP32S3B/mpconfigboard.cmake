set(IDF_TARGET esp32s3)

set(SDKCONFIG_DEFAULTS
    ${MICROPY_ORIGINAL_PORT_DIR}/boards/sdkconfig.base
    ${MICROPY_ORIGINAL_PORT_DIR}/boards/sdkconfig.usb
    ${MICROPY_ORIGINAL_PORT_DIR}/boards/sdkconfig.ble
    ${MICROPY_ORIGINAL_PORT_DIR}/boards/sdkconfig.240mhz
    ${MICROPY_ORIGINAL_PORT_DIR}/boards/sdkconfig.spiram_sx
    ${MICROPY_ORIGINAL_PORT_DIR}/boards/sdkconfig.spiram_oct
    ${MICROPY_BOARD_DIR}/sdkconfig.board
)
