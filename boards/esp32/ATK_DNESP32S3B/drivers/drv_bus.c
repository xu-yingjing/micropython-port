#include "drv_bus.h"
#include "drv_config.h"

#ifndef CONFIG_BUS_I2C0_SDA_IO_NUM
#define CONFIG_BUS_I2C0_SDA_IO_NUM (GPIO_NUM_NC)
#endif

#ifndef CONFIG_BUS_I2C0_SCL_IO_NUM
#define CONFIG_BUS_I2C0_SCL_IO_NUM (GPIO_NUM_NC)
#endif

#if SOC_I2C_NUM >= 2
#ifndef CONFIG_BUS_I2C1_SDA_IO_NUM
#define CONFIG_BUS_I2C1_SDA_IO_NUM (GPIO_NUM_NC)
#endif

#ifndef CONFIG_BUS_I2C1_SCL_IO_NUM
#define CONFIG_BUS_I2C1_SCL_IO_NUM (GPIO_NUM_NC)
#endif
#endif

int bus_i2c_master_init(i2c_port_t port, uint32_t frequency) {
    esp_err_t ret;
    i2c_config_t config = {0};

    config.mode = I2C_MODE_MASTER;
    if (port == I2C_NUM_0) {
        config.sda_io_num = CONFIG_BUS_I2C0_SDA_IO_NUM;
        config.scl_io_num = CONFIG_BUS_I2C0_SCL_IO_NUM;
    }
#if SOC_I2C_NUM >= 2
    else if (port == I2C_NUM_1) {
        config.sda_io_num = CONFIG_BUS_I2C1_SDA_IO_NUM;
        config.scl_io_num = CONFIG_BUS_I2C1_SCL_IO_NUM;
    }
#endif
    else {
        return -1;
    }
    config.sda_pullup_en = GPIO_PULLUP_ENABLE,
    config.scl_pullup_en = GPIO_PULLUP_ENABLE,
    config.master.clk_speed = frequency,
    config.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
    ret = i2c_param_config(port, &config);
    if (ret != ESP_OK) {
        return -1;
    }

    ret = i2c_driver_install(port, config.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        return -1;
    }

    return 0;
}

int bus_i2c_master_deinit(i2c_port_t port) {
    esp_err_t ret;

    ret = i2c_driver_delete(port);
    if (ret != ESP_OK) {
        return -1;
    }

    return 0;
}
