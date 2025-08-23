#include "fccu_v2.h"

LOG_MODULE_REGISTER(fccu);

fccu_flags_t flags;

 fccu_valve_pin_t valve_pin = {
    .main_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(main_valve_pin), gpios),
    .purge_valve_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(purge_valve_pin), gpios),
};

fccu_adc_t adc = {
    .low_pressure_sensor.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
    .fuel_cell_voltage.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
    .supercap_voltage.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 2),
    .temp_sensor.adc_channel = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 3),
};

 fccu_fan_t fan = {
    .fan_on_pin = GPIO_DT_SPEC_GET(DT_ALIAS(fan_pin), gpios),
    .fan_pwm = PWM_DT_SPEC_GET(DT_ALIAS(fan_pwm)),
};

 fccu_can_t can = {
    .can_device = DEVICE_DT_GET(DT_ALIAS(can)),
    .can_status_led = GPIO_DT_SPEC_GET(DT_ALIAS(can_led), gpios),
};

fccu_current_driver_t current_driver = {
    .driver_enable_pin = GPIO_DT_SPEC_GET(DT_ALIAS(driver_enable), gpios),
    .driver_pwm = PWM_DT_SPEC_GET(DT_ALIAS(driver_pwm)),
};

fccu_button_t button = {
    GPIO_DT_SPEC_GET(DT_ALIAS(button_start), gpios),
};

bmp280_sensor_t sensor = {
    DEVICE_DT_GET_ANY(bosch_bme280),
};

