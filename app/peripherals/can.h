#ifndef CAN_H
#define CAN_H

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "can_ids.h"

int can_send_(const struct device *can_dev, uint8_t id, uint8_t *data, uint8_t data_len);
int can_init(const struct device *can_dev, uint16_t baudrate);

#endif //CAN_H
