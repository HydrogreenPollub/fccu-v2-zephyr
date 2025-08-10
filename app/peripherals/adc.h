#ifndef ADC_H
#define ADC_H

#include <zephyr/drivers/adc.h>
#include "zephyr/logging/log.h"
#include <inttypes.h>

int adc_init(const struct adc_dt_spec *adc, struct adc_sequence *sequence, int16_t *buffer);
int adc_read_(const struct adc_dt_spec *adc, struct adc_sequence *sequence);


#endif //ADC_H
