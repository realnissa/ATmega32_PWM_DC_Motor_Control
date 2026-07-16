#include "pwm_motor.h"
#include <stdio.h>

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
    // Weka pini za vitufe kuwa Input na washa Pull-up resistors za ndani
    BTN_DDR  &= ~((1 << BTN_INC_PIN) | (1 << BTN_DEC_PIN) | (1 << BTN_STOP_PIN));
    BTN_PORT |=  ((1 << BTN_INC_PIN) | (1 << BTN_DEC_PIN) | (1 << BTN_STOP_PIN));

    // Weka LED kuwa Output
    LED_DDR  |=  ((1 << LED_LOW_PIN) | (1 << LED_MED_PIN) | (1 << LED_HIGH_PIN));
    LED_PORT &= ~((1 << LED_LOW_PIN) | (1 << LED_MED_PIN) | (1 << LED_HIGH_PIN));

    // Weka PWM Pin kuwa Output
    PWM_DDR  |=  (1 << PWM_PIN);
    
    // Weka pini za LCD (PC0 - PC5) kuwa Output
    LCD_DDR  |= 0x3F; 
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
    // Kitufe cha STOP (PA2)
    if (debounce_button(1 << BTN_STOP_PIN))
    {
        speed_level = SPEED_MIN;
        while (!(BTN_PIN & (1 << BTN_STOP_PIN))); // Subiri kitufe kiachiwe
        _delay_ms(DEBOUNCE_DELAY_MS);
        return;
    }

    // Kitufe cha kuongeza kasi (PA0)
    if (debounce_button(1 << BTN_INC_PIN))
    {
        if (speed_level < SPEED_MAX)
        {
            speed_level++;
        }
        while (!(BTN_PIN & (1 << BTN_INC_PIN))); // Subiri kitufe kiachiwe
        _delay_ms(DEBOUNCE_DELAY_MS);
        return;
    }

    // Kitufe cha kupunguza kasi (PA1)
    if (debounce_button(1 << BTN_DEC_PIN))
    {
        if (speed_level > SPEED_MIN)
        {
            speed_level--;
        }
        while (!(BTN_PIN & (1 << BTN_DEC_PIN))); // Subiri kitufe kiachiwe
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

// === KODI ZA LCD (4-Bit Mode) ===
void lcd_command(uint8_t cmd)
{
    LCD_PORT = (LCD_PORT & 0x03) | ((cmd & 0xF0) >> 2);
    LCD_PORT &= ~(1 << RS_PIN);
    LCD_PORT |= (1 << EN_PIN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN_PIN);
    _delay_us(200);

    LCD_PORT = (LCD_PORT & 0x03) | ((cmd & 0x0F) << 2);
    LCD_PORT |= (1 << EN_PIN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN_PIN);
    _delay_ms(2);
}

void lcd_char(uint8_t data)
{
    LCD_PORT = (LCD_PORT & 0x03) | ((data & 0xF0) >> 2);
    LCD_PORT |= (1 << RS_PIN);
    LCD_PORT |= (1 << EN_PIN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN_PIN);
    _delay_us(200);

    LCD_PORT = (LCD_PORT & 0x03) | ((data & 0x0F) << 2);
    LCD_PORT |= (1 << EN_PIN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN_PIN);
    _delay_ms(2);
}

void lcd_init(void)
{
    _delay_ms(20);
    lcd_command(0x02);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    _delay_ms(2);
}

void lcd_string(const char *str)
{
    while (*str)
    {
        lcd_char(*str++);
    }
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_command(address);
}

void update_lcd_display(void)
{
    char buffer1[17];
    char buffer2[17];
    uint8_t duty_percent = (speed_level * 100) / SPEED_MAX;

    lcd_set_cursor(0, 0);
    sprintf(buffer1, "Speed Level: %d  ", speed_level);
    lcd_string(buffer1);

    lcd_set_cursor(1, 0);
    sprintf(buffer2, "PWM Duty: %d%%   ", duty_percent);
    lcd_string(buffer2);
}