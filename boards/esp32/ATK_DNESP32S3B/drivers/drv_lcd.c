#include "drv_lcd.h"
#include "drv_config.h"
#include "drv_xl9555.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

static lcd_obj_t lcd_default_obj = {0};

int lcd_init_obj(lcd_obj_t *obj) {
    esp_lcd_i80_bus_config_t i80_bus_config = {0};
    esp_lcd_panel_io_i80_config_t panel_io_i80_config = {0};
    esp_lcd_panel_dev_config_t panel_dev_config = {0};
    esp_err_t ret;

    if (obj == NULL) {
        obj = &lcd_default_obj;
    }

    obj->width = CONFIG_LCD_PANEL_WIDTH;
    obj->height = CONFIG_LCD_PANEL_HEIGHT;

    xl9555_init(CONFIG_XL9555_I2C_PORT, CONFIG_XL9555_I2C_ADDRESS);
    xl9555_io_config(CONFIG_LCD_BL_IO_NUM, XL9555_IO_MODE_OUTPUT);
    lcd_backlight_ctrl(0);

    i80_bus_config.dc_gpio_num = CONFIG_LCD_DC_IO_NUM;
    i80_bus_config.wr_gpio_num = CONFIG_LCD_WR_IO_NUM;
    i80_bus_config.clk_src = LCD_CLK_SRC_DEFAULT;
    i80_bus_config.data_gpio_nums[0] = CONFIG_LCD_D0_IO_NUM;
    i80_bus_config.data_gpio_nums[1] = CONFIG_LCD_D1_IO_NUM;
    i80_bus_config.data_gpio_nums[2] = CONFIG_LCD_D2_IO_NUM;
    i80_bus_config.data_gpio_nums[3] = CONFIG_LCD_D3_IO_NUM;
    i80_bus_config.data_gpio_nums[4] = CONFIG_LCD_D4_IO_NUM;
    i80_bus_config.data_gpio_nums[5] = CONFIG_LCD_D5_IO_NUM;
    i80_bus_config.data_gpio_nums[6] = CONFIG_LCD_D6_IO_NUM;
    i80_bus_config.data_gpio_nums[7] = CONFIG_LCD_D7_IO_NUM;
    i80_bus_config.bus_width = 8;
    i80_bus_config.max_transfer_bytes = obj->width * obj->height * sizeof(uint16_t);
    i80_bus_config.psram_trans_align = 64;
    i80_bus_config.sram_trans_align = 4;
    ret = esp_lcd_new_i80_bus(&i80_bus_config, &obj->i80_bus);
    if (ret != ESP_OK) {
        return -1;
    }

    panel_io_i80_config.cs_gpio_num = CONFIG_LCD_CS_IO_NUM;
    panel_io_i80_config.pclk_hz = 10000000;
    panel_io_i80_config.trans_queue_depth = 10;
    panel_io_i80_config.on_color_trans_done = NULL;
    panel_io_i80_config.user_ctx = NULL;
    panel_io_i80_config.lcd_cmd_bits = 8;
    panel_io_i80_config.lcd_param_bits = 8;
    panel_io_i80_config.dc_levels.dc_idle_level = 0;
    panel_io_i80_config.dc_levels.dc_cmd_level = 0;
    panel_io_i80_config.dc_levels.dc_dummy_level = 0;
    panel_io_i80_config.dc_levels.dc_data_level = 1;
    panel_io_i80_config.flags.cs_active_high = 0;
    panel_io_i80_config.flags.reverse_color_bits = 0;
    panel_io_i80_config.flags.swap_color_bytes = 0;
    panel_io_i80_config.flags.pclk_active_neg = 0;
    panel_io_i80_config.flags.pclk_idle_low = 0;
    ret = esp_lcd_new_panel_io_i80(obj->i80_bus, &panel_io_i80_config, &obj->panel_io);
    if (ret != ESP_OK) {
        return -1;
    }

    panel_dev_config.reset_gpio_num = CONFIG_LCD_RST_IO_NUM;
    panel_dev_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    panel_dev_config.data_endian = LCD_RGB_DATA_ENDIAN_BIG;
    panel_dev_config.bits_per_pixel = 16;
    panel_dev_config.flags.reset_active_high = CONFIG_LCD_RST_ACTIVE_LEVEL;
    panel_dev_config.vendor_config = NULL;
    ret = esp_lcd_new_panel_st7789(obj->panel_io, &panel_dev_config, &obj->panel);
    if (ret != ESP_OK) {
        return -1;
    }

    esp_lcd_panel_reset(obj->panel);
    esp_lcd_panel_init(obj->panel);
    esp_lcd_panel_disp_on_off(obj->panel, true);

    lcd_backlight_ctrl_obj(obj, 1);

    return 0;
}

int lcd_backlight_ctrl_obj(lcd_obj_t *obj, uint8_t on) {
    int ret;

    if (obj == NULL) {
        obj = &lcd_default_obj;
    }
    
    if (on != 0) {
        ret = xl9555_io_set_level(CONFIG_LCD_BL_IO_NUM, CONFIG_LCD_BL_ACTIVE_LEVEL);
    }
    else {
        ret = xl9555_io_set_level(CONFIG_LCD_BL_IO_NUM, !CONFIG_LCD_BL_ACTIVE_LEVEL);
    }

    return ret;
}
