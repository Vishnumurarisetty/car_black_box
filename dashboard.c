/*
 * File:   dashboard.c
 * Author: VISHNU
 *
 * Created on July 6, 2024, 1:29 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "adc.h"

// External variables
extern char time[9] = "00:00:00"; // Time string
extern char sp[4]; // Speed string
extern char events[][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "C ", "DL", "CL", "ST", "CP"}; // Events array
extern int i = 0; // Event index

// Function to display dashboard information
void dashboard() {
    // Display logic

    clcd_print("TIME", LINE1(0)); // Print "TIME" at the beginning of the first line
    clcd_print("EV", LINE1(10)); // Print "EV" at the 10th position of the first line
    clcd_print("SP", LINE1(14)); // Print "SP" at the 14th position of the first line
    clcd_print(time, LINE2(0)); // Print the time string at the beginning of the second line
    clcd_print(sp, LINE2(14)); // Print the speed string at the 14th position of the second line
    clcd_print(events[i], LINE2(10)); // Print the current event at the 10th position of the second line
}