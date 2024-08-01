/*
 * File:   main.c
 * Author: VISHNU
 *
 * Created on July 6, 2024, 1:28 PM
 */

#include <xc.h>
#include "main.h"
#include "adc.h"
#include "clcd.h"
#include "ext_eeprom.h"
#include "matrix_keypad.h"
#include "timer0.h"
#include "i2c.h"
#include "uart.h"
#include "ds1307.h"

// Flag variables
char main_f = 0, menu_f = 0, key;
// Array to store time
char time[9];
// Variable to store speed
float speed;
// Array to store speed as string
char sp[4];
// Array of event codes
char events[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "C ", "DL", "CL", "ST", "CP"};
// Index for event codes
int i = 0;
// Array to store clock registers
unsigned char clock_reg[3];

// Function to initialize configuration
void init_config() {
    init_clcd(); // Initialize LCD
    init_adc(); // Initialize ADC
    init_matrix_keypad(); // Initialize matrix keypad
    PEIE = 1; // Enable peripheral interrupts
    init_timer0(); // Initialize timer0
    GIE = 1; // Enable global interrupts
    init_i2c(); // Initialize I2C
    init_uart(); // Initialize UART
    init_ds1307(); // Initialize DS1307
}

// Main function
void main(void) {
    init_config(); // Call initialization function

    if(i == 0)
        store_event(); // Store initial event

    while (1) {
        //dashboard(); // Call dashboard function (commented out)
        get_time(); // Get current time
        speed = read_adc(CHANNEL4) / 10.33; // Read speed from ADC

        // Convert speed to string
        sp[0] = ((int) speed / 10) + '0';
        sp[1] = ((int) speed % 10) + '0';
        sp[2] = '\0';

        key = read_switches(STATE_CHANGE); // Read switches for state change

        // Handle key presses to change events
        if (i == 6 && (key == MK_SW2 || key == MK_SW3)) {
            i = 2;
            store_event(); // Store event
        } else if (key == MK_SW1) {
            i = 6;
            store_event(); // Store event
        } else if (key == MK_SW2 && i < 5) {
            i++;
            store_event(); // Store event
        } else if (key == MK_SW3 && i > 1) {
            i--;
            store_event(); // Store event
        }

        // Handle different main function states
        if (main_f == DASHBOARD) {
            dashboard(); // Call dashboard function
            if (key == MK_SW4) {
                main_f = 1;
                CLEAR_DISP_SCREEN; // Clear display screen
            }
        } else if (main_f == PASSWORD) {
            password(key); // Call password function
        } else if (main_f == MENU) {
            menu(key); // Call menu function
        } else if (main_f == MENU_ENTER) {
            if (menu_f == VIEWLOG) {
                view_log(key); // Call view log function
            } else if (menu_f == DOWNLOADLOG) {
                download_log(); // Call download log function
            } else if (menu_f == CLEARLOG) {
                clear_log(key); // Call clear log function
            } else if (menu_f == SETTIME) {
                settime(); // Call set time function
            } else if (menu_f == CHANGEPASS) {
                change_pass(key); // Call change password function
            }
        }
    }

    return;
}

// Function to get the current time from DS1307
void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // Read hours
    clock_reg[1] = read_ds1307(MIN_ADDR); // Read minutes
    clock_reg[2] = read_ds1307(SEC_ADDR); // Read seconds

    if (clock_reg[0] & 0x40) {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':'; // Add colon
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F); // Convert minutes to string
    time[4] = '0' + (clock_reg[1] & 0x0F); // Convert minutes to string
    time[5] = ':'; // Add colon
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F); // Convert seconds to string
    time[7] = '0' + (clock_reg[2] & 0x0F); // Convert seconds to string
    time[8] = '\0'; // Null terminate string
}