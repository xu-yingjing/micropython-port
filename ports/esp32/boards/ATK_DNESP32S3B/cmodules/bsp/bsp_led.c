#include "py/runtime.h"
#include "extmod/modmachine.h"
#include "extmod/virtpin.h"
#include "modbsp.h"

typedef struct _bsp_led_obj_t {
    mp_obj_base_t base;
    mp_obj_t pin;
    mp_int_t active;
} bsp_led_obj_t;

static mp_obj_t bsp_led_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    enum { ARG_pin, ARG_active };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pin, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_active, MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1} },
    };
    mp_arg_val_t vals[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, args, MP_ARRAY_SIZE(allowed_args), allowed_args, vals);

    bool is_valid_pin = false;
    if (mp_obj_is_type(vals[ARG_pin].u_obj, &machine_pin_type)) {
        is_valid_pin = true;
    }
#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C
    if (mp_obj_is_type(vals[ARG_pin].u_obj, &bsp_xl9555_pin_type)) {
        is_valid_pin = true;
    }
#endif
    if (!is_valid_pin) {
        mp_raise_ValueError(MP_ERROR_TEXT("Invalid Pin object"));
    }

    if ((vals[ARG_active].u_int != 0) && (vals[ARG_active].u_int != 1)) {
        mp_raise_ValueError(MP_ERROR_TEXT("Invalid active value"));
    }

    mp_pin_p_t *pin_p = (mp_pin_p_t *)MP_OBJ_TYPE_GET_SLOT(mp_obj_get_type(vals[ARG_pin].u_obj), protocol);
    mp_int_t ret = pin_p->ioctl(vals[ARG_pin].u_obj, MP_PIN_WRITE, (vals[ARG_active].u_int == 0) ? 1 : 0, NULL);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("Pin write failed"));
    }

    bsp_led_obj_t *self = mp_obj_malloc(bsp_led_obj_t, type);
    self->pin = vals[ARG_pin].u_obj;
    self->active = (vals[ARG_active].u_int == 0) ? 0 : 1;

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t led_on(mp_obj_t self_in) {
    bsp_led_obj_t *self = (bsp_led_obj_t *)MP_OBJ_TO_PTR(self_in);
    mp_pin_p_t *pin_p = (mp_pin_p_t *)MP_OBJ_TYPE_GET_SLOT(mp_obj_get_type(self->pin), protocol);

    mp_int_t ret = pin_p->ioctl(self->pin, MP_PIN_WRITE, (self->active == 0) ? 0 : 1, NULL);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("Pin write failed"));
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bsp_led_on, led_on);

static mp_obj_t led_off(mp_obj_t self_in) {
    bsp_led_obj_t *self = (bsp_led_obj_t *)MP_OBJ_TO_PTR(self_in);
    mp_pin_p_t *pin_p = (mp_pin_p_t *)MP_OBJ_TYPE_GET_SLOT(mp_obj_get_type(self->pin), protocol);

    mp_int_t ret = pin_p->ioctl(self->pin, MP_PIN_WRITE, (self->active == 0) ? 1 : 0, NULL);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("Pin write failed"));
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bsp_led_off, led_off);

static mp_obj_t led_toggle(mp_obj_t self_in) {
    bsp_led_obj_t *self = (bsp_led_obj_t *)MP_OBJ_TO_PTR(self_in);
    mp_pin_p_t *pin_p = (mp_pin_p_t *)MP_OBJ_TYPE_GET_SLOT(mp_obj_get_type(self->pin), protocol);

    mp_int_t io_level = pin_p->ioctl(self->pin, MP_PIN_READ, 0, NULL);
    if (io_level < 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("Pin read failed"));
    }

    mp_int_t ret = pin_p->ioctl(self->pin, MP_PIN_WRITE, (io_level == 0) ? 1 : 0, NULL);
    if (ret != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("Pin write failed"));
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(bsp_led_toggle, led_toggle);

static const mp_rom_map_elem_t bsp_led_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_on), MP_ROM_PTR(&bsp_led_on) },
    { MP_ROM_QSTR(MP_QSTR_off), MP_ROM_PTR(&bsp_led_off) },
    { MP_ROM_QSTR(MP_QSTR_toggle), MP_ROM_PTR(&bsp_led_toggle) },
};
static MP_DEFINE_CONST_DICT(bsp_led_locals_dict, bsp_led_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    bsp_led_type,
    MP_QSTR_LED,
    MP_TYPE_FLAG_NONE,
    make_new, bsp_led_make_new,
    locals_dict, &bsp_led_locals_dict
    );
