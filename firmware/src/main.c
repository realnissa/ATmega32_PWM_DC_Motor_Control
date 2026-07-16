#include "pwm_motor.h"

volatile uint8_t speed_level = SPEED_MIN;

int main(void)
{
    initialize_gpio();
    initialize_pwm();
    lcd_init();

    // Onyesha ujumbe wa kuanza
    lcd_set_cursor(0, 2);
    lcd_string("MOTOR CONTROL");
    lcd_set_cursor(1, 4);
    lcd_string("SYSTEM ACTIVE");
    _delay_ms(1500);
    lcd_command(0x01); // Safisha kioo

    speed_level = SPEED_MIN;
    update_duty_cycle();
    update_speed_indicators();
    update_lcd_display();

    sei();

    uint8_t last_speed_level = 99;

    while (1)
    {
        // 1. Soma Vitufe vya kuongeza/kupunguza kasi
        read_buttons(); 
        
        // 2. Sasisha Kasi ya Motor na LED
        update_duty_cycle();
        update_speed_indicators();
        
        // 3. Sasisha LCD iwapo kasi imebadilika
        if (speed_level != last_speed_level)
        {
            update_lcd_display();
            last_speed_level = speed_level;
        }

        _delay_ms(50); // Delay ndogo kwa ajili ya utulivu wa mfumo
    }

    return 0;
}