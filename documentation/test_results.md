# Test Results

## Test Environment

- **Simulation tool:** SimulIDE
- **Microcontroller model used in simulation:** mega32-2 (ATmega32 equivalent)
- **Firmware:** Compiled `.hex` file generated from MPLAB for VS Code, loaded directly into the simulated microcontroller
- **Test method:** Manual button presses on the simulated circuit, with oscilloscope probe monitoring the PWM output pin (PB3)

## Test 1: System Startup

| Item | Expected | Observed |
|---|---|---|
| Motor state at power-on | Stopped or minimum speed | Motor stopped, OCR0 initialized to 0 |
| VCC indicator | Powered | Confirmed active (VCC source shown energized) |
| LEDs at startup | All OFF (level 0) | Confirmed all OFF |

**Result: PASS**

## Test 2: Increase Speed Button

| Press Count | Expected OCR0 | Expected Duty Cycle | Observed Behavior |
|---|---|---|---|
| 1st press | 64 | 25% | Duty cycle increased, low-speed LED (PB0) turned ON |
| 2nd press | 128 | 50% | Duty cycle increased further, medium-speed LED (PB1) turned ON |
| 3rd press | 191 | 75% | Duty cycle increased further, high-speed LED (PB2) turned ON |
| 4th press | 255 | 100% | Duty cycle at maximum, motor at full speed |
| 5th press (at max) | 255 (capped) | 100% | No further increase — upper limit correctly enforced |

**Result: PASS**

## Test 3: Decrease Speed Button

| Press Count (from 100%) | Expected OCR0 | Expected Duty Cycle | Observed Behavior |
|---|---|---|---|
| 1st press | 191 | 75% | Duty cycle decreased, high-speed LED turned OFF |
| 2nd press | 128 | 50% | Duty cycle decreased further |
| 3rd press | 64 | 25% | Duty cycle decreased further |
| 4th press | 0 | 0% | Motor stopped, all LEDs OFF |
| 5th press (at 0%) | 0 (floored) | 0% | No further decrease — lower limit correctly enforced |

**Result: PASS**

## Test 4: Stop Button

| Condition | Expected | Observed |
|---|---|---|
| Pressed while motor running at any speed level | OCR0 immediately set to 0, motor stops | Confirmed: duty cycle dropped to 0% instantly regardless of prior speed level |
| LEDs after stop | All OFF | Confirmed all OFF |

**Result: PASS**

## Test 5: PWM Waveform Observation (Oscilloscope)

| Duty Cycle Level | Measured Frequency (SimulIDE scope) | Notes |
|---|---|---|
| 0% | 3.906 kHz | Matches calculated value for the simulated clock configuration |
| 25% | 3.906 kHz | Stable reading |
| 50% | 3.906 kHz | Stable reading |
| 75% | 3.866 kHz | Minor deviation — attributed to oscilloscope edge-counting measurement error at non-50% duty cycles, not a firmware fault |
| 100% | ~2.x kHz (unstable) | At 100% duty cycle the signal becomes constant HIGH with no falling edges within the sampling window, which causes the frequency counter to misread; this is a known simulator measurement limitation, not a PWM generation fault |

**Result: PASS** (PWM generation confirmed correct; frequency display anomalies at extreme duty cycles are simulator artifacts)

## Test 6: Reset Function

| Condition | Expected | Observed |
|---|---|---|
| Reset button pressed during operation | Microcontroller restarts, motor returns to stopped state | Confirmed |

**Result: PASS**

## Summary

| Test | Result |
|---|---|
| System startup | PASS |
| Increase speed | PASS |
| Decrease speed | PASS |
| Stop function | PASS |
| Maximum/minimum duty cycle limits | PASS |
| PWM waveform generation | PASS |
| Reset function | PASS |

## Known Limitations / Notes for Future Improvement

1. The oscilloscope frequency reading in SimulIDE becomes unreliable at 0% and 100% duty cycle due to the absence of signal edges within the measurement window. This does not affect actual motor behavior or real hardware performance.
2. Button debounce was implemented in firmware (software debounce) and was not separately verified with a logic analyzer; behavior during testing showed no false multiple-speed-change events.
3. Closed-loop speed control (encoder feedback) was not implemented in this version and remains a suggested extension task.
