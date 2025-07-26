#include "fccu_v2.h"



const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led_0), gpios);
const struct gpio_dt_spec fan_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(fan_pin), gpios);
const struct gpio_dt_spec main_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(main_valve_pin), gpios);
const struct gpio_dt_spec purge_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(purge_valve_pin), gpios);

const struct device *const can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

// static const struct device *adc = DEVICE_DT_GET(DT_ALIAS(adc_1));

const fccu_adc_channel_t  fccu_channel = {
    //.low_pressure_sensor = ADC_DT_SPEC_GET(DT_PATH(zephyr_user)),
    .low_pressure_sensor = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),

    };