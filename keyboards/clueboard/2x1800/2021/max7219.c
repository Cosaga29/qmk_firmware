/*
 * Copyright (c) 2021 Zach White <skullydazed@gmail.com>
 * Copyright (c) 2007 Eberhard Fahle
 *
 *    max7219.c - A library for controling Leds with a MAX7219/MAX7221
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * This driver started as a port of Arduino's LedControl to QMK. The
 * original Arduino code can be found here:
 *
 * https://github.com/wayoda/LedControl
 *
 * Unlike LedControl we are using the native SPI support, you will need to
 * use the native SPI pins for your MCU. You can set the CS pin with
 * `#define MAX7219_LOAD <pin>`.
 */

#include "max7219.h"

// Datastructures
uint8_t max7219_spidata[MAX_BYTES];
uint8_t max7219_led_a[8][MAX7219_BUFFER_SIZE];

void shift_left(void *object, size_t size) {
   unsigned char *byte;
   for (byte = object; size--; ++byte) {
      unsigned char bit = 0;
      if (size) {
         bit = byte[1] & (1 << (CHAR_BIT - 1)) ? 1 : 0;
      }
      *byte <<= 1;
      *byte |= bit;
   }
}

/* Write max7219_spidata to all the max7219's
 */
void max7219_write_all(void) {
    dprintf("max7219_write_all()\n");
    if (spi_start(MAX7219_LOAD, false, 0, 8)) {
        for(int i = MAX_BYTES; i>0; i--) {
            dprintf("spi_write(%d)\n", max7219_spidata[i-1]);
            spi_write(max7219_spidata[i-1]);
        }
        spi_stop();
    } else {
        xprintf("Could not spi_start!\n");
    }
}

/* Write the current frame in max7219_led_a to all the max7219's
 */
void max7219_write_frame(void) {
    dprintf("max7219_write_frame()\n");

    // Set our opcode and data
    for (int col=0; col<8; col++) {
        for (int device_num=0; device_num<MAX7219_CONTROLLERS; device_num++) {
            int offset=device_num*2;
            max7219_spidata[offset] = max7219_led_a[col][device_num];
            max7219_spidata[offset+1] = col+1;
        }
        max7219_write_all();
    }
}

/* Write data to a single max7219
 */
void max7219_write(int device_num, volatile uint8_t opcode, volatile uint8_t data) {
    dprintf("max7219_write(%d, %d, %d)\n", device_num, opcode, data);

    // Clear the data array
    for(int i = MAX_BYTES; i>0; i--) {
        max7219_spidata[i-1]=0;
    }

    // Set our opcode and data
    uint8_t offset = device_num*2;
    max7219_spidata[offset] = data;
    max7219_spidata[offset+1] = opcode;

    // Write the data
    max7219_write_all();
}

/* Turn off all the LEDs
 */
void max7219_clear_display(void) {
    dprintf("max7219_clear_display();\n");

    for (int col=0; col<8; col++) {
        for (int device_num=0; device_num<MAX7219_CONTROLLERS; device_num++) {
            max7219_led_a[col][device_num] = 0;
        }
    }
    max7219_write_frame();
}

/* Enable the display test (IE turn on all 64 LEDs)
 */
void max7219_display_test(int device_num, bool enabled) {
    dprintf("max7219_display_test(%d, %d);\n", device_num, enabled);

    if (device_num<0 || device_num >= MAX7219_CONTROLLERS) {
        return;
    }

    max7219_write(device_num, OP_DISPLAYTEST, enabled);
}

/* Initialize the max7219 system and set the controller(s) to a default state.
 */
void max7219_init(void) {
    wait_ms(1500);
    dprintf("max7219_init()\n");

    setPinOutput(MAX7219_LOAD);
    writePinHigh(MAX7219_LOAD);
    spi_init();

    for (int i=0; i<MAX7219_CONTROLLERS; i++) {
        max7219_shutdown(i, true);
    }

    for (int i=0; i<MAX7219_CONTROLLERS; i++) {
        // Reset everything to defaults and enable the display
        max7219_display_test(i, false);
        max7219_set_scan_limit(i, 7);
        max7219_set_decode_mode(i, 0);
        max7219_set_intensity(i, LED_INTENSITY);
    }

    max7219_clear_display();

    for (int i=0; i<MAX7219_CONTROLLERS; i++) {
        // Test this display
        max7219_display_test(i, true);
        wait_ms(75);
        max7219_display_test(i, false);
    }

    for (int i=0; i<MAX7219_CONTROLLERS; i++) {
        max7219_shutdown(i, false);
    }
}

/* Set the decode mode of the controller. You probably don't want to change this.
 */
void max7219_set_decode_mode(int device_num, int mode) {
    dprintf("max7219_set_decode_mode(%d, %d);\n", device_num, mode);

    if (device_num<0 || device_num >= MAX7219_CONTROLLERS) {
        return;
    }

    max7219_write(device_num, OP_DECODEMODE, mode);
}

/* Set the intensity (brightness) for the LEDs.
 */
void max7219_set_intensity(int device_num, int intensity) {
    dprintf("max7219_set_intensity(%d, %d);\n", device_num, intensity);

    if (device_num<0 || device_num >= MAX7219_CONTROLLERS) {
        return;
    }

    if (intensity >= 0 && intensity<16) {
            max7219_write(device_num, OP_INTENSITY, intensity);
    }
}

/* Control a single LED.
 */
void max7219_set_led(int row, int column, bool state) {
    dprintf("max7219_set_led(%d, %d, %d);\n", row, column, state);

    if (column<0 || column>8*MAX7219_CONTROLLERS) {
        xprintf("max7219_set_led: column (%d) out of bounds\n", column);
        return;
    }

    if (row<0 || row>7) {
        xprintf("max7219_set_led: row (%d) out of bounds\n", row);
        return;
    }

    /* At this point we reverse the sense of row and column to match the
     * physical layout of my LEDs.
     */
    uint8_t device_num = column / 8;
    uint8_t col = column % 8;
    uint8_t val = 0b10000000 >> row;

    if (state) {
        max7219_led_a[col][device_num] = max7219_led_a[col][device_num]|val;
    } else {
        val = ~val;
        max7219_led_a[col][device_num] = max7219_led_a[col][device_num]&val;
    }
    max7219_write(device_num, col+1, max7219_led_a[col][device_num]);
}

/* Set the number of digits (rows) to be scanned.
 */
void max7219_set_scan_limit(int device_num, int limit) {
    dprintf("max7219_set_scan_limit(%d, %d);\n", device_num, limit);

    if (device_num<0 || device_num >= MAX7219_CONTROLLERS) {
        return;
    }

    if (limit >= 0 && limit < 8) {
        max7219_write(device_num, OP_SCANLIMIT, limit);
    }
}

/* Enable (true) or disable (false) the controller.
 */
void max7219_shutdown(int device_num, bool shutdown) {
    dprintf("max7219_shutdown(%d, %d);\n", device_num, shutdown);

    if (device_num<0 || device_num >= MAX7219_CONTROLLERS) {
        return;
    }

    max7219_write(device_num, OP_SHUTDOWN, !shutdown);
}