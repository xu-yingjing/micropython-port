#include "py/runtime.h"
#include "extmod/modmachine.h"
#include "drv_xl9555.h"

#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C
typedef struct _bsp_xl9555_obj_t {
    mp_obj_base_t base;
    mp_obj_t i2c;
    uint16_t address;
    xl9555_drv_obj_t xl9555_drv_obj;
} bsp_xl9555_obj_t;

static int bsp_xl9555_drv_transfer(uint8_t *buf, size_t len, uint8_t flags_in, void *user_data) {
    bsp_xl9555_obj_t *self = (bsp_xl9555_obj_t *)user_data;
    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)MP_OBJ_TYPE_GET_SLOT(mp_obj_get_type(self->i2c), protocol);
    mp_machine_i2c_buf_t bufs = { .len = len, .buf = buf };
    unsigned int flags = ((flags_in & XL9555_DRV_TRANSFER_FLAG_READ) ? MP_MACHINE_I2C_FLAG_READ : 0) | ((flags_in & XL9555_DRV_TRANSFER_FLAG_STOP) ? MP_MACHINE_I2C_FLAG_STOP : 0);

    return i2c_p->transfer(self->i2c, self->address, 1, &bufs, flags);
}

static mp_obj_t bsp_xl9555_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    int ret;
    enum { ARG_i2c, ARG_address };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i2c, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_address, MP_ARG_INT, {.u_int = CONFIG_XL9555_I2C_ADDRESS} },
    };
    mp_arg_val_t vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, vals);

    bool is_valid_i2c = false;
#if MICROPY_PY_MACHINE_SOFTI2C
    if (mp_obj_is_type(vals[ARG_i2c].u_obj, &mp_machine_soft_i2c_type)) {
        is_valid_i2c = true;
    }
#endif
#if MICROPY_PY_MACHINE_I2C
    if (mp_obj_is_type(vals[ARG_i2c].u_obj, &machine_i2c_type)) {
        is_valid_i2c = true;
    }
#endif
    if (!is_valid_i2c) {
        mp_raise_ValueError(MP_ERROR_TEXT("Invalid I2C object"));
    }

    mp_machine_i2c_p_t *i2c_p = (mp_machine_i2c_p_t *)MP_OBJ_TYPE_GET_SLOT(mp_obj_get_type(vals[ARG_i2c].u_obj), protocol);
    mp_machine_i2c_buf_t bufs = { .len = 0, .buf = NULL };
    ret = i2c_p->transfer(vals[ARG_i2c].u_obj, vals[ARG_address].u_int, 1, &bufs, MP_MACHINE_I2C_FLAG_STOP);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("XL9555 not found at specified address"));
    }

    bsp_xl9555_obj_t *self = mp_obj_malloc(bsp_xl9555_obj_t, type);
    self->i2c = vals[ARG_i2c].u_obj;
    self->address = vals[ARG_address].u_int;

    ret = xl9555_drv_register_transfer(&self->xl9555_drv_obj, bsp_xl9555_drv_transfer, self);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("XL9555 driver register failed"));
    }

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t xl9555_io_config(size_t n_args, const mp_obj_t *args) {
    int ret;
    bsp_xl9555_obj_t *self = (bsp_xl9555_obj_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t io_num = mp_obj_get_int(args[1]);
    mp_int_t io_mode = mp_obj_get_int(args[2]);
    mp_int_t io_level = -1;
    if (n_args == 4) {
        io_level = mp_obj_get_int(args[3]);
    }

    ret = xl9555_drv_io_config(&self->xl9555_drv_obj, io_num, io_mode);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("XL9555 io config failed"));
    }

    if (io_level >= 0) {
        ret = xl9555_drv_io_set_level(&self->xl9555_drv_obj, io_num, (io_level == 0) ? XL9555_DRV_IO_LEVEL_LOW : XL9555_DRV_IO_LEVEL_HIGH);
        if (ret != 0) {
            mp_raise_ValueError(MP_ERROR_TEXT("XL9555 io set level failed"));
        }
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bsp_xl9555_io_config, 3, 4, xl9555_io_config);

static mp_obj_t xl9555_io_set_level(size_t n_args, const mp_obj_t *args) {
    int ret;
    bsp_xl9555_obj_t *self = (bsp_xl9555_obj_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t io_num = mp_obj_get_int(args[1]);
    mp_int_t io_level = mp_obj_get_int(args[2]);

    ret = xl9555_drv_io_set_level(&self->xl9555_drv_obj, io_num, (io_level == 0) ? XL9555_DRV_IO_LEVEL_LOW : XL9555_DRV_IO_LEVEL_HIGH);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("XL9555 io set level failed"));
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bsp_xl9555_io_set_level, 3, 3, xl9555_io_set_level);

