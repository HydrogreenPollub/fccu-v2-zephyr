#include "fccu_v2.h"

#include "zephyr/drivers/adc.h"

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

void fccu_adc_init(fccu_adc_t *fccu_adc) {

    *fccu_adc = (fccu_adc_t){
        .low_pressure_sensor.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
        .fuel_cell_voltage.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
        .supercap_voltage.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 2),
        .temp_sensor.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 3),
    };
    adc_init(&fccu_adc->low_pressure_sensor.adc_channel, &fccu_adc->low_pressure_sensor.sequence, &fccu_adc->low_pressure_sensor.raw_value);
    adc_init(&fccu_adc->fuel_cell_voltage.adc_channel, &fccu_adc->fuel_cell_voltage.sequence, &fccu_adc->fuel_cell_voltage.raw_value);
    adc_init(&fccu_adc->supercap_voltage.adc_channel, &fccu_adc->supercap_voltage.sequence, &fccu_adc->supercap_voltage.raw_value);
    adc_init(&fccu_adc->temp_sensor.adc_channel, &fccu_adc->temp_sensor.sequence, &fccu_adc->temp_sensor.raw_value);
}

void fccu_adc_read(fccu_adc_t *fccu_adc) {
    adc_read_(&fccu_adc->low_pressure_sensor.adc_channel, &fccu_adc->low_pressure_sensor.sequence);
    adc_read_(&fccu_adc->fuel_cell_voltage.adc_channel, &fccu_adc->fuel_cell_voltage.sequence);
    adc_read_(&fccu_adc->supercap_voltage.adc_channel, &fccu_adc->supercap_voltage.sequence);
    adc_read_(&fccu_adc->temp_sensor.adc_channel, &fccu_adc->temp_sensor.sequence);
}

void fccu_can_init(fccu_can_device_t *can_device) {
    *can_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
}

void fccu_init(fccu_device_t* fccu_device) {
    fccu_gpio_init(&fccu_device->fccu_gpio);
    fccu_adc_init(&fccu_device->fccu_adc);
    fccu_can_init(&fccu_device->fccu_can_device);
}

void fccu_on_tick(fccu_device_t* fccu_device) {

}

