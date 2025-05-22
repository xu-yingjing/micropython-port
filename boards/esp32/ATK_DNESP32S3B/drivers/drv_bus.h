#ifndef __DRV_BUS_H
#define __DRV_BUS_H

#include <stdint.h>
#include "driver/i2c.h"

int bus_i2c_master_init(i2c_port_t port, uint32_t frequency);
int bus_i2c_master_deinit(i2c_port_t port);

#endif
