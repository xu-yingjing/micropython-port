#ifndef __DRV_XL9555_H
#define __DRV_XL9555_H

#include "drv_config.h"

#define XL9555_DRV_TRANSFER_FLAG_READ   ((uint8_t)0x01)
#define XL9555_DRV_TRANSFER_FLAG_STOP   ((uint8_t)0x02)

typedef int (*xl9555_drv_transfer_t)(uint8_t *buf, size_t len, uint8_t flags, void *user_data);

typedef struct _xl9555_obj_t {
    xl9555_drv_transfer_t transfer;
    void *transfer_user_data;
} xl9555_drv_obj_t;

typedef enum {
    XL9555_DRV_IO_NUM_0_0 = 0,
    XL9555_DRV_IO_NUM_0_1 = 1,
    XL9555_DRV_IO_NUM_0_2 = 2,
    XL9555_DRV_IO_NUM_0_3 = 3,
    XL9555_DRV_IO_NUM_0_4 = 4,
    XL9555_DRV_IO_NUM_0_5 = 5,
    XL9555_DRV_IO_NUM_0_6 = 6,
    XL9555_DRV_IO_NUM_0_7 = 7,
    XL9555_DRV_IO_NUM_1_0 = 8,
    XL9555_DRV_IO_NUM_1_1 = 9,
    XL9555_DRV_IO_NUM_1_2 = 10,
    XL9555_DRV_IO_NUM_1_3 = 11,
    XL9555_DRV_IO_NUM_1_4 = 12,
    XL9555_DRV_IO_NUM_1_5 = 13,
    XL9555_DRV_IO_NUM_1_6 = 14,
    XL9555_DRV_IO_NUM_1_7 = 15,
    XL9555_DRV_IO_NUM_MAX,
} xl9555_drv_io_num_t;

typedef enum {
    XL9555_DRV_IO_MODE_OUTPUT = 0,
    XL9555_DRV_IO_MODE_INPUT = 1,
} xl9555_drv_io_mode_t;

typedef enum {
    XL9555_DRV_IO_LEVEL_LOW = 0,
    XL9555_DRV_IO_LEVEL_HIGH = 1,
} xl9555_drv_io_level_t;

int xl9555_drv_register_transfer(xl9555_drv_obj_t *obj, xl9555_drv_transfer_t transfer, void *user_data);
int xl9555_drv_io_config(xl9555_drv_obj_t *obj, xl9555_drv_io_num_t io_num, xl9555_drv_io_mode_t io_mode);
int xl9555_drv_io_set_level(xl9555_drv_obj_t *obj, xl9555_drv_io_num_t io_num, xl9555_drv_io_level_t io_level);
int xl9555_drv_io_get_level(xl9555_drv_obj_t *obj, xl9555_drv_io_num_t io_num, xl9555_drv_io_level_t *io_level);

#endif
