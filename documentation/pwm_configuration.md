# PWM Configuration

## Overview

This project uses **Timer/Counter0 (Timer0)** of the ATmega32 in **Fast PWM mode** to generate a PWM signal on pin **PB3 (OC0)**. The PWM signal drives an N-channel MOSFET, which switches power to a DC motor, allowing speed control through duty cycle adjustment.

## Selected PWM Resource

| Parameter | Value |
|---|---|
| Timer used | Timer/Counter0 (Timer0) |
| PWM mode | Fast PWM (8-bit) |
| Output pin | PB3 (OC0) |
| Clock source | External crystal, 8 MHz |
| Prescaler | 1 (no prescaling) |
| PWM resolution | 8-bit (0–255) |

## Why Timer0 / OC0 / PB3

The project brief allows OC0, OC1A, OC1B, or OC2. PB3/OC0 was selected because:

- It is the simplest 8-bit timer to configure on the ATmega32.
- Fast PWM mode on Timer0 directly maps the OCR0 register to duty cycle, which matches the project's required duty-cycle levels (0%, 25%, 50%, 75%, 100%).
- Only one PWM channel is needed for this design, so the more complex 16-bit Timer1 (OC1A/OC1B) was not required.

## Register Configuration

### TCCR0 (Timer/Counter0 Control Register)

| Bit | Name | Value | Purpose |
|---|---|---|---|
| WGM01:WGM00 | Waveform Generation Mode | 1:1 | Selects Fast PWM mode |
| COM01:COM00 | Compare Output Mode | 1:0 | Clear OC0 on compare match, set at BOTTOM (non-inverting PWM) |
| CS02:CS00 | Clock Select | 0:0:1 | No prescaling (clk/1) |

Resulting configuration value:

```c
TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS00);
```

### DDRB (Data Direction Register B)

PB3 must be configured as an output for OC0 to physically drive the pin:

```c
DDRB |= (1 << PB3);
```

### OCR0 (Output Compare Register 0)

This register sets the duty cycle. It is updated whenever the user presses the increase or decrease button.

```c
OCR0 = duty_value; // 0–255
```

## PWM Frequency Calculation

For Fast PWM mode on an 8-bit timer:

```
f_PWM = f_clk / (N x 256)
```

Where:
- f_clk = 8,000,000 Hz (8 MHz crystal)
- N = prescaler = 1
- 256 = timer resolution (8-bit, counts 0–255)

```
f_PWM = 8,000,000 / (1 x 256) = 31,250 Hz (31.25 kHz)
```

This frequency is fixed and does not change with duty cycle — only the proportion of time the signal stays HIGH within each fixed period changes.

## Signal Path

```
ATmega32 (PB3/OC0) → 220 ohm gate resistor → MOSFET Gate
MOSFET Drain → DC Motor (-) terminal
MOSFET Source → GND
Flyback diode (1N4007) placed across motor terminals
```

The microcontroller never drives the motor directly — it only switches the MOSFET, which handles the actual motor current. This protects the ATmega32 pins from overcurrent and back-EMF.

## Note on Simulation (SimulIDE)

During SimulIDE testing, the oscilloscope reported a measured PWM frequency of approximately 3.9 kHz rather than the theoretical value, which depends on the clock frequency actually configured inside the simulated microcontroller component (this may differ from the real 8 MHz hardware target depending on simulator settings). Minor frequency fluctuations observed near 0% and 100% duty cycle in the simulator are measurement artifacts of the oscilloscope's edge-counting method, not errors in the PWM hardware logic itself.
