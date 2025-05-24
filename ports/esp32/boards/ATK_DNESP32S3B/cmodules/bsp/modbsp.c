#include "py/runtime.h"
#include "modbsp.h"

static const mp_rom_map_elem_t mp_module_bsp_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_bsp) },
#if MICROPY_PY_MACHINE_I2C || MICROPY_PY_MACHINE_SOFTI2C
    { MP_ROM_QSTR(MP_QSTR_XL9555), MP_ROM_PTR(&bsp_xl9555_type) },
#endif
    { MP_ROM_QSTR(MP_QSTR_LED), MP_ROM_PTR(&bsp_led_type) },
};
static MP_DEFINE_CONST_DICT(mp_module_bsp_globals, mp_module_bsp_globals_table);

const mp_obj_module_t mp_module_bsp = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_bsp_globals,
};

MP_REGISTER_MODULE(MP_QSTR_bsp, mp_module_bsp);
