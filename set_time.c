/*
 * File:   set_time.c
 * Author: VISHNU
 *
 * Created on July 16, 2024, 3:26 PM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "matrix_keypad.h"

// External variables
extern char time[9], main_f;
char pre_event, curr_event;

// Function to set the time
void settime() {
    static char hr, min, sec; // Static variables to hold hour, minute, and second
    static char once, field_flag = 2, s_key; // Static variables for flags and key input
    static unsigned int blink; // Static variable for blink timing
    static int set_timer, back_timer, wait = 500; // Static variables for timers and wait

    // Read key input
    s_key = read_switches(LEVEL_CHANGE);

    // Initialize the display and time variables once
    if (!once) {
        clcd_print("Time (HH:M:SS) ", LINE1(0)); // Print the prompt
        clcd_print(time, LINE2(0)); // Print the current time
        hr = ((time[0] - '0') * 10) + (time[1] - '0'); // Parse hours
        min = ((time[3] - '0') * 10) + (time[4] - '0'); // Parse minutes
        sec = ((time[6] - '0') * 10) + (time[7] - '0'); // Parse seconds
    }
    once = 1;

    // Blink the selected field
    if (blink++ == 250) {
        if (field_flag == 0) {
            clcd_print("  ", LINE2(0)); // Blink hours
        } else if (field_flag == 1) {
            clcd_print("  ", LINE2(3)); // Blink minutes
        } else if (field_flag == 2) {
            clcd_print("  ", LINE2(6)); // Blink seconds
        }
    } else if (blink == 500) {
        clcd_putch((hr / 10) + '0', LINE2(0)); // Display hours tens
        clcd_putch((hr % 10) + '0', LINE2(1)); // Display hours units
        clcd_putch(':', LINE2(2)); // Display colon
        clcd_putch((min / 10) + '0', LINE2(3)); // Display minutes tens
        clcd_putch((min % 10) + '0', LINE2(4)); // Display minutes units
        clcd_putch(':', LINE2(5)); // Display colon
        clcd_putch((sec / 10) + '0', LINE2(6)); // Display seconds tens
        clcd_putch((sec % 10) + '0', LINE2(7)); // Display seconds units
        blink = 0;
    }

    // Handle incrementing time fields
    if (s_key == MK_SW5) {
        back_timer++;
    } else if (back_timer > 1 && back_timer < 500) {
        if (field_flag == 0) {
            hr = (hr + 1) % 24; // Increment hours
        } else if (field_flag == 1) {
            min = (min + 1) % 60; // Increment minutes
        } else if (field_flag == 2) {
            sec = (sec + 1) % 60; // Increment seconds
        }
        back_timer = 0;
    }

    // Handle long press to exit
    if (back_timer >= 500) {
        back_timer = 0;
        once = 0;
        field_flag = 2;
        wait = 500;
        main_f = MENU;
        CLEAR_DISP_SCREEN; // Clear display screen
    }

    // Handle changing the selected field
    if (s_key == MK_SW4) {
        set_timer++;
    } else if (set_timer > 1 && set_timer < 400) {
        field_flag = (field_flag + 1) % 3; // Cycle through fields
        set_timer = 0;
    }

    // Handle setting the time
    if (set_timer >= 400) {
        clcd_print("Time Set        ", LINE1(0)); // Print success message
        clcd_print("Successfully!   ", LINE2(0)); // Print success message
        
        if (!wait--) {
            if (curr_event <= 6)
                pre_event = curr_event;
            curr_event = 9;
            store_event(); // Store the event
            
            write_ds1307(HOUR_ADDR, (((hr / 10) << 4) | (hr % 10))); // Write hours to RTC
            write_ds1307(MIN_ADDR, (((min / 10) << 4) | (min % 10))); // Write minutes to RTC
            write_ds1307(SEC_ADDR, (((sec / 10) << 4) | (sec % 10))); // Write seconds to RTC
            
            set_timer = 0;
            once = 0;
            field_flag = 2;
            wait = 500;
            main_f = MENU;
            CLEAR_DISP_SCREEN; // Clear display screen
        }
    }
}