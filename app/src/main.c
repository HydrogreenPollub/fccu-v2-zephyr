#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h> // Include for printk

#define SLEEP_TIME_MS 1000

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led_0), gpios);


int led_initialize() {
    if (!gpio_is_ready_dt(&led)) {
        printk("Error: GPIO device %s is not ready\n", led.port->name);
        return 1; // Indicate error
    } else {
        printk("GPIO device %s is ready\n", led.port->name);
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error %d: failed to configure LED pin %d\n", ret, led.pin);
        return 1; // Indicate error
    } else {
        printk("Successfully configured LED pin %d\n", led.pin);
    }

    return ret;
}

int led_on_tick() {
    int ret = gpio_pin_toggle_dt(&led);
    printk("[LED] Toggled LED. Return code: %d\n", ret);
    if (ret < 0) {
        printk("Error %d: failed to toggle LED pin %d\n", ret, led.pin);
    }
    return ret;
}



int main() {
    printk("Starting program on board: %s\n", CONFIG_BOARD);

    if (led_initialize()!= 0) {
        printk("LED init failed!\n");
        return -1;
    }

    printk("Entering blink loop...\n");
    while (1) {
        led_on_tick();

        k_msleep(1000);
    }
    return 0;
}
