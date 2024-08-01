/*
 * File:   store_event.c
 * Author: VISHNU
 *
 * Created on July 6, 2024, 2:49 PM
 */

#include <xc.h>
#include "main.h"
#include "ext_eeprom.h"

// External variables
extern char time[9] = "00:00:00"; // Default time
extern char sp[4]; // Speed variable
extern char events[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "C ", "DL", "CL", "ST", "CP"}; // Event codes
// extern char store[11]; // Uncomment if needed
extern int i; // Index for the current event
unsigned int logcom = 0; // Log counter
char ov_flag = 0; // Overflow flag

// Function to store events in EEPROM
void store_event() {
    // Store time in EEPROM
    write_ext_eeprom(logcom * 10 + 0, time[0]); // Store hour tens place
    write_ext_eeprom(logcom * 10 + 1, time[1]); // Store hour units place
    write_ext_eeprom(logcom * 10 + 2, time[3]); // Store minute tens place
    write_ext_eeprom(logcom * 10 + 3, time[4]); // Store minute units place
    write_ext_eeprom(logcom * 10 + 4, time[6]); // Store second tens place
    write_ext_eeprom(logcom * 10 + 5, time[7]); // Store second units place

    // Store event code in EEPROM
    write_ext_eeprom(logcom * 10 + 6, events[i][0]); // Store event code first character
    write_ext_eeprom(logcom * 10 + 7, events[i][1]); // Store event code second character

    // Store speed in EEPROM
    write_ext_eeprom(logcom * 10 + 8, sp[0]); // Store speed tens place
    write_ext_eeprom(logcom * 10 + 9, sp[1]); // Store speed units place

    write_ext_eeprom(logcom * 10 + 10, '\0'); // Null terminator for the string

    logcom++; // Increment log counter

    // Handle log overflow
    if (logcom == 10) {
        logcom = 0; // Reset log counter
        ov_flag = 1; // Set overflow flag
    }
}