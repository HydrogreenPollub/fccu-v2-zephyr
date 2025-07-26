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

#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(app);



// int led_on_tick() {
//     int ret = gpio_pin_toggle_dt(&led);
//     // printf("[LED] Toggled LED. Return code: %d\n", ret);
//     if (ret < 0) {
//         printk("Error %d: failed to toggle LED pin %d\n", ret, led.pin);
//     }
//     return ret;
// }


int main() {
    // // printf("Starting program on board: %s\n", CONFIG_BOARD);
    //
    // if (led_initialize()!= 0) {
    //     printk("LED init failed!\n");
    //     return -1;
    // }

    // can_init(can_dev, 500000);



    while (1) {
        // led_on_tick();
        // can_send_(can_dev, 24, data, 2);

        k_msleep(200);
    }
    return 0;
}
