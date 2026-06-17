# ATmega32 PWM DC Motor Control

A PWM-based DC motor speed control system built with the ATmega32 microcontroller. The system uses Timer0 (OC0/PB3) to generate a PWM signal that drives a DC motor through an N-channel MOSFET, with push-button speed control and LED speed-level indicators.

## Project Overview

This project demonstrates pulse-width modulation (PWM) as a method of controlling the average power delivered to a DC motor. Instead of simply switching the motor ON/OFF, the ATmega32 generates a variable-duty-cycle square wave that adjusts motor speed smoothly across five discrete levels (0%, 25%, 50%, 75%, 100%).

| Control | Function |
|---|---|
| Increase button (PA0) | Raises PWM duty cycle by one level |
| Decrease button (PA1) | Lowers PWM duty cycle by one level |
| Stop button (PA2) | Immediately sets duty cycle to 0% |
| LEDs (PB0–PB2) | Indicate current speed level |
| PWM output (PB3 / OC0) | Drives the motor via MOSFET switching |

## System Block Diagram

```
[Push Buttons] --> [ATmega32: Timer0 PWM] --> [Gate Resistor] --> [MOSFET] --> [DC Motor]
                            |                                          |
                       [LED Indicators]                          [Flyback Diode]
```

## Hardware Summary

| Component | Designator | Purpose |
|---|---|---|
| ATmega32-P | U1 | Main controller and PWM generator |
| DC Motor | M1 | Controlled actuator |
| N-Channel MOSFET | Q1 | Motor driver (low-side switch) |
| 1N4007 Diode | D4 | Flyback / back-EMF protection |
| Push buttons | SW1–SW4 | Increase, decrease, stop, reset |
| LEDs | D1–D3 | Speed-level indication |
| 8 MHz Crystal | Y1 | External clock source |
| 220 ohm resistor | R2 | MOSFET gate current limiting |
| 330 ohm resistors | R3–R5 | LED current limiting |
| 10 kohm resistor | R1 | Reset pull-up |

Full pin assignments are documented in [`documentation/pin_mapping_table.md`](documentation/pin_mapping_table.md).

## How PWM Speed Control Works

The ATmega32's Timer0 is configured in Fast PWM mode, outputting on PB3 (OC0). The duty cycle is controlled by the OCR0 register:

```
D ≈ (OCR0 / 255) x 100%
```

Pressing the increase or decrease button changes OCR0 in steps, moving the motor between five predefined speed levels. Full calculations are in [`documentation/duty_cycle_calculation.md`](documentation/duty_cycle_calculation.md), and the timer/register configuration is explained in [`documentation/pwm_configuration.md`](documentation/pwm_configuration.md).

## Repository Structure

```
ATmega32_PWM_DC_Motor_Control/
│
├── README.md
│
├── firmware/
│   ├── src/            # Firmware source code (.c)
│   ├── include/        # Header files (.h)
│   └── hex/            # Compiled .hex output
│
├── kicad/
│   ├── schematic/      # KiCad schematic files
│   ├── pcb/            # KiCad PCB layout files
│   └── gerber/         # Generated Gerber fabrication files
│
├── simulide/
│   ├── circuit/        # SimulIDE circuit file (.sim1)
│   └── screenshots/    # Simulation screenshots
│
├── documentation/
│   ├── block_diagram.png
│   ├── flowchart.png
│   ├── pwm_configuration.md
│   ├── duty_cycle_calculation.md
│   ├── pin_mapping_table.md
│   └── test_results.md
│
└── media/
    └── demonstration_video_link.txt
```

## Tools Used

| Tool | Purpose |
|---|---|
| KiCad | Schematic design and PCB layout |
| MPLAB for VS Code | Firmware development (C) |
| SimulIDE | Circuit simulation and PWM verification |
| Git / GitHub | Version control and submission |

## Firmware Behavior

```
main()
{
    initialize_gpio();
    initialize_pwm();
    while(1)
    {
        read_buttons();
        update_duty_cycle();
        update_speed_indicators();
    }
}
```

Source code is available in [`firmware/src/`](firmware/src/), and the compiled `.hex` file used for both hardware programming and SimulIDE simulation is in [`firmware/hex/`](firmware/hex/).

## Simulation

The circuit was simulated in SimulIDE before PCB fabrication to verify PWM generation, button response, LED indication, and motor behavior. An oscilloscope probe on PB3 confirmed the PWM waveform changed correctly across all five speed levels. Full results are documented in [`documentation/test_results.md`](documentation/test_results.md).

## Status

| Stage | Status |
|---|---|
| Schematic design (KiCad) | Complete |
| PCB layout (KiCad) | Complete |
| DRC / ERC checks | Complete |
| Firmware (PWM, buttons, LEDs) | Complete |
| SimulIDE simulation | Complete |
| Gerber file generation | Complete |
| Documentation | Complete |

## Extension (Optional)

A suggested next step for this project is closed-loop speed control using a rotary encoder, where the ATmega32 measures actual motor speed from encoder pulses and adjusts the PWM duty cycle automatically to maintain a target speed.

## Author

Student project for ATmega32 PWM-based DC Motor Speed Control assignment.
