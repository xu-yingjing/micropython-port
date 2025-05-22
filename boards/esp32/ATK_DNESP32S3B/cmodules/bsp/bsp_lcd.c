#include "py/runtime.h"
#include "drv_lcd.h"

typedef struct _bsp_lcd_obj_t {
    mp_obj_base_t base;
} bsp_lcd_obj_t;

static mp_obj_t bsp_lcd_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    bsp_lcd_obj_t *self = mp_obj_malloc(bsp_lcd_obj_t, type);

    lcd_init();

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t bsp_lcd_dump(void) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(bsp_lcd_dump_obj, bsp_lcd_dump);

static const mp_rom_map_elem_t bsp_lcd_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_dump), MP_ROM_PTR(&bsp_lcd_dump_obj) },
};
static MP_DEFINE_CONST_DICT(bsp_lcd_locals_dict, bsp_lcd_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    bsp_lcd_type,
    MP_QSTR_lcd,
    MP_TYPE_FLAG_NONE,
    make_new, bsp_lcd_make_new,
    locals_dict, &bsp_lcd_locals_dict
    );
