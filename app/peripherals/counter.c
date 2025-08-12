//
// Created by User on 11.08.2025.
//

#include "counter.h"

LOG_MODULE_REGISTER(counter);

void counter_init(struct device *counter_dev) {
    if (!device_is_ready(counter_dev)) {
        LOG_ERR("Counter device not ready");
        return;
    }
    counter_start(counter_dev);
    LOG_INF("Counter initialized");
}

void counter_set_alarm(struct device *counter_dev, uint8_t channel_id, counter_alarm_callback_t callback, uint32_t microseconds) {
    struct counter_alarm_cfg alarm_cfg = {
        .flags = 0,
        .ticks = counter_us_to_ticks(counter_dev, microseconds),
        .callback = callback,
        .user_data = NULL
    };

    int err = counter_set_channel_alarm(counter_dev, channel_id, &alarm_cfg);
    if (err < 0) {
        LOG_ERR("Failed to set counter alarm (%d)", err);
    } else {
        LOG_INF("Counter alarm set for 1s");
    }
}
