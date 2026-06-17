#ifndef PWM_MOTOR_H
#define PWM_MOTOR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define F_CPU           8000000UL

#define PWM_DDR         DDRB
#define PWM_PORT        PORTB
#define PWM_PIN         PB3

#define BTN_DDR         DDRA
#define BTN_PORT        PORTA
#define BTN_PIN         PINA

#define BTN_INC_PIN     PA0
#define BTN_DEC_PIN     PA1
#define BTN_STOP_PIN    PA2

#define LED_DDR         DDRB
#define LED_PORT        PORTB

#define LED_LOW_PIN     PB0
#define LED_MED_PIN     PB1
#define LED_HIGH_PIN    PB2

#define SPEED_LEVELS        5
#define SPEED_MIN           0
#define SPEED_MAX           4

#define OCR0_LEVEL_0        0
#define OCR0_LEVEL_1        64
#define OCR0_LEVEL_2        128
#define OCR0_LEVEL_3        191
#define OCR0_LEVEL_4        255

#define DEBOUNCE_DELAY_MS   20

void initialize_gpio(void);
void initialize_pwm(void);
void set_pwm_duty_cycle(uint8_t level);
void read_buttons(void);
void update_duty_cycle(void);
void update_speed_indicators(void);
uint8_t debounce_button(uint8_t pin_mask);

#endif
