#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h> // Include for printk
#include "can.h"
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/uart.h>

#include "../../zephyr/include/zephyr/devicetree.h"
#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>
#include "fccu_v2.h"

#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(app);

fccu_device_t fccu;


//     int ret = gpio_pin_toggle_dt(&led);
//     // printf("[LED] Toggled LED. Return code: %d\n", ret);
//     if (ret < 0) {
//         printk("Error %d: failed to toggle LED pin %d\n", ret, led.pin);
//     }



int main() {
    // // printf("Starting program on board: %s\n", CONFIG_BOARD);

    // can_init(can_dev, 500000);

    fccu_init(&fccu);

    while (1) {
        fccu_on_tick(&fccu);
        // can_send_(can_dev, 24, data, 2);


    }
    return 0;
}
