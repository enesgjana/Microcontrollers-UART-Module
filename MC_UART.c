//@author: Enes Gjana
//TM4C1294 LED Control via UART

#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAXSIZE 50

void send_prompt(void);
void init_port_m(void);
void decode_command(char* buffer);

void main(void)
{
    char buffer[MAXSIZE];
    int i=0;

    // Initialize Port M for LEDs
    init_port_m();

    // initialize Port D for UART2 Rx (PD4) and Tx (PD5)
    SYSCTL_RCGCGPIO_R |= 0x00000008;
    while(!(SYSCTL_PRGPIO_R & 0x08));
    GPIO_PORTD_AHB_DEN_R |= 0x30;
    GPIO_PORTD_AHB_AFSEL_R |= 0x30;
    GPIO_PORTD_AHB_PCTL_R |= 0x00110000;

    // Initialize UART2
    SYSCTL_RCGCUART_R |= 0x00000004;
    while(!(SYSCTL_PRUART_R & 0x04));
    UART2_CTL_R &= ~0x0001;

    UART2_IBRD_R = 8;
    UART2_FBRD_R = 44;
    UART2_LCRH_R = 0x00000060;
    UART2_CTL_R |= 0x0301;

    while(1) {
        send_prompt();  // Requirement 4: Send '>' prompt
        i = 0;

        while(i < MAXSIZE-1) {
            while(UART2_FR_R & (1<<4))
                ;
            buffer[i] = UART2_DR_R;

            if (buffer[i] == 0x0D) {  // Enter key pressed
                buffer[i] = 0x00;
                break;
            }
            i++;
        }

        if (i >= MAXSIZE-1) {
            buffer[MAXSIZE-1] = 0x00;
        }

        // Requirement 5: Output received string with printf()
        printf("%s\n", buffer);

        // Requirement 1-3: Decode and execute command
        decode_command(buffer);
    }
}

// Initialize Port M for LED control
void init_port_m(void)
{
    SYSCTL_RCGCGPIO_R |= 0x00000800;  // Enable clock for Port M
    while(!(SYSCTL_PRGPIO_R & 0x0800));  // Wait for Port M to be ready
    GPIO_PORTM_DEN_R |= 0x0F;  // Enable PM0-PM3 as digital
    GPIO_PORTM_DIR_R |= 0x0F;  // Set PM0-PM3 as outputs
    GPIO_PORTM_DATA_R &= ~0x0F;  // Initialize all LEDs off
}

// Decode and execute LED command
void decode_command(char* buffer)
{
    // Requirement 1: Valid format is "led<+|-><0|1|2|3>"
    // Check if command starts with "led"
    if (buffer[0] != 'l' || buffer[1] != 'e' || buffer[2] != 'd') {
        return;  // Requirement 3: Abort if syntax violated
    }

    // Check if character 4 is + or -
    char action = buffer[3];
    if (action != '+' && action != '-') {
        return;  // Requirement 3: Abort if syntax violated
    }

    // Check if character 5 is a digit 0-3
    char led_num = buffer[4];
    if (led_num < '0' || led_num > '3') {
        return;  // Requirement 3: Abort if syntax violated
    }

    // Check if string ends here (character 6 should be null terminator)
    if (buffer[5] != 0x00) {
        return;  // Requirement 3: Abort if syntax violated
    }

    // Convert LED number from ASCII to integer
    int led_index = led_num - '0';  // '0'->0, '1'->1, '2'->2, '3'->3

    // Requirement 2: Control the LED
    if (action == '+') {
        // Turn on LED (set bit)
        GPIO_PORTM_DATA_R |= (1 << led_index);
    } else {
        // Turn off LED (clear bit)
        GPIO_PORTM_DATA_R &= ~(1 << led_index);
    }
}

// Send prompt characters: \r\n>
void send_prompt(void)
{
    // Send 0x0D (carriage return)
    while(UART2_FR_R & (1<<5))
        ;
    UART2_DR_R = 0x0D;

    // Send 0x0A (line feed)
    while(UART2_FR_R & (1<<5))
        ;
    UART2_DR_R = 0x0A;

    // Send '>' (prompt symbol)
    while(UART2_FR_R & (1<<5))
        ;
    UART2_DR_R = '>';
}