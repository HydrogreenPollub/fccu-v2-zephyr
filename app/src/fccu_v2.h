#ifndef FCCU_V2_H
#define FCCU_V2_H


#include "can.h"
#include "gpio.h"
#include "adc.h"
#include <zephyr/kernel.h>


// typedef struct {
//     struct adc_dt_spec low_pressure_sensor;
//     struct adc_dt_spec fuel_cell_voltage;
//     struct adc_dt_spec supercap_voltage;
//     struct adc_dt_spec temp_sensor;
// }fccu_adc_channel_t;
//
// typedef struct {
//     int16_t low_pressure_sensor_value;
//     int16_t fuel_cell_voltage_value;
//     int16_t supercap_voltage_value;
//     int16_t temp_sensor_value;
// }fccu_adc_measurement_t;

typedef struct {
    struct adc_dt_spec adc_channel;
    struct adc_sequence sequence;
    int16_t raw_value;
}fccu_adc_device_t;

typedef struct {
    fccu_adc_device_t low_pressure_sensor;
    fccu_adc_device_t fuel_cell_voltage;
    fccu_adc_device_t supercap_voltage;
    fccu_adc_device_t temp_sensor;
}fccu_adc_t;


typedef struct {
    struct gpio_dt_spec can_status_led;
    struct gpio_dt_spec fan_on_pin;
    struct gpio_dt_spec main_valve_on_pin;
    struct gpio_dt_spec purge_valve_on_pin;
}fccu_gpio_t;

typedef const struct device *fccu_can_device_t;

typedef struct {
    fccu_gpio_t fccu_gpio;
    fccu_adc_t fccu_adc;
    fccu_can_device_t fccu_can_device;
}fccu_device_t;

void fccu_init(fccu_device_t *fccu_device);
void fccu_gpio_init(fccu_gpio_t *fccu_gpio);
void fccu_adc_init(fccu_adc_t *fccu_adc);
void fccu_adc_read(fccu_adc_t *fccu_adc);
void fccu_on_tick(fccu_device_t* fccu_device);

#endif //FCCU_V2_H
