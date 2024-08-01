/*
 * File : change_password.c
 * Name  : VISHNU
 * 
 */

#include <xc.h>
#include "main.h"
#include "ext_eeprom.h"
#include "clcd.h"
#include "matrix_keypad.h"

extern char main_f;
extern int i;
extern char event[12][3];

void change_pass(char key) {
    static unsigned int blink, count;
    static int k;
    static char input1[4], input2[4];
    static char re_flag;
    static unsigned int wait1 = 100, wait2 = 100;

    if (count < 4) {
        if (!re_flag)
            clcd_print("Enter new passwd:", LINE1(0));
        else
            clcd_print("Re-enter passwd: ", LINE1(0));
    }

    // Blinking effect on LCD every 1000 cycles
    if (blink++ == 250) {
        clcd_putch(0x5F, LINE2(count)); // Display blinking character on LCD
    } else if (blink == 500) {
        clcd_putch(0x20, LINE2(count)); // Display space to stop blinking
        blink = 0; // Reset blink counter
    }


    // Handle keypad input
input:
    if (key != ALL_RELEASED && count < 4) {
        if (key == MK_SW4) {
            if (!re_flag)
                input1[k++] = '0'; // Record '0' in input array
            else
                input2[k++] = '0';
            clcd_putch(0x2A, LINE2(count)); // Display '*' on LCD
            count++; // Increment count of entered characters
        } else if (key == MK_SW5) {
            if (!re_flag)
                input1[k++] = '1'; // Record '1' in input array
            else
                input2[k++] = '1';
            clcd_putch(0x2A, LINE2(count)); // Display '*' on LCD
            count++; // Increment count of entered characters
        }
    } else if (count >= 4 && !re_flag) {
        re_flag = 1;
        count = 0;
        k = 0;
        CLEAR_DISP_SCREEN;
        goto input;
    } else if (count >= 4) {
        for (k = 0; k < 4; k++) {
            if (input2[k] != input1[k]) // If mismatch found
                break; // Exit loop
        }

        if (k == 4) {
            clcd_print("Password change", LINE1(0));
            clcd_print("SUCCESSFUL     ", LINE2(0)); // Display success message

            write_ext_eeprom(200, input1[0]);
            write_ext_eeprom(201, input1[1]);
            write_ext_eeprom(202, input1[2]);
            write_ext_eeprom(203, input1[3]);

            if (!wait1--) {
                main_f = MENU;
                re_flag = 0;
                count = 0;
                k = 0;
                //once=0;
                CLEAR_DISP_SCREEN;
                wait1 = 100;
            }

            i = 10;
            store_event();
        } else {
            clcd_print("Enter same      ", LINE1(0));
            clcd_print("password        ", LINE2(0)); // Display success message

            if (!wait2--) {
                main_f = MENU;
                re_flag = 0;
                count = 0;
                k = 0;
                //once=0;
                CLEAR_DISP_SCREEN;
                wait2 = 100;
            }
        }
    }
}