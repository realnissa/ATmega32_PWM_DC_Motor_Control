# Pin Mapping Table

## ATmega32 Pin Assignments

| ATmega32 Pin | Port/Function | Connected Device | Direction | Notes |
|---|---|---|---|---|
| PB3 | OC0 (Timer0 PWM output) | 220 ohm resistor → MOSFET Gate | Output | Main PWM signal for motor speed control |
| PB0 | GPIO | Low-speed indicator LED (+ 330 ohm resistor) | Output | ON when duty cycle = 25% |
| PB1 | GPIO | Medium-speed indicator LED (+ 330 ohm resistor) | Output | ON when duty cycle = 50% |
| PB2 | GPIO | High-speed indicator LED (+ 330 ohm resistor) | Output | ON when duty cycle = 75% / 100% |
| PA0 | GPIO | Increase-speed push button | Input (pull-up) | BTN_INC |
| PA1 | GPIO | Decrease-speed push button | Input (pull-up) | BTN_DEC |
| PA2 | GPIO | Stop push button | Input (pull-up) | BTN_STOP |
| RESET | Reset | Push button + 10 kΩ pull-up resistor | Input | Manual MCU reset |
| XTAL1 | Clock | Crystal + 22 pF capacitor | — | External 8 MHz oscillator |
| XTAL2 | Clock | Crystal + 22 pF capacitor | — | External 8 MHz oscillator |
| VCC | Power | +5V supply | — | Logic power |
| AVCC | Power | +5V supply | — | Analog power reference |
| GND | Power | Common ground | — | Shared with motor driver ground |
| AREF | Reference | Tied to VCC | — | Not actively used (no ADC in this design) |

## Net Labels Used in Schematic

| Net Label | Connected To | Purpose |
|---|---|---|
| PWM_MOTOR | PB3 → R(gate) → MOSFET Gate | Carries the PWM control signal |
| BTN_INC | PA0 → Push button | Increase speed command |
| BTN_DEC | PA1 → Push button | Decrease speed command |
| BTN_STOP | PA2 → Push button | Stop command |
| MOTOR_SUPPLY | External connector (J1) → Motor (+) | Separate motor power input |
| LED_LOW | PB0 → LED (D1) | Low-speed indicator |
| LED_MED | PB1 → LED (D2) | Medium-speed indicator |
| LED_HIGH | PB2 → LED (D3) | High-speed indicator |
| GND | Common ground rail | Shared logic/motor ground reference |

## Motor Driver Stage Components

| Component | Designator | Value | Purpose |
|---|---|---|---|
| Gate resistor | R2 | 220 ohm | Limits gate charge/discharge current from PB3 |
| MOSFET | Q1 | N-Channel, Enhancement mode | Low-side switch driving the motor |
| Flyback diode | D4 | 1N4007 | Suppresses back-EMF when motor switches off |
| Motor connector | J1 | 2-pin | External motor power supply input |
| DC Motor | M1 | — | Final controlled actuator |

## LED Indicator Stage Components

| Component | Designator | Value | Purpose |
|---|---|---|---|
| LED 1 | D1 | Standard LED | Low-speed indicator |
| LED 2 | D2 | Standard LED | Medium-speed indicator |
| LED 3 | D3 | Standard LED | High-speed indicator |
| Resistor (D1) | R3 | 330 ohm | Current limiting |
| Resistor (D2) | R5 | 330 ohm | Current limiting |
| Resistor (D3) | R4 | 330 ohm | Current limiting |

## Button Input Stage Components

| Component | Designator | Connected Pin | Pull Method |
|---|---|---|---|
| SW2 | Increase button | PA0 | Internal pull-up (firmware-enabled) |
| SW3 | Decrease button | PA1 | Internal pull-up (firmware-enabled) |
| SW4 | Stop button | PA2 | Internal pull-up (firmware-enabled) |
| SW1 | Reset-adjacent button (if used separately) | — | External 10 kΩ pull-up |

## Clock Stage Components

| Component | Designator | Value | Purpose |
|---|---|---|---|
| Crystal | Y1 | 8 MHz | External clock source |
| Capacitor | C1 | 22 pF | XTAL1 load capacitor |
| Capacitor | C2 | 22 pF | XTAL2 load capacitor |

## Reset Stage Components

| Component | Designator | Value | Purpose |
|---|---|---|---|
| Pull-up resistor | R1 | 10 kΩ | Holds RESET high during normal operation |
| Push button | SW1 | — | Manual reset trigger |
