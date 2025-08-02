#include "fccu_v2.h"

LOG_MODULE_REGISTER(fccu);

void fccu_valves_init(fccu_valve_pin_t *valve_pin) {
    *valve_pin = (fccu_valve_pin_t){
        .main_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(main_valve_pin), gpios),
        .purge_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(purge_valve_pin), gpios),
    };
    gpio_init(&valve_pin->main_valve_on_pin, GPIO_OUTPUT_INACTIVE);
    gpio_init(&valve_pin->purge_valve_on_pin, GPIO_OUTPUT_INACTIVE);
}

void fccu_adc_init(fccu_adc_t *adc) {

    *adc = (fccu_adc_t){
        .low_pressure_sensor.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
        .fuel_cell_voltage.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
        .supercap_voltage.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 2),
        .temp_sensor.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 3),
    };
    adc_init(&adc->low_pressure_sensor.adc_channel, &adc->low_pressure_sensor.sequence, &adc->low_pressure_sensor.raw_value);
    adc_init(&adc->fuel_cell_voltage.adc_channel, &adc->fuel_cell_voltage.sequence, &adc->fuel_cell_voltage.raw_value);
    adc_init(&adc->supercap_voltage.adc_channel, &adc->supercap_voltage.sequence, &adc->supercap_voltage.raw_value);
    adc_init(&adc->temp_sensor.adc_channel, &adc->temp_sensor.sequence, &adc->temp_sensor.raw_value);
}


void fccu_fan_init(fccu_fan_t *fan) {
    *fan = (fccu_fan_t){
        .fan_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(fan_pin), gpios),
        .fan_pwm = PWM_DT_SPEC_GET(DT_ALIAS(fan_pwm)),
    };
    gpio_init(&fan->fan_on_pin, GPIO_OUTPUT_INACTIVE);
    pwm_init(&fan->fan_pwm);
}

void fccu_can_init(fccu_can_t *can) {
    *can = (fccu_can_t){
        .can_device = DEVICE_DT_GET(DT_ALIAS(can)),
        .can_status_led = GPIO_DT_SPEC_GET(DT_ALIAS(can_led), gpios),
    };
    gpio_init(&can->can_status_led, GPIO_OUTPUT_INACTIVE);
    can_init(can->can_device, 500000);
}

void fccu_start_button_init(fccu_button_t *button) {
    fccu_button_t button_temp = GPIO_DT_SPEC_GET(DT_ALIAS(button_start), gpios);
    *button = button_temp;
    gpio_init(button, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(button, GPIO_INT_EDGE_TO_INACTIVE);
}

void fccu_bmp280_sensor_init(bmp280_sensor_t *sensor) {
    sensor->sensor = DEVICE_DT_GET_ANY(bosch_bme280);

    if (sensor->sensor == NULL) {
        /* No such node, or the node does not have status "okay". */
        LOG_ERR("\nError: no device found.\n");
        return;
    }

    if (!device_is_ready(sensor->sensor)) {
        LOG_ERR("\nError: Device \"%s\" is not ready\n", sensor->sensor->name);
        return;
    }

    LOG_INF("Found device \"%s\", getting sensor data\n", sensor->sensor->name);
}

void fccu_bmp280_sensor_read(bmp280_sensor_t *sensor) {
    int ret = sensor_sample_fetch(sensor->sensor);
    if (ret) {
        LOG_ERR("Sensor sample update error: %d\n", ret);
        return;
    }
    sensor_channel_get(sensor->sensor, SENSOR_CHAN_AMBIENT_TEMP, &sensor->temperature_buffer);
    sensor_channel_get(sensor->sensor, SENSOR_CHAN_PRESS, &sensor->pressure_buffer);
    sensor_channel_get(sensor->sensor, SENSOR_CHAN_HUMIDITY, &sensor->humidity_buffer);

    sensor->temperature = sensor_value_to_float(&sensor->temperature_buffer);
    sensor->pressure = sensor_value_to_float(&sensor->pressure_buffer)/ 100.0f;
    sensor->humidity = sensor_value_to_float(&sensor->humidity_buffer);

    LOG_INF("Temp: %.2f C, Pressure: %.2f hPa, Humidity: %.2f RH\n", sensor->temperature, sensor->pressure, sensor->humidity);
}

void fccu_adc_read(fccu_adc_t *fccu_adc) {
    adc_read_(&fccu_adc->low_pressure_sensor.adc_channel, &fccu_adc->low_pressure_sensor.sequence);
    adc_read_(&fccu_adc->fuel_cell_voltage.adc_channel, &fccu_adc->fuel_cell_voltage.sequence);
    adc_read_(&fccu_adc->supercap_voltage.adc_channel, &fccu_adc->supercap_voltage.sequence);
    adc_read_(&fccu_adc->temp_sensor.adc_channel, &fccu_adc->temp_sensor.sequence);
}



void fccu_init(fccu_device_t* fccu_device) {
    fccu_adc_init(&fccu_device->adc);
    fccu_can_init(&fccu_device->can);
    fccu_valves_init(&fccu_device->valve_pins);
    fccu_fan_init(&fccu_device->fan);
    fccu_start_button_init(&fccu_device->start_button);

}

void fccu_on_tick(fccu_device_t* fccu_device) {

}

