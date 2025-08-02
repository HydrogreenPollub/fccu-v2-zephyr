#ifndef FCCU_V2_H
#define FCCU_V2_H


#include "can.h"
#include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/dsp/print_format.h>



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
    struct pwm_dt_spec fan_pwm;
    struct gpio_dt_spec fan_on_pin;
}fccu_fan_t;

typedef struct {
    struct gpio_dt_spec main_valve_on_pin;
    struct gpio_dt_spec purge_valve_on_pin;
}fccu_valve_pin_t;





typedef struct {
    struct gpio_dt_spec can_status_led;

}fccu_gpio_t;

typedef const struct device *fccu_can_t;
typedef const struct device *bmp280_sensor_t;


typedef struct {
    fccu_valve_pin_t valve_pin;
    fccu_fan_t fccu_fan;
    fccu_adc_t fccu_adc;
    fccu_can_t fccu_can;
    bmp280_sensor_t bmp280_sensor;

}fccu_device_t;

void fccu_init(fccu_device_t *fccu_device);
// void fccu_gpio_init(fccu_gpio_t *fccu_gpio);
void fccu_adc_init(fccu_adc_t *fccu_adc);
void fccu_can_init(fccu_can_t *can_device);
void fccu_fan_init(fccu_fan_t *fan);
void





void fccu_bmp280_sensor_init(bmp280_sensor_t *sensor);

void fccu_bmp280_sensor_read(bmp280_sensor_t *sensor);


void fccu_adc_read(fccu_adc_t *fccu_adc);
void fccu_on_tick(fccu_device_t* fccu_device);

#endif //FCCU_V2_H
