#ifndef GPIO_H
#define GPIO_H

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

int gpio_init(struct gpio_dt_spec *gpio, gpio_flags_t extra_flags);
int gpio_reset(struct gpio_dt_spec *gpio);
int gpio_set(struct gpio_dt_spec *gpio);

#endif //GPIO_H
