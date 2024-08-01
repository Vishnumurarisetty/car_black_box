#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
    CLCD_RS = control_bit; // Set the RS (Register Select) control bit
    CLCD_PORT = byte;      // Load the byte onto the CLCD data port

    /* Should be at least 200ns */
    CLCD_EN = HI; // Enable the CLCD
    CLCD_EN = LO; // Disable the CLCD

    PORT_DIR = INPUT;     // Set the port direction to input
    CLCD_RW = HI;         // Set the Read/Write control bit to Read
    CLCD_RS = INSTRUCTION_COMMAND; // Set the RS to instruction command

    do
    {
        CLCD_EN = HI; // Enable the CLCD
        CLCD_EN = LO; // Disable the CLCD
    } while (CLCD_BUSY); // Wait until the CLCD is not busy

    CLCD_RW = LO;       // Set the Read/Write control bit to Write
    PORT_DIR = OUTPUT;  // Set the port direction to output
}

void init_clcd()
{
    /* Set PortD as output port for CLCD data */
    TRISD = 0x00;
    /* Set PortC as output port for CLCD control */
    TRISC = TRISC & 0xF8;

    CLCD_RW = LO; // Set the Read/Write control bit to Write

    /* Startup Time for the CLCD controller */
    __delay_ms(30);

    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(4100); // Delay for 4.1ms
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(100);  // Delay for 100us
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(1);    // Delay for 1us

    CURSOR_HOME;           // Set cursor to home position
    __delay_us(100);       // Delay for 100us
    TWO_LINE_5x8_MATRIX_8_BIT; // Configure display for 2 lines, 5x8 dots, 8-bit mode
    __delay_us(100);       // Delay for 100us
    CLEAR_DISP_SCREEN;     // Clear the display screen
    __delay_us(500);       // Delay for 500us
    DISP_ON_AND_CURSOR_OFF; // Turn on display, cursor off
    __delay_us(100);       // Delay for 100us
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND); // Set the address to start writing
    while (*data != '\0') // Loop until the end of the string
    {
        clcd_write(*data++, DATA_COMMAND); // Write each character to the CLCD
    }
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND); // Set the address to write the character
    clcd_write(data, DATA_COMMAND); // Write the character to the CLCD
}