static mp_obj_t xl9555_io_get_level(size_t n_args, const mp_obj_t *args) {
    int ret;
    bsp_xl9555_obj_t *self = (bsp_xl9555_obj_t *)MP_OBJ_TO_PTR(args[0]);
    mp_int_t io_num = mp_obj_get_int(args[1]);
    xl9555_drv_io_level_t io_level;

    ret = xl9555_drv_io_get_level(&self->xl9555_drv_obj, io_num, &io_level);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("XL9555 io get level failed"));
    }

    return MP_OBJ_NEW_SMALL_INT((io_level == XL9555_DRV_IO_LEVEL_LOW) ? 0 : 1);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bsp_xl9555_io_get_level, 2, 2, xl9555_io_get_level);

static const mp_rom_map_elem_t bsp_xl9555_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_io_config), MP_ROM_PTR(&bsp_xl9555_io_config) },
    { MP_ROM_QSTR(MP_QSTR_io_set_level), MP_ROM_PTR(&bsp_xl9555_io_set_level) },
    { MP_ROM_QSTR(MP_QSTR_io_get_level), MP_ROM_PTR(&bsp_xl9555_io_get_level) },
    { MP_ROM_QSTR(MP_QSTR_IO0_0), MP_ROM_INT(XL9555_DRV_IO_NUM_0_0) },
    { MP_ROM_QSTR(MP_QSTR_IO0_1), MP_ROM_INT(XL9555_DRV_IO_NUM_0_1) },
    { MP_ROM_QSTR(MP_QSTR_IO0_2), MP_ROM_INT(XL9555_DRV_IO_NUM_0_2) },
    { MP_ROM_QSTR(MP_QSTR_IO0_3), MP_ROM_INT(XL9555_DRV_IO_NUM_0_3) },
    { MP_ROM_QSTR(MP_QSTR_IO0_4), MP_ROM_INT(XL9555_DRV_IO_NUM_0_4) },
    { MP_ROM_QSTR(MP_QSTR_IO0_5), MP_ROM_INT(XL9555_DRV_IO_NUM_0_5) },
    { MP_ROM_QSTR(MP_QSTR_IO0_6), MP_ROM_INT(XL9555_DRV_IO_NUM_0_6) },
    { MP_ROM_QSTR(MP_QSTR_IO0_7), MP_ROM_INT(XL9555_DRV_IO_NUM_0_7) },
    { MP_ROM_QSTR(MP_QSTR_IO1_0), MP_ROM_INT(XL9555_DRV_IO_NUM_1_0) },
    { MP_ROM_QSTR(MP_QSTR_IO1_1), MP_ROM_INT(XL9555_DRV_IO_NUM_1_1) },
    { MP_ROM_QSTR(MP_QSTR_IO1_2), MP_ROM_INT(XL9555_DRV_IO_NUM_1_2) },
    { MP_ROM_QSTR(MP_QSTR_IO1_3), MP_ROM_INT(XL9555_DRV_IO_NUM_1_3) },
    { MP_ROM_QSTR(MP_QSTR_IO1_4), MP_ROM_INT(XL9555_DRV_IO_NUM_1_4) },
    { MP_ROM_QSTR(MP_QSTR_IO1_5), MP_ROM_INT(XL9555_DRV_IO_NUM_1_5) },
    { MP_ROM_QSTR(MP_QSTR_IO1_6), MP_ROM_INT(XL9555_DRV_IO_NUM_1_6) },
    { MP_ROM_QSTR(MP_QSTR_IO1_7), MP_ROM_INT(XL9555_DRV_IO_NUM_1_7) },
    { MP_ROM_QSTR(MP_QSTR_IO_MODE_OUTPUT), MP_ROM_INT(XL9555_DRV_IO_MODE_OUTPUT) },
    { MP_ROM_QSTR(MP_QSTR_IO_MODE_INPUT), MP_ROM_INT(XL9555_DRV_IO_MODE_INPUT) },
    { MP_ROM_QSTR(MP_QSTR_IO_LEVEL_LOW), MP_ROM_INT(XL9555_DRV_IO_LEVEL_LOW) },
    { MP_ROM_QSTR(MP_QSTR_IO_LEVEL_HIGH), MP_ROM_INT(XL9555_DRV_IO_LEVEL_HIGH) },
};
static MP_DEFINE_CONST_DICT(bsp_xl9555_locals_dict, bsp_xl9555_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    bsp_xl9555_type,
    MP_QSTR_xl9555,
    MP_TYPE_FLAG_NONE,
    make_new, bsp_xl9555_make_new,
    locals_dict, &bsp_xl9555_locals_dict
    );
#endif
