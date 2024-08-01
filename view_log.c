/*
 * File:   view_log.c
 * Author: VISHNU
 *
 * Created on July 10, 2024, 5:57 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"

// Global variables
unsigned int view = 0; // View counter
extern unsigned int logcom; // Log counter
extern char ov_flag; // Overflow flag
extern char main_f; // Main function flag

// Function to view logs
void view_log(char key) {
    // Logic of view log
    static char v_key; // Variable to store key value
    static int vex_time; // Variable for exit timer
    v_key = read_switches(LEVEL_CHANGE); // Read the current switch state
    
    clcd_print("#   TIME   EV SP", LINE1(0)); // Print header on CLCD

    // Display log details on CLCD
    clcd_putch(view + '0', LINE2(0)); // Display log number
    clcd_putch(read_ext_eeprom(view * 10 + 0), LINE2(2)); // Display hour tens place
    clcd_putch(read_ext_eeprom(view * 10 + 1), LINE2(3)); // Display hour units place
    clcd_putch(':', LINE2(4)); // Display colon separator
    clcd_putch(read_ext_eeprom(view * 10 + 2), LINE2(5)); // Display minute tens place
    clcd_putch(read_ext_eeprom(view * 10 + 3), LINE2(6)); // Display minute units place
    clcd_putch(':', LINE2(7)); // Display colon separator
    clcd_putch(read_ext_eeprom(view * 10 + 4), LINE2(8)); // Display second tens place
    clcd_putch(read_ext_eeprom(view * 10 + 5), LINE2(9)); // Display second units place
    clcd_putch(read_ext_eeprom(view * 10 + 6), LINE2(11)); // Display event code first character
    clcd_putch(read_ext_eeprom(view * 10 + 7), LINE2(12)); // Display event code second character
    clcd_putch(read_ext_eeprom(view * 10 + 8), LINE2(14)); // Display speed tens place
    clcd_putch(read_ext_eeprom(view * 10 + 9), LINE2(15)); // Display speed units place

    // Handle next log view
    if (key == MK_SW4 && view < 9 && (view < (logcom - 1) || ov_flag == 1)) {
        view++;
    }
    // Handle previous log view
    else if (key == MK_SW5 && view > 0) {
        view--;
    }

    // Handle exit condition
    if (v_key == MK_SW5) {
        vex_time++;
    } else if (vex_time > 1 && vex_time < 1000) {
        vex_time = 0;
    }

    // Exit to menu after timeout
    if (vex_time == 1000) {
        vex_time = 0;
        main_f = MENU;
        CLEAR_DISP_SCREEN;
    }
}