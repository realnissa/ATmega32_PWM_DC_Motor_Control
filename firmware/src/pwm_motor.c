#include "pwm_motor.h"

extern volatile uint8_t speed_level;

static const uint8_t ocr0_table[SPEED_LEVELS] =
{
    OCR0_LEVEL_0,
    OCR0_LEVEL_1,
    OCR0_LEVEL_2,
    OCR0_LEVEL_3,
    OCR0_LEVEL_4
};

void initialize_gpio(void)
{
    BTN_DDR  &= ~((1 << BTN_INC_PIN) | (1 << BTN_DEC_PIN) | (1 << BTN_STOP_PIN));
    BTN_PORT |=  ((1 << BTN_INC_PIN) | (1 << BTN_DEC_PIN) | (1 << BTN_STOP_PIN));

    LED_DDR  |=  ((1 << LED_LOW_PIN) | (1 << LED_MED_PIN) | (1 << LED_HIGH_PIN));
    LED_PORT &= ~((1 << LED_LOW_PIN) | (1 << LED_MED_PIN) | (1 << LED_HIGH_PIN));

    PWM_DDR  |=  (1 << PWM_PIN);
}

void initialize_pwm(void)
{
    TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) |
            (0 << CS02)  | (1 << CS01)  | (0 << CS00);

    OCR0 = OCR0_LEVEL_0;
}

void set_pwm_duty_cycle(uint8_t level)
{
    if (level > SPEED_MAX)
    {
        level = SPEED_MAX;
    }
    OCR0 = ocr0_table[level];
}

uint8_t debounce_button(uint8_t pin_mask)
{
    if (!(BTN_PIN & pin_mask))
    {
        _delay_ms(DEBOUNCE_DELAY_MS);
        if (!(BTN_PIN & pin_mask))
        {
            return 1;
        }
    }
    return 0;
}

void read_buttons(void)
{
    if (debounce_button(1 << BTN_STOP_PIN))
    {
        speed_level = SPEED_MIN;
        while (!(BTN_PIN & (1 << BTN_STOP_PIN)));
        _delay_ms(DEBOUNCE_DELAY_MS);
        return;
    }

    if (debounce_button(1 << BTN_INC_PIN))
    {
        if (speed_level < SPEED_MAX)
        {
            speed_level++;
        }
        while (!(BTN_PIN & (1 << BTN_INC_PIN)));
        _delay_ms(DEBOUNCE_DELAY_MS);
        return;
    }

    if (debounce_button(1 << BTN_DEC_PIN))
    {
        if (speed_level > SPEED_MIN)
        {
            speed_level--;
        }
        while (!(BTN_PIN & (1 << BTN_DEC_PIN)));
        _delay_ms(DEBOUNCE_DELAY_MS);
        return;
    }
}

void update_duty_cycle(void)
{
    set_pwm_duty_cycle(speed_level);
}

void update_speed_indicators(void)
{
    LED_PORT &= ~((1 << LED_LOW_PIN) | (1 << LED_MED_PIN) | (1 << LED_HIGH_PIN));

    switch (speed_level)
    {
        case 0:
            break;
        case 1:
            LED_PORT |= (1 << LED_LOW_PIN);
            break;
        case 2:
            LED_PORT |= (1 << LED_LOW_PIN) | (1 << LED_MED_PIN);
            break;
        case 3:
        case 4:
            LED_PORT |= (1 << LED_LOW_PIN) | (1 << LED_MED_PIN) | (1 << LED_HIGH_PIN);
            break;
        default:
            break;
    }
}