void fccu_valves_init() {
    gpio_init(&valve_pin.main_valve_on_pin, GPIO_OUTPUT_INACTIVE);
    gpio_init(&valve_pin.purge_valve_on_pin, GPIO_OUTPUT_INACTIVE);
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

static void fccu_current_driver_enable(fccu_current_driver_t *driver) {
    gpio_set(&driver->driver_enable_pin);
}

static void fccu_fan_pwm_set(fccu_fan_t *fan, uint8_t pwm_percent) {
    pwm_set_pulse_width_percent(&fan->fan_pwm, pwm_percent);
}

static void fccu_current_driver_set_pwm(fccu_current_driver_t *driver, uint8_t pwm_percent) {
    pwm_set_pulse_width_percent(&driver->driver_pwm, pwm_percent);
}

void fccu_adc_init() {
    adc_init(&adc.low_pressure_sensor.adc_channel, &adc.low_pressure_sensor.sequence, &adc.low_pressure_sensor.raw_value);
    adc_init(&adc.fuel_cell_voltage.adc_channel, &adc.fuel_cell_voltage.sequence, &adc.fuel_cell_voltage.raw_value);
    adc_init(&adc.supercap_voltage.adc_channel, &adc.supercap_voltage.sequence, &adc.supercap_voltage.raw_value);
    adc_init(&adc.temp_sensor.adc_channel, &adc.temp_sensor.sequence, &adc.temp_sensor.raw_value);
}

void fccu_fan_init() {
    gpio_init(&fan.fan_on_pin, GPIO_OUTPUT_INACTIVE);
    pwm_init(&fan.fan_pwm);
}

void fccu_can_init() {
    gpio_init(&can.can_status_led, GPIO_OUTPUT_INACTIVE);
    can_init(can.can_device, 500000);
}

void fccu_current_driver_init() {

    gpio_init(&current_driver.driver_enable_pin, GPIO_OUTPUT_INACTIVE);
    pwm_init(&current_driver.driver_pwm);
}

// void fccu_counter_init(fccu_counter_t *counter) {
//     counter -> counter_dev = DEVICE_DT_GET(DT_ALIAS(counter0));
//     counter_init(counter->counter_dev);
// }
//
//
// static void counter_alarm_callback(const struct device *dev,
//                                    uint8_t chan_id,
//                                    uint32_t ticks,
//                                    void *user_data)
// {
//     LOG_INF("Counter alarm triggered! chan=%d ticks=%u", chan_id, ticks);
// }
//
//
//
// void fccu_set_channel_isr(fccu_counter_t *counter, uint8_t channel_id, counter_alarm_callback_t callback, uint32_t microseconds) {
//     counter_set_alarm(counter->counter_dev, channel_id, callback, microseconds);
// }

static void cooldown_expired(struct k_work *work)
{
    ARG_UNUSED(work);

    // int val = gpio_pin_get_dt(&button);
    flags.start_button_pressed_flag = true;
    printf("Button pressed at %" PRIu32 "\n", k_cycle_get_32());

}
static K_WORK_DELAYABLE_DEFINE(cooldown_work, cooldown_expired);

void button_pressed(const struct device *dev, struct gpio_callback *cb,
            uint32_t pins)
{
    k_work_reschedule(&cooldown_work, K_MSEC(15));
}


void fccu_start_button_init() {
    gpio_init(&button.button, GPIO_INPUT);
    gpio_set_interrupt(&button.button, GPIO_INT_EDGE_TO_ACTIVE, &button.button_cb_data, button_pressed);
    flags.start_button_pressed_flag = false;
    printf("dupa_ok\n");
}

void fccu_bmp280_sensor_init() {

    if (sensor.sensor == NULL) {
        LOG_ERR("\nError: no device found.\n");
        return;
    }

    if (!device_is_ready(sensor.sensor)) {
        LOG_ERR("\nError: Device \"%s\" is not ready\n", sensor.sensor->name);
        return;
    }

    LOG_INF("Found device \"%s\", getting sensor data\n", sensor.sensor->name);
}

void fccu_bmp280_sensor_read() {
    int ret = sensor_sample_fetch(sensor.sensor);
    if (ret) {
        LOG_ERR("Sensor sample update error: %d\n", ret);
        return;
    }
    sensor_channel_get(sensor.sensor, SENSOR_CHAN_AMBIENT_TEMP, &sensor.temperature_buffer);
    sensor_channel_get(sensor.sensor, SENSOR_CHAN_PRESS, &sensor.pressure_buffer);
    sensor_channel_get(sensor.sensor, SENSOR_CHAN_HUMIDITY, &sensor.humidity_buffer);

    sensor.temperature = sensor_value_to_float(&sensor.temperature_buffer);
    sensor.pressure = sensor_value_to_float(&sensor.pressure_buffer) * 10.0f;
    sensor.humidity = sensor_value_to_float(&sensor.humidity_buffer);

    LOG_INF("Temp: %.2f C, Pressure: %.2f hPa, Humidity: %.2f RH\n", (double)sensor.temperature, (double)sensor.pressure, (double)sensor.humidity);
}

void fccu_init() {
    // fccu_adc_init(&fccu->adc);
    // fccu_can_init(&fccu->can);
    // fccu_valves_init(&fccu->valve_pins);
    // fccu_fan_init(&fccu->fan);
    fccu_start_button_init();
    // fccu_bmp280_sensor_init(&fccu->bmp280_sensor);
    // fccu_counter_init(&fccu->counter);
    // ads1015_init(&fccu->ads1015_device);
    // fccu_current_driver_init(&fccu->current_driver);
    // fccu_current_driver_enable(&fccu->current_driver);
    // k_msleep(10);
}

void fccu_adc_read() {
    LOG_INF("LP:\n");
    adc_read_(&adc.low_pressure_sensor.adc_channel, &adc.low_pressure_sensor.sequence);
    k_msleep(500);
    LOG_INF("FC_V:\n");
    adc_read_(&adc.fuel_cell_voltage.adc_channel, &adc.fuel_cell_voltage.sequence);
    k_msleep(500);
    LOG_INF("SC_V:\n");
    adc_read_(&adc.supercap_voltage.adc_channel, &adc.supercap_voltage.sequence);
    k_msleep(500);
    LOG_INF("Temp:\n");
    adc_read_(&adc.temp_sensor.adc_channel, &adc.temp_sensor.sequence);
    k_msleep(500);
}

void fccu_on_tick() {
    // fccu_bmp280_sensor_read(&fccu->bmp280_sensor);
    // fccu_adc_read(&fccu->adc);

    if (flags.start_button_pressed_flag == true /*|| (fccu->adc.low_pressure_sensor.voltage >= 0.5f)) */){
        printf("superekstra\n");
        k_msleep(1000);

        // fccu_main_valve_on(&fccu->valve_pins);
        // fccu_fan_on(&fccu->fan);
        // fccu_fan_pwm_set(&fccu->fan, 50);

    }
    int8_t current_driver_pwm = 10;


    // fccu_current_driver_set_pwm(&fccu->current_driver, current_driver_pwm);
    // // fccu_fan_pwm_set(&fccu->fan, current_driver_pwm);
    // fccu->ads1015_data.fuel_cell_current = ads1015_read_channel_single_shot(&fccu->ads1015_device, 0);
    // fccu->ads1015_data.fuel_cell_current = adc_map(fccu->ads1015_data.fuel_cell_current, 1.508f, 1.432, 0, 5); // Current sensor: 0-25A
    // LOG_INF("Current sensor = %.3f A, PWM = %d\r\n",(double)fccu->ads1015_data.fuel_cell_current, current_driver_pwm);
    // k_msleep(10);

    // if (fccu->ads1015_data.fuel_cell_current > FC_MAX_CURRENT) {
    //     current_driver_pwm -= 20;
    //     if (current_driver_pwm < 0)
    //         current_driver_pwm = 0;
    //
    //     fccu_current_driver_set_pwm(&fccu->current_driver, current_driver_pwm);
    //
    // }
    // else if (fccu->ads1015_data.fuel_cell_current < FC_MIN_CURRENT) {
    //     current_driver_pwm += 5;
    //     if (current_driver_pwm > 100)
    //         current_driver_pwm = 100;
    //
    //     fccu_current_driver_set_pwm(&fccu->current_driver, current_driver_pwm);
    //
    // }

}

