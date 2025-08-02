#include "pwm.h"

void pwm_init(struct pwm_dt_spec *pwm) {
    if (!pwm_is_ready_dt(pwm)) {
        printf("Error: PWM device %s is not ready\n", pwm->dev->name);
    }
}
