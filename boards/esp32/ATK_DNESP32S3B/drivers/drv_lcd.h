#ifndef __DRV_LCD_H
#define __DRV_LCD_H

#include "esp_lcd_panel_io.h"

typedef struct _lcd_obj_t {
    uint16_t width;
    uint16_t height;

    esp_lcd_i80_bus_handle_t i80_bus;
    esp_lcd_panel_io_handle_t panel_io;
    esp_lcd_panel_handle_t panel;
} lcd_obj_t;

int lcd_init_obj(lcd_obj_t *obj);
int lcd_backlight_ctrl_obj(lcd_obj_t *obj, uint8_t on);

static inline int lcd_init(void) {
    return lcd_init_obj(NULL);
}

static inline int lcd_backlight_ctrl(uint8_t on) {
    return lcd_backlight_ctrl_obj(NULL, on);
}

#endif
