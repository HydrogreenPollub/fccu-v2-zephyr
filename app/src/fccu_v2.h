#ifndef FCCU_V2_H
#define FCCU_V2_H

#include "can.h"
#include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/logging/log.h>
#include "ads1015.h"

typedef struct {
    struct adc_dt_spec adc_channel;
    struct adc_sequence sequence;
    int16_t raw_value;
    float voltage;
}fccu_adc_device_t;

typedef struct {
    fccu_adc_device_t low_pressure_sensor;
    fccu_adc_device_t fuel_cell_voltage;
    fccu_adc_device_t supercap_voltage;
    fccu_adc_device_t temp_sensor;
}fccu_adc_t;

typedef struct {
    float low_pressure_sensor;
    float high_pressure_sensor;
    float fuel_cell_current;
}ads1015_adc_data_t;

typedef struct {
    struct pwm_dt_spec fan_pwm;
    struct gpio_dt_spec fan_on_pin;
}fccu_fan_t;

typedef struct {
    struct gpio_dt_spec main_valve_on_pin;
    struct gpio_dt_spec purge_valve_on_pin;
}fccu_valve_pin_t;

typedef struct {
    const struct device *can_device;
    struct gpio_dt_spec can_status_led;
}fccu_can_t;

typedef struct {
    const struct device *sensor;
    struct sensor_value temperature_buffer;
    struct sensor_value pressure_buffer;
    struct sensor_value humidity_buffer;
    float temperature;
    float pressure;
    float humidity;
}bmp280_sensor_t;

typedef struct {
    struct gpio_dt_spec button;
    struct gpio_callback button_cb_data;
}fccu_button_t;

typedef struct {
    struct device *counter_dev;
}fccu_counter_t;

typedef struct {
    uint8_t start_button_pressed_flag :1;
}fccu_flags_t;

typedef struct {
    fccu_valve_pin_t valve_pins;
    fccu_button_t start_button;
    fccu_fan_t fan;
    fccu_adc_t adc;
    fccu_can_t can;
    fccu_counter_t counter;
    bmp280_sensor_t bmp280_sensor;
    ads1015_adc_data_t ads1015_data;
    ads1015_type_t ads1015_device;
}fccu_device_t;

extern fccu_flags_t flags;

void fccu_init(fccu_device_t *fccu);
void fccu_adc_init(fccu_adc_t *adc);
void fccu_can_init(fccu_can_t *can);
void fccu_fan_init(fccu_fan_t *fan);
void fccu_valves_init(fccu_valve_pin_t *valve_pin);
void fccu_start_button_init(fccu_button_t *button);
void fccu_bmp280_sensor_init(bmp280_sensor_t *sensor);



void fccu_bmp280_sensor_read(bmp280_sensor_t *sensor);
void fccu_adc_read(fccu_adc_t *adc);

void fccu_on_tick(fccu_device_t* fccu);

#endif //FCCU_V2_H
