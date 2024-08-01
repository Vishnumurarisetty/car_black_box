/*
 * File:   clear_log.c
 * Author: VISHNU
 *
 * Created on July 11, 2024, 6:19 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"

// External variables
extern char main_f;
extern int i;
extern char event[12][3];
extern unsigned int logcom;
extern char ov_flag;

// Function to clear the log
void clear_log(char key) {
    // Logic for clearing the log
    static int wait = 1000; // Initialize wait timer
    static char once; // Flag to check if the function has run once

    if (!once) {
        // First-time initialization
        i = 8; // Set event index to 8 for clear event
        ov_flag = 0; // Reset overflow flag
        logcom = 0; // Reset log counter
        store_event(); // Store the clear event
        clcd_print("   CLEAR LOG    ", LINE1(0)); // Print "CLEAR LOG" on the first line
        clcd_print(" SUCCESSFULL    ", LINE2(0)); // Print "SUCCESSFULL" on the second line
        once = 1; // Set the once flag to indicate initialization has been done
    }

    if (!wait--) {
        // Wait for a period before clearing the screen
        once = 0; // Reset the once flag
        wait = 1000; // Reset the wait timer
        CLEAR_DISP_SCREEN; // Clear the display screen
        main_f = MENU; // Set the main function to MENU
    }
}