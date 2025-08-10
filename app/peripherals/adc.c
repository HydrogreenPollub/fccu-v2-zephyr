#include "adc.h"

LOG_MODULE_REGISTER(adc);

int adc_init(const struct adc_dt_spec *adc, struct adc_sequence *sequence, int16_t *buffer) {
    if (!device_is_ready(adc->dev)) {
        LOG_ERR("ADC device not ready\n");
        return -1;
    }

    if (adc_channel_setup_dt(adc) != 0) {
        LOG_ERR("ADC channel setup failed\n");
        return -1;
    }

    *sequence = (struct adc_sequence){
        .channels = BIT(adc->channel_id),
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .resolution = adc->resolution,
    };
    return 0;
}

int adc_read_(const struct adc_dt_spec *adc, struct adc_sequence *sequence) {
    int ret = adc_read_dt(adc, sequence);

    if (ret != 0) {
        LOG_ERR("ADC read failed (err %d)", ret);
    } else {
        int16_t *value = sequence->buffer;
        LOG_INF("ADC raw value: %d \n", value[0]);
    }
    return ret;
}
