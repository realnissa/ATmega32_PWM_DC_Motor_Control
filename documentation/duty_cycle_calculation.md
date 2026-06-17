# Duty Cycle Calculation

## Formula

For 8-bit Fast PWM using Timer0, the duty cycle is set by the Output Compare Register (OCR0) relative to the maximum count of 255:

```
D ≈ (OCR0 / 255) x 100%
```

Where:
- D = Duty cycle (%)
- OCR0 = Output Compare Register 0 value (0–255)

## Duty Cycle Levels Used in This Project

| Level | OCR0 Value | Calculated Duty Cycle | Motor Condition |
|---|---|---|---|
| 0 | 0 | 0% | Stopped |
| 1 | 64 | 25.1% | Low speed |
| 2 | 128 | 50.2% | Medium speed |
| 3 | 191 | 74.9% | High speed |
| 4 | 255 | 100% | Full speed |

## Worked Examples

**Level 1 (Low speed):**
```
D = (64 / 255) x 100% = 25.1%
```

**Level 2 (Medium speed):**
```
D = (128 / 255) x 100% = 50.2%
```

**Level 3 (High speed):**
```
D = (191 / 255) x 100% = 74.9%
```

**Level 4 (Full speed):**
```
D = (255 / 255) x 100% = 100%
```

## Relationship Between Button Presses and OCR0

| Action | Effect on OCR0 | Result |
|---|---|---|
| Increase button pressed | OCR0 += 64 (capped at 255) | Duty cycle increases by ~25% per press |
| Decrease button pressed | OCR0 -= 64 (floored at 0) | Duty cycle decreases by ~25% per press |
| Stop button pressed | OCR0 = 0 | Motor stops immediately |

## Safety Limits Implemented in Firmware

To prevent invalid register values:

```c
if (OCR0 > 255) OCR0 = 255;   // upper bound
if (OCR0 < 0)   OCR0 = 0;     // lower bound (OCR0 is unsigned, so this is handled via comparison before subtraction)
```

These limits ensure the duty cycle can never exceed 100% or go negative, regardless of how many times the increase or decrease button is pressed in succession.

## PWM Period vs. ON-Time

At a fixed PWM frequency of 31.25 kHz (period ≈ 32 microseconds), the ON-time (T_ON) for each level is:

| Level | Duty Cycle | T_ON (approx.) | T_OFF (approx.) |
|---|---|---|---|
| 0 | 0% | 0 µs | 32 µs |
| 1 | 25% | 8 µs | 24 µs |
| 2 | 50% | 16 µs | 16 µs |
| 3 | 75% | 24 µs | 8 µs |
| 4 | 100% | 32 µs | 0 µs |

This confirms that the average voltage (and therefore average power) delivered to the motor scales linearly with the OCR0 value, which is the core principle behind PWM-based speed control.
