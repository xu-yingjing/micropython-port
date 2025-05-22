#include "drv_xl9555.h"
#include "drv_config.h"
#include "drv_bus.h"

typedef enum {
    XL9555_REG_INPUT_PORT_0 = 0x00,
    XL9555_REG_INPUT_PORT_1 = 0x01,
    XL9555_REG_OUTPUT_PORT_0 = 0x02,
    XL9555_REG_OUTPUT_PORT_1 = 0x03,
    XL9555_REG_POLARITY_INVERSION_PORT_0 = 0x04,
    XL9555_REG_POLARITY_INVERSION_PORT_1 = 0x05,
    XL9555_REG_CONFIGURATION_PORT_0 = 0x06,
    XL9555_REG_CONFIGURATION_PORT_1 = 0x07,
} xl9555_reg_t;

static xl9555_obj_t xl9555_default_obj = {0};

static int xl9555_read_reg(xl9555_obj_t *obj, xl9555_reg_t reg, uint8_t *data);
static int xl9555_write_reg(xl9555_obj_t *obj, xl9555_reg_t reg, uint8_t data);

int xl9555_init_obj(xl9555_obj_t *obj, i2c_port_t i2c_port, uint8_t i2c_address) {
    int ret;
    xl9555_io_num_t io_num;

    if (obj == NULL) {
        obj = &xl9555_default_obj;
    }

    if (obj->initialised != 0) {
        return 0;
    }

    obj->i2c_port = i2c_port;
    obj->i2c_address = i2c_address;

    ret = bus_i2c_master_init(obj->i2c_port, 400000);
    if (ret != 0) {
        return -1;
    }

    for (io_num = XL9555_IO_NUM_0_0; io_num < XL9555_IO_NUM_MAX; io_num++) {
        ret = xl9555_io_config_obj(obj, io_num, XL9555_IO_MODE_INPUT);
        if (ret != 0) {
            return -1;
        }
    }

    obj->initialised = !0;

    return 0;
}

int xl9555_io_config_obj(xl9555_obj_t *obj, xl9555_io_num_t io_num, xl9555_io_mode_t io_mode) {
    xl9555_reg_t reg;
    uint8_t config;
    int ret;

    if (obj == NULL) {
        obj = &xl9555_default_obj;
    }

    if (io_num <= XL9555_IO_NUM_0_7) {
        reg = XL9555_REG_CONFIGURATION_PORT_0;
    }
    else if (io_num <= XL9555_IO_NUM_1_7) {
        reg = XL9555_REG_CONFIGURATION_PORT_1;
        io_num -= 8;
    }
    else {
        return -1;
    }

    ret = xl9555_read_reg(obj, reg, &config);
    if (ret != 0) {
        return -1;
    }

    config &= ~((uint8_t)1 << io_num);
    config |= (uint8_t)io_mode << io_num;

    ret = xl9555_write_reg(obj, reg, config);
    if (ret != 0) {
        return -1;
    }

    return 0;
}

int xl9555_io_set_level_obj(xl9555_obj_t *obj, xl9555_io_num_t io_num, uint32_t io_level) {
    xl9555_reg_t reg;
    uint8_t config;
    int ret;

    if (obj == NULL) {
        obj = &xl9555_default_obj;
    }

    if (io_num <= XL9555_IO_NUM_0_7) {
        reg = XL9555_REG_OUTPUT_PORT_0;
    }
    else if (io_num <= XL9555_IO_NUM_1_7) {
        reg = XL9555_REG_OUTPUT_PORT_1;
        io_num -= 8;
    }
    else {
        return -1;
    }

    ret = xl9555_read_reg(obj, reg, &config);
    if (ret != 0) {
        return -1;
    }

    config &= ~((uint8_t)1 << io_num);
    config |= (uint8_t)((io_level == 0) ? (0) : (1)) << io_num;

    ret = xl9555_write_reg(obj, reg, config);
    if (ret != 0) {
        return -1;
    }

    return 0;
}

uint32_t xl9555_io_get_level_obj(xl9555_obj_t *obj, xl9555_io_num_t io_num) {
    xl9555_reg_t reg;
    uint8_t config;
    int ret;
    uint32_t io_level;

    if (obj == NULL) {
        obj = &xl9555_default_obj;
    }

    if (io_num <= XL9555_IO_NUM_0_7) {
        reg = XL9555_REG_INPUT_PORT_0;
    }
    else if (io_num <= XL9555_IO_NUM_1_7) {
        reg = XL9555_REG_INPUT_PORT_1;
        io_num -= 8;
    }
    else {
        return UINT32_MAX;
    }

    ret = xl9555_read_reg(obj, reg, &config);
    if (ret != 0) {
        return UINT32_MAX;
    }

    io_level = (((config >> io_num) & 0x01) == 0) ? (0) : (1);

    return io_level;
}

static int xl9555_read_reg(xl9555_obj_t *obj, xl9555_reg_t reg, uint8_t *data) {
    esp_err_t ret;

    ret = i2c_master_write_read_device(obj->i2c_port, obj->i2c_address, (uint8_t *)&reg, 1, data, 1, 100);
    if (ret != ESP_OK) {
        return -1;
    }

    return 0;
}

static int xl9555_write_reg(xl9555_obj_t *obj, xl9555_reg_t reg, uint8_t data) {
    esp_err_t ret;
    uint8_t buffer[] = {(uint8_t)reg, data};

    ret = i2c_master_write_to_device(obj->i2c_port, obj->i2c_address, buffer, 2, 100);
    if (ret != ESP_OK) {
        return -1;
    }

    return 0;
}
