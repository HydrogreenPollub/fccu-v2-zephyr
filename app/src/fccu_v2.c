#include "fccu_v2.h"

LOG_MODULE_REGISTER(fccu);

fccu_flags_t flags;

void fccu_valves_init(fccu_valve_pin_t *valve_pin) {
    *valve_pin = (fccu_valve_pin_t){
        .main_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(main_valve_pin), gpios),
        .purge_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(purge_valve_pin), gpios),
    };
    gpio_init(&valve_pin->main_valve_on_pin, GPIO_OUTPUT_INACTIVE);
    gpio_init(&valve_pin->purge_valve_on_pin, GPIO_OUTPUT_INACTIVE);
}

static void fccu_main_valve_on(fccu_valve_pin_t *valve_pin) {
    gpio_set(&valve_pin->main_valve_on_pin);
}

static void fccu_purge_valve_on(fccu_valve_pin_t *valve_pin) {
    gpio_set(&valve_pin->purge_valve_on_pin);
}

static void fccu_fan_on(fccu_fan_t *fan) {
    gpio_set(&fan->fan_on_pin);
}

static void fccu_fan_pwm_set(fccu_fan_t *fan, uint8_t pwm_percent) {
    pwm_set_pulse_width_percent(&fan->fan_pwm, pwm_percent);
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

void button_pressed(const struct device *dev, struct gpio_callback *cb,
            uint32_t pins)
{
    flags.start_button_pressed_flag = true;
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}


void fccu_start_button_init(fccu_button_t *button) {
    *button = (fccu_button_t){
        GPIO_DT_SPEC_GET(DT_ALIAS(button_start), gpios),
    };
    gpio_init(&button->button, GPIO_INPUT);
    gpio_set_interrupt(&button->button, GPIO_INT_EDGE_TO_INACTIVE, button->button_cb_data, button_pressed);
    flags.start_button_pressed_flag = false;
}

void fccu_bmp280_sensor_init(bmp280_sensor_t *sensor) {
    sensor->sensor = DEVICE_DT_GET_ANY(bosch_bme280);

    if (sensor->sensor == NULL) {
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

    LOG_INF("Temp: %.2f C, Pressure: %.2f hPa, Humidity: %.2f RH\n", (double)sensor->temperature, (double)sensor->pressure, (double)sensor->humidity);
}

void fccu_init(fccu_device_t* fccu) {
    fccu_adc_init(&fccu->adc);
    // fccu_can_init(&fccu->can);
    // fccu_valves_init(&fccu->valve_pins);
    // fccu_fan_init(&fccu->fan);
    // fccu_start_button_init(&fccu->start_button);
    // fccu_bmp280_sensor_init(&fccu->bmp280_sensor);
    fccu_ads1015_adc_init(&fccu->ads1015_adc);
}

void fccu_adc_read(fccu_adc_t *adc) {
    LOG_INF("LP:\n");
    adc_read_(&adc->low_pressure_sensor.adc_channel, &adc->low_pressure_sensor.sequence);
    k_msleep(500);
    LOG_INF("FC_V:\n");
    adc_read_(&adc->fuel_cell_voltage.adc_channel, &adc->fuel_cell_voltage.sequence);
    k_msleep(500);
    LOG_INF("SC_V:\n");
    adc_read_(&adc->supercap_voltage.adc_channel, &adc->supercap_voltage.sequence);
    k_msleep(500);
    LOG_INF("Temp:\n");
    adc_read_(&adc->temp_sensor.adc_channel, &adc->temp_sensor.sequence);
    k_msleep(500);
}

void fccu_ads1015_adc_read(ads1015_adc_t *adc) {
    adc_read_(&adc->low_pressure_sensor.adc_channel, &adc->low_pressure_sensor.sequence);
    k_msleep(100);
    adc_read_(&adc->high_pressure_sensor.adc_channel, &adc->high_pressure_sensor.sequence);
    k_msleep(100);
    adc_read_(&adc->fuel_cell_current.adc_channel, &adc->fuel_cell_current.sequence);
    k_msleep(100);
}

void fccu_on_tick(fccu_device_t* fccu) {
    fccu_bmp280_sensor_read(&fccu->bmp280_sensor);
    fccu_adc_read(&fccu->adc);

    if ((flags.start_button_pressed_flag == true) || (fccu->adc.low_pressure_sensor.voltage >= 0.5f)) {
        fccu_main_valve_on(&fccu->valve_pins);
        fccu_fan_on(&fccu->fan);
        fccu_fan_pwm_set(&fccu->fan, 20);

    }
}

