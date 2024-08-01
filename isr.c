#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"

extern unsigned long int p_count, b_count;
extern char main_f;
extern int flag;
extern char key;

void __interrupt() isr(void) {
    static unsigned short count;
    // Make d_count static to retain its value across interrupts

    if (TMR0IF) {
        // Add 8 to the current value of TMR0 to create a precise delay
        TMR0 = TMR0 + 8;

        if (main_f == 1 && flag==0) {
            // Increment p_count and reset the main flag and clear the display if the count reaches 100000
            if (p_count++ == 100000 && flag == 0) {
                p_count = 0;
                CLEAR_DISP_SCREEN;
                main_f = 0;
            }
        }
            // Decrement b_count every 20000 counts and reset count
            if (flag == 1 && count++ == 20000) {
                if (b_count > 0) {
                    b_count--;
                }
                // Display block duration
                count = 0;
            }
        



        // Clear the Timer 0 interrupt flag
        TMR0IF = 0;
    }
}
