#include "drv_xl9555.h"

typedef enum {
    XL9555_DRV_REG_INPUT_PORT_0 = 0x00,
    XL9555_DRV_REG_INPUT_PORT_1 = 0x01,
    XL9555_DRV_REG_OUTPUT_PORT_0 = 0x02,
    XL9555_DRV_REG_OUTPUT_PORT_1 = 0x03,
    XL9555_DRV_REG_POLARITY_INVERSION_PORT_0 = 0x04,
    XL9555_DRV_REG_POLARITY_INVERSION_PORT_1 = 0x05,
    XL9555_DRV_REG_CONFIGURATION_PORT_0 = 0x06,
    XL9555_DRV_REG_CONFIGURATION_PORT_1 = 0x07,
} xl9555_drv_reg_t;

static int xl9555_drv_read_reg(xl9555_drv_obj_t *obj, uint8_t reg, uint8_t *data);
static int xl9555_drv_write_reg(xl9555_drv_obj_t *obj, uint8_t reg, uint8_t data);

int xl9555_drv_register_transfer(xl9555_drv_obj_t *obj, xl9555_drv_transfer_t transfer, void *user_data) {
    if (obj == NULL) {
        return -1;
    }

    obj->transfer = transfer;
    obj->transfer_user_data = user_data;

    return 0;
}

int xl9555_drv_io_config(xl9555_drv_obj_t *obj, xl9555_drv_io_num_t io_num, xl9555_drv_io_mode_t io_mode) {
    xl9555_drv_reg_t reg;
    int ret;
    uint8_t config;

    if (obj == NULL) {
        return -1;
    }

    if (io_num <= XL9555_DRV_IO_NUM_0_7) {
        reg = XL9555_DRV_REG_CONFIGURATION_PORT_0;
    }
    else if (io_num <= XL9555_DRV_IO_NUM_1_7) {
        io_num -= 8;
        reg = XL9555_DRV_REG_CONFIGURATION_PORT_1;
    }
    else {
        return -1;
    }

    ret = xl9555_drv_read_reg(obj, reg, &config);
    if (ret != 0) {
        return ret;
    }

    config &= ~((uint8_t)1 << io_num);
    if (io_mode != XL9555_DRV_IO_MODE_OUTPUT) {
        config |= (uint8_t)1 << io_num;
    }

    ret = xl9555_drv_write_reg(obj, reg, config);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

int xl9555_drv_io_set_level(xl9555_drv_obj_t *obj, xl9555_drv_io_num_t io_num, xl9555_drv_io_level_t io_level) {
    xl9555_drv_reg_t reg;
    int ret;
    uint8_t config;

    if (obj == NULL) {
        return -1;
    }

    if (io_num <= XL9555_DRV_IO_NUM_0_7) {
        reg = XL9555_DRV_REG_OUTPUT_PORT_0;
    }
    else if (io_num <= XL9555_DRV_IO_NUM_1_7) {
        io_num -= 8;
        reg = XL9555_DRV_REG_OUTPUT_PORT_1;
    }
    else {
        return -1;
    }

    ret = xl9555_drv_read_reg(obj, reg, &config);
    if (ret != 0) {
        return ret;
    }

    config &= ~((uint8_t)1 << io_num);
    if (io_level != XL9555_DRV_IO_LEVEL_LOW) {
        config |= (uint8_t)1 << io_num;
    }

    ret = xl9555_drv_write_reg(obj, reg, config);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

int xl9555_drv_io_get_level(xl9555_drv_obj_t *obj, xl9555_drv_io_num_t io_num, xl9555_drv_io_level_t *io_level) {
    xl9555_drv_reg_t reg;
    int ret;
    uint8_t config;

    if ((obj == NULL) || (io_level == NULL)) {
        return -1;
    }

    if (io_num <= XL9555_DRV_IO_NUM_0_7) {
        reg = XL9555_DRV_REG_INPUT_PORT_0;
    }
    else if (io_num <= XL9555_DRV_IO_NUM_1_7) {
        io_num -= 8;
        reg = XL9555_DRV_REG_INPUT_PORT_1;
    }
    else {
        return -1;
    }

    ret = xl9555_drv_read_reg(obj, reg, &config);
    if (ret != 0) {
        return ret;
    }

    *io_level = (((config >> io_num) & (uint8_t)0x01) == 0) ? XL9555_DRV_IO_LEVEL_LOW : XL9555_DRV_IO_LEVEL_HIGH;

    return 0;
}

static int xl9555_drv_read_reg(xl9555_drv_obj_t *obj, uint8_t reg, uint8_t *data) {
    int ret;

    if ((obj == NULL) || (obj->transfer == NULL) || (data == NULL)) {
        return -1;
    }

    ret = obj->transfer(&reg, 1, 0, obj->transfer_user_data);
    if (ret != 1) {
        return ret;
    }

    ret = obj->transfer(data, 1, XL9555_DRV_TRANSFER_FLAG_READ | XL9555_DRV_TRANSFER_FLAG_STOP, obj->transfer_user_data);
    if (ret != 1) {
        return ret;
    }

    return 0;
}

static int xl9555_drv_write_reg(xl9555_drv_obj_t *obj, uint8_t reg, uint8_t data) {
    int ret;
    uint8_t buffer[2] = {reg, data};

    if ((obj == NULL) || (obj->transfer == NULL)) {
        return -1;
    }

    ret = obj->transfer(buffer, 2, XL9555_DRV_TRANSFER_FLAG_STOP, obj->transfer_user_data);
    if (ret != 2) {
        return ret;
    }

    return 0;
}
