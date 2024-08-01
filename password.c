/*
 * File:   password.c
 * Author: VISHNU
 *
 * Created on July 8, 2024, 4:48 PM
 */

#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "ext_eeprom.h"

// External variables
extern char key;
extern char main_f;

// Local variables
int once = 0, count = 0;
unsigned long int p_count = 0, b_count = 180;
int flag = 0;

// Function to handle password input and verification
void password(char key) {
    static int input[4]; // Declare the input array as static to retain values between calls
    static unsigned int fail_count = 3, blink = 0; // Fail count and blink variables

    // Write the correct password to external EEPROM
    write_ext_eeprom(200, '0');
    write_ext_eeprom(201, '1');
    write_ext_eeprom(202, '0');
    write_ext_eeprom(203, '1');

    // Clear the display screen once
    if (once == 0) {
        CLEAR_DISP_SCREEN;
        once = 1;
    }

    // Handle password input
    if (count < 4) {
        clcd_print("Enter Password", LINE1(0)); // Print message on the first line of the CLCD
        if (blink++ == 1000) {
            clcd_putch(0x5F, LINE2(count)); // Blink cursor
        } else if (blink == 2000) {
            clcd_putch(0x20, LINE2(count)); // Clear cursor blink
            blink = 0;
        }
        if (key == MK_SW4) {
            input[count] = '0';
            clcd_putch(0x2A, LINE2(count)); // Print '*' for input
            count++;
            p_count = 0;
        } else if (key == MK_SW5) {
            input[count] = '1';
            clcd_putch(0x2A, LINE2(count)); // Print '*' for input
            count++;
            p_count = 0;
        }
    } else if (count >= 4) { // Check the input password
        int i;
        for (i = 0; i < 4; i++) {
            if (read_ext_eeprom(200 + (1 * i)) != input[i])
                break;
        }

        if (i == 4) { // Password is correct
            clcd_print("   SUCCESSFUL   ", LINE2(0));
            for (unsigned long int wait = 100000; wait--;);
            main_f = 2;
            fail_count = 3;
            count = 0;
            once = 0;
            CLEAR_DISP_SCREEN;
        } else if (fail_count > 1) { // Password is incorrect, but attempts remain
            fail_count--;
            clcd_print("Try Again       ", LINE1(0));
            clcd_putch('0' + fail_count, LINE2(0));
            clcd_print(" attempt left", LINE2(1));
            for (unsigned long int wait = 1000000; wait--;);
            count = 0;
            once = 0;
        } else { // Password is incorrect, and no attempts remain
            flag = 1;

            clcd_print("You are Blocked ", LINE1(0));
            clcd_print("  for ", LINE2(0));
            clcd_print(" sec", LINE2(9));
            clcd_putch((b_count / 100) + '0', LINE2(6)); // Hundreds place
            clcd_putch(((b_count / 10) % 10) + '0', LINE2(7)); // Tens place
            clcd_putch((b_count % 10) + '0', LINE2(8)); // Units place

            if (b_count == 0) {
                b_count = 180;
                fail_count = 3;
                count = 0;
                flag = 0;
                p_count = 0;
                CLEAR_DISP_SCREEN;
            }
        }
    }
}