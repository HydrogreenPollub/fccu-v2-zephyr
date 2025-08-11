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
    LOG_INF("Counter initialized");
}
