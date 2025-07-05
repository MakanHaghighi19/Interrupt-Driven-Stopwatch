#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Timer0A.h"
#include "Timer1A.h"

// Global variables
volatile uint32_t Displayed_Number = 0;  // Value to display (change as needed)
volatile uint8_t currentDigit = 0;          // Current digit being displayed
volatile uint8_t stopwatchRunning = 0;      // 0 = stopped, 1 = running

// Function prototypes
void sevenseg_init(void);
void CS_init(void);
void SSI2_write(uint8_t data, uint8_t csMask);
void displayDigit(uint8_t digit, uint8_t display);
void Buttons_Init(void);

// Main function
int main(void) {
    PLL_Init(Bus80MHz);         // Set clock to 80 MHz
    sevenseg_init();            // Initialize SSI2 for seven-segment display
    CS_init();                  // Initialize Chip Select for display
    Buttons_Init();             // Initialize buttons with interrupts
    Timer0A_Init(40000);        // Initialize Timer0A for display multiplexing (0.5 ms)
    Timer1A_Init(1600000);  // Assuming 80 MHz clock, 20 ms debounce period

    while (1) {
    }

    return 0;
}

// Buttons are connected to PA2 (Start/Stop), PA3 (Increment), PA4 (Reset)
void Buttons_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x01;
    while ((SYSCTL_PRGPIO_R & 0x01) == 0);

    GPIO_PORTA_DIR_R &= ~0x1C;
    GPIO_PORTA_DEN_R |= 0x1C;
    GPIO_PORTA_PUR_R |= 0x1C;
    GPIO_PORTA_IS_R &= ~0x1C;
    GPIO_PORTA_IBE_R &= ~0x1C;
    GPIO_PORTA_IEV_R &= ~0x1C;
    GPIO_PORTA_ICR_R = 0x1C;
    GPIO_PORTA_IM_R |= 0x1C;

    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF00) | 0x00000040;
    NVIC_EN0_R |= 1 << 0;
}

// GPIO Port A Interrupt Handler for buttons
void GPIOPortA_Handler(void) {
    if (!debounceFlag) {  // Skip handling if debounce timer is active
        TIMER1_CTL_R |= TIMER_CTL_TAEN; // Start Timer1A for debounce delay
        return;
    }

    debounceFlag = 0; // Reset debounce flag
    uint32_t status = GPIO_PORTA_RIS_R & 0x1C;
    GPIO_PORTA_ICR_R = 0x1C;

    if (status & 0x04) {            // PA2 - Start/Stop button
        stopwatchRunning ^= 1;
    }
    if (status & 0x08) {            // PA3 - Increment button
        if (!stopwatchRunning) {
            Displayed_Number += 1;
        }
    }
    if (status & 0x10) {            // PA4 - Reset button
        stopwatchRunning = 0;
        Displayed_Number = 0;
    }
}


// Initialize SSI2 module for seven-segment display
void sevenseg_init(void) {
    SYSCTL_RCGCSSI_R |= 0x04;    // Enable clock to SSI2
    SYSCTL_RCGCGPIO_R |= 0x02;   // Enable clock to GPIOB

    while ((SYSCTL_PRGPIO_R & 0x02) == 0); // Wait for GPIOB
    while ((SYSCTL_PRSSI_R & 0x04) == 0);  // Wait for SSI2

    // Configure GPIOB pins for SSI2
    GPIO_PORTB_AMSEL_R &= ~0x90;      // Disable analog on PB7, PB4
    GPIO_PORTB_AFSEL_R |= 0x90;       // Enable alt funct on PB7, PB4
    GPIO_PORTB_PCTL_R &= ~0xF00F0000; // Clear PCTL on PB7, PB4
    GPIO_PORTB_PCTL_R |= 0x20020000;  // Configure PB7, PB4 for SSI2
    GPIO_PORTB_DEN_R |= 0x90;         // Enable digital I/O on PB7, PB4

    SSI2_CR1_R = 0;              // Disable SSI2 for configuration
    SSI2_CC_R = 0;               // Use system clock
    SSI2_CPSR_R = 16;            // Prescaler to get 1 MHz clock
    SSI2_CR0_R = 0x0007;         // Freescale mode, 8-bit data
    SSI2_CR1_R = 2;              // Enable SSI2 as master
}

// Initialize CS pin (PC7) for the seven-segment display
void CS_init(void) {
    SYSCTL_RCGCGPIO_R |= 0x04;    // Enable clock to GPIOC
    while ((SYSCTL_PRGPIO_R & 0x04) == 0); // Wait for GPIOC

    GPIO_PORTC_DIR_R |= 0x80;     // Set PC7 as output
    GPIO_PORTC_DEN_R |= 0x80;     // Enable digital I/O on PC7
    GPIO_PORTC_DATA_R |= 0x80;    // Set PC7 high (inactive)
}

// Send data via SSI2
void SSI2_write(uint8_t data, uint8_t csMask) {
    GPIO_PORTC_DATA_R &= ~csMask;  // Assert CS (active low)
    SSI2_DR_R = data;              // Send data
    while (SSI2_SR_R & 0x10) {}    // Wait for transmit complete
    GPIO_PORTC_DATA_R |= csMask;   // Deassert CS
}

void displayDigit(uint8_t digit, uint8_t display) {
    const static unsigned char digitPattern[] = {
        0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
    };
    const static unsigned char displayEnable[] = {
        0x08, 0x04, 0x02, 0x01
    };

    uint8_t pattern = digitPattern[digit];

    if (display == 0) {
        pattern &= ~0x80;
    }

    SSI2_write(pattern, 0x80);
    SSI2_write(displayEnable[display], 0x80);
}
