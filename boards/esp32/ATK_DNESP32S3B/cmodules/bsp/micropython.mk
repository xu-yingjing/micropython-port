BSP_MOD_DIR := $(USERMOD_DIR)

SRC_USERMOD += $(BSP_MOD_DIR)/modbsp.c
SRC_USERMOD += $(BSP_MOD_DIR)/bsp_lcd.c
SRC_USERMOD += $(BSP_MOD_DIR)/../../drivers/drv_bus.c
SRC_USERMOD += $(BSP_MOD_DIR)/../../drivers/drv_xl9555.c
SRC_USERMOD += $(BSP_MOD_DIR)/../../drivers/drv_lcd.c

CFLAGS_USERMOD += -I$(BSP_MOD_DIR)
CFLAGS_USERMOD += -I$(BSP_MOD_DIR)/../../drivers
