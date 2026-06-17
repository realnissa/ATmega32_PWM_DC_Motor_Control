#include "pwm_motor.h"

volatile uint8_t speed_level = SPEED_MIN;

int main(void)
{
    initialize_gpio();
    initialize_pwm();

    speed_level = SPEED_MIN;
    update_duty_cycle();
    update_speed_indicators();

    sei();

    while (1)
    {
        read_buttons();
        update_duty_cycle();
        update_speed_indicators();
    }

    return 0;
}
