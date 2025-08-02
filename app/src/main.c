#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h> // Include for printk
#include "can.h"
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>

#include "../../zephyr/include/zephyr/devicetree.h"
#include <zephyr/logging/log.h>

#include "fccu_v2.h"

LOG_MODULE_REGISTER(app);

// fccu_device_t fccu;

int main() {
    printf("Starting program on board: %s\n", CONFIG_BOARD);

    // fccu_init(&fccu);


    while (1) {
        //fccu_on_tick(&fccu);



    }
    return 0;
}
