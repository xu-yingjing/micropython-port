#ifndef __MODBSP_H
#define __MODBSP_H

#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C
extern const mp_obj_type_t bsp_xl9555_type;
extern const mp_obj_type_t bsp_xl9555_pin_type;
#endif
extern const mp_obj_type_t bsp_led_type;
extern const mp_obj_type_t bsp_buzzer_type;

#endif
