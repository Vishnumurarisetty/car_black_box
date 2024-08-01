/*
 * File:   download_log.c
 * Author: VISHNU
 *
 * Created on July 11, 2024, 5:21 PM
 */

#include <xc.h>
#include "main.h"
#include "uart.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"

// External variables
extern char ov_flag, main_f; // Overflow flag and main function flag
extern int i; // Index for events
extern unsigned int logcom; // Log counter
extern char event[12][3]; // Event array
unsigned int prev_event; // Previous event index

// Function to download logs
void download_log() {
    char j = 0; // Loop counter
    char log[11], curr_log; // Log buffer and current log index
    static int wait = 1000; // Wait timer
    static char once; // Flag for one-time initialization
    
    if (!once) {
        prev_event = i; // Store previous event index
        i = 7; // Set event index to download log event
        store_event(); // Store the current event
        
        puts("#   TIME   EVENT SPEED\n\r"); // Print header for log

        // Loop through logs
        for (curr_log = 0; curr_log < (ov_flag ? 10 : logcom); curr_log++) {
            for (char i = 0; i < 10; i++) {
                log[i] = read_ext_eeprom(i + (curr_log * 10)); // Read log from EEPROM
            }  
            log[10] = '\0'; // Null-terminate the log string

            // Print log to UART
            for (char i = 0; i < 16; i++) {
                if (i == 0)
                    putch(curr_log + '0'); // Print log number
                else if (i == 1)
                    putch(' '); // Print space
                else if (i == 10)
                    puts("  "); // Print double space
                else if (i == 13)
                    puts("    "); // Print quadruple space
                else if (i == 4 || i == 7)
                    putch(':'); // Print colon separator
                else
                    putch(log[j++]); // Print log data
            }
            puts("\n\r"); // Print newline
            j = 0; // Reset loop counter
        }

        clcd_print("Download Log    ", LINE1(0)); // Print success message on CLCD
        clcd_print("SUCCESSFULL", LINE2(0)); // Print success message on CLCD
        once = 1; // Set one-time flag
    }
    
    if (!wait--) { // Wait timer countdown
        once = 0; // Reset one-time flag
        wait = 1000; // Reset wait timer
        CLEAR_DISP_SCREEN; // Clear CLCD screen
        main_f = MENU; // Set main function to menu
    }
}