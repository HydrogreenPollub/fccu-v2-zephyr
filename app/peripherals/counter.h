#ifndef COUNTER_H
#define COUNTER_H
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/logging/log.h>

void counter_init(struct device *counter_dev);
void counter_set_alarm(struct device *counter_dev, uint16_t miliseconds);


#endif //COUNTER_H
