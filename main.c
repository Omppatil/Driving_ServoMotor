#include <reg52.h>
#include <intrins.h>

// === Pin Definitions ===
sbit SERVO = P2^0;
sbit START_BTN = P3^2;   // Active LOW

// === Delay Functions ===
void delay_us(unsigned int us) {
    while (us--) {
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1275; j++);
}

// === Servo PWM Pulse (one 20ms cycle) ===
void servoPulse(unsigned int high_time_us) {
    SERVO = 1;
    delay_us(high_time_us);
    SERVO = 0;
    delay_ms(18);  // To complete ~20ms
}

void main() {
    unsigned int angle_us = 1000;
    bit direction = 0;       // 0 = increase, 1 = decrease
    bit is_started = 0;      // Flag to check if button was pressed
    unsigned char i;

    P3 |= 0x04;  // Enable internal pull-up for P3.2

    while (1) {
        // Check for one-time button press
        if (START_BTN == 0 && !is_started) {
            delay_ms(20);  // Debounce
            if (START_BTN == 0) {
                is_started = 1;
            }
        }

        // Once started, sweep continuously
        if (is_started) {
            if (direction == 0) {
                angle_us += 100;
                if (angle_us >= 2000) {
                    angle_us = 2000;
                    direction = 1;
                }
            } else {
                angle_us -= 100;
                if (angle_us <= 1000) {
                    angle_us = 1000;
                    direction = 0;
                }
            }

            // Send 10 pulses at current angle
            for (i = 0; i < 10; i++) {
                servoPulse(angle_us);
            }
        }
    }
}