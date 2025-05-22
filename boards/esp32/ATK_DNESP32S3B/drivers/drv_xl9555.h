#ifndef __DRV_XL9555_H
#define __DRV_XL9555_H

#include <stdint.h>
#include "driver/i2c.h"

typedef enum {
    XL9555_IO_MODE_OUTPUT = 0,
    XL9555_IO_MODE_INPUT = 1,
} xl9555_io_mode_t;

typedef enum {
    XL9555_IO_NUM_0_0 = 0,
    XL9555_IO_NUM_0_1 = 1,
    XL9555_IO_NUM_0_2 = 2,
    XL9555_IO_NUM_0_3 = 3,
    XL9555_IO_NUM_0_4 = 4,
    XL9555_IO_NUM_0_5 = 5,
    XL9555_IO_NUM_0_6 = 6,
    XL9555_IO_NUM_0_7 = 7,
    XL9555_IO_NUM_1_0 = 8,
    XL9555_IO_NUM_1_1 = 9,
    XL9555_IO_NUM_1_2 = 10,
    XL9555_IO_NUM_1_3 = 11,
    XL9555_IO_NUM_1_4 = 12,
    XL9555_IO_NUM_1_5 = 13,
    XL9555_IO_NUM_1_6 = 14,
    XL9555_IO_NUM_1_7 = 15,
    XL9555_IO_NUM_MAX,
} xl9555_io_num_t;

typedef struct _xl9555_obj_t {
    uint8_t initialised;

    i2c_port_t i2c_port;
    uint8_t i2c_address;
} xl9555_obj_t;

int xl9555_init_obj(xl9555_obj_t *obj, i2c_port_t i2c_port, uint8_t i2c_address);
int xl9555_io_config_obj(xl9555_obj_t *obj, xl9555_io_num_t io_num, xl9555_io_mode_t io_mode);
int xl9555_io_set_level_obj(xl9555_obj_t *obj, xl9555_io_num_t io_num, uint32_t io_level);
uint32_t xl9555_io_get_level_obj(xl9555_obj_t *obj, xl9555_io_num_t io_num);

static inline int xl9555_init(i2c_port_t i2c_port, uint8_t i2c_address) {
    return xl9555_init_obj(NULL, i2c_port, i2c_address);
}

static inline int xl9555_io_config(xl9555_io_num_t io_num, xl9555_io_mode_t io_mode) {
    return xl9555_io_config_obj(NULL, io_num, io_mode);
}

static inline int xl9555_io_set_level(xl9555_io_num_t io_num, uint32_t io_level) {
    return xl9555_io_set_level_obj(NULL, io_num, io_level);
}

static inline uint32_t xl9555_io_get_level(xl9555_io_num_t io_num) {
    return xl9555_io_get_level_obj(NULL, io_num);
}

#endif
