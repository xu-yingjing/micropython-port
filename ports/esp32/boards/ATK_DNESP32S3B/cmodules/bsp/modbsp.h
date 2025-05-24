#ifndef __MODBSP_H
#define __MODBSP_H

#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C
typedef struct _bsp_xl9555_pin_p_t {
    int (*init)(mp_obj_base_t *obj, mp_int_t io_mode, mp_int_t io_value);
    int (*set)(mp_obj_base_t *obj, mp_int_t io_value);
    int (*get)(mp_obj_base_t *obj, mp_int_t *io_value);
} bsp_xl9555_pin_p_t;
#endif

#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C
extern const mp_obj_type_t bsp_xl9555_type;
#endif

#endif
