# QDSP-6064 7-Segment Display Driver

Portable, multi-instance 7-segment display driver.

## Overview

This HAL-mediated custom 7-segment display driver permits ease of use that is portable across many platforms. It is designed specifically for the [QDSP-6064 Bubble Display](https://www.sparkfun.com/products/retired/12710), though it's compatibility may extend beyond this to generic 7-segment displays.

## Usage

The driver's header and source rely on an external user-defined hardware abstraction layer (HAL) called `hal.h` which defines the necessary calls in the `HAL` namespace. Namely, a GPIOPort class with `init()`, `portMode()` and `write()` methods.

The HAL GPIOPort object `init()` method should perform any necessary initialization required by the hardware used to implement each set of segment and selection pins. The `portMode()` method should set any such segment or selection pins to output when supplied with the enumeration value `GPIO_OUTPUT`. The `write()` method should map the value given (0 = not illuminated, not selected; 1 = illuminated, selected) onto each of these pins in the order corresponding to the segment and selection arrangement (see example below). Direct GPIO pins need not used, as the HAL layer may map these arbitrarily.

Because the display must be refreshed at regular intervals, a timer must be used to call the Micro7Seg's `refresh()` periodically (e.g. every 5ms).

### Example

```cpp
#include <micro7seg.h>

...
// Character segment and selection pin number mapping
const uint8_t DISPLAY_PINS_CHAR[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // {A, B, C, D, E, F, G, DG}
const uint8_t DISPLAY_PINS_SEL[4]  = {8, 9, 10, 11}; // {Left, ..., Right}

// Instantiate display
PeripheralIO::Micro7Seg display(DISPLAY_PINS_CHAR, DISPLAY_PINS_SEL);

...

int main()
{
    uint32_t counter = 0;

...
    // Init display
    display.init();
    someTimer.start();

...
    // Print to the display as desired
    display.write("DEAD");
    delay_s(1);
    display.write("BEEF");
    delay_s(1);

    // Count and display value in hex
    while (true)
    {
        display.write(counter++);
        delay_ms(100);
    }

...
}

// Should be triggered at regular intervals (e.g. 5ms)
someTimerISR()
{
    display.refresh();
}

...
```

## License

MIT © 2023 John Greenwell