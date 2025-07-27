#include "fccu_v2.h"

void fccu_gpio_init(fccu_gpio_t *fccu_gpio) {

    *fccu_gpio = (fccu_gpio_t){
        .can_status_led = GPIO_DT_SPEC_GET(DT_ALIAS(can_led), gpios),
        .fan_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(fan_pin), gpios),
        .main_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(main_valve_pin), gpios),
        .purge_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(purge_valve_pin), gpios),
    };
    gpio_init(&fccu_gpio->can_status_led, GPIO_OUTPUT_INACTIVE);
    gpio_init(&fccu_gpio->fan_on_pin, GPIO_OUTPUT_INACTIVE);
    gpio_init(&fccu_gpio->main_valve_on_pin, GPIO_OUTPUT_INACTIVE);
    gpio_init(&fccu_gpio->purge_valve_on_pin, GPIO_OUTPUT_INACTIVE);
}

void fccu_adc_init(fccu_adc_channel_t *fccu_channel) {

    *fccu_channel = (fccu_adc_channel_t){
        .low_pressure_sensor = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
        .fuel_cell_voltage = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
        .supercap_voltage = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 2),
        .temp_sensor = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 3),
    };
}

void fccu_can_init(fccu_can_device_t *can_device) {
    *can_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
}

void fccu_init(fccu_device_t* fccu_device) {
    fccu_gpio_init(&fccu_device->fccu_gpio);
    fccu_adc_init(&fccu_device->fccu_adc_channel);
    fccu_can_init(&fccu_device->fccu_can_device);
}

void fccu_on_tick(fccu_device_t* fccu_device) {

}

