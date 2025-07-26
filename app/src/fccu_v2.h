#ifndef FCCU_V2_H
#define FCCU_V2_H

#include <stdio.h>
#include "can.h"
#include "gpio.h"
#include "adc.h"
#include <zephyr/drivers/adc.h>

typedef struct {
    struct adc_dt_spec low_pressure_sensor;
    struct adc_dt_spec fuel_cell_voltage;
    struct adc_dt_spec supercap_voltage;
}fccu_adc_channel_t;




#endif //FCCU_V2_H
