#include "gpio.h"

LOG_MODULE_REGISTER(gpio);

int gpio_init(struct gpio_dt_spec *gpio, gpio_flags_t extra_flags) {

    if (!gpio_is_ready_dt(gpio)) {
        LOG_ERR("Error: GPIO device %s is not ready\n", gpio->port->name);
        return 1; // Indicate error
    } else {
        LOG_INF("GPIO device %s is ready\n", gpio->port->name);
    }

    int ret = gpio_pin_configure_dt(gpio, extra_flags);
    if (ret < 0) {
        LOG_ERR("Error %d: failed to configure pin %d\n", ret, gpio->pin);
        return 1; // Indicate error
    } else {
        LOG_INF("Successfully configured pin %d\n", gpio->pin);
    }

    return ret;
}

int gpio_reset(struct gpio_dt_spec *gpio) {
    return gpio_pin_set_dt(gpio, 0);
}

int gpio_set(struct gpio_dt_spec *gpio) {
    return gpio_pin_set_dt(gpio, 1);
}
