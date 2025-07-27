#ifndef FCCU_V2_H
#define FCCU_V2_H


#include "can.h"
#include "gpio.h"
#include "adc.h"
#include <zephyr/kernel.h>


typedef struct {
    struct adc_dt_spec low_pressure_sensor;
    struct adc_dt_spec fuel_cell_voltage;
    struct adc_dt_spec supercap_voltage;
    struct adc_dt_spec temp_sensor;
}fccu_adc_channel_t;


typedef struct {
    struct gpio_dt_spec can_status_led;
    struct gpio_dt_spec fan_on_pin;
    struct gpio_dt_spec main_valve_on_pin;
    struct gpio_dt_spec purge_valve_on_pin;
}fccu_gpio_t;

typedef const struct device *fccu_can_device_t;

typedef struct {
    fccu_gpio_t fccu_gpio;
    fccu_adc_channel_t fccu_adc_channel;
    fccu_can_device_t fccu_can_device;
}fccu_device_t;



void fccu_init(fccu_device_t *fccu_device);
void fccu_gpio_init(fccu_gpio_t *fccu_gpio);
void fccu_adc_init(fccu_adc_channel_t *fccu_channel);
void fccu_on_tick(fccu_device_t* fccu_device);

#endif //FCCU_V2_H
