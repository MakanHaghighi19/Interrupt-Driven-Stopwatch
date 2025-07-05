#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer0A.h"

// External declarations for shared variables and functions

extern void displayDigit(uint8_t digit, uint8_t display);

void Timer0A_Init(uint32_t period) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;  // Activate Timer0
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;  // Disable Timer0A during setup
    TIMER0_CFG_R = 0x00000000;  // Configure for 32-bit timer mode
    TIMER0_TAMR_R = 0x02;  // Configure for periodic mode (TAMR = 0x02)
    TIMER0_TAILR_R = period - 1;  // Set the reload value (period)
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;  // Clear any previous interrupts
    TIMER0_IMR_R |= TIMER_IMR_TATOIM;  // Enable timeout interrupt
    NVIC_PRI4_R = (NVIC_PRI4_R & 0x00FFFFFF) | 0x40000000;  // Set priority to 2
    NVIC_EN0_R |= 1 << 19;  // Enable interrupt 19 in NVIC (Timer0A)
    TIMER0_CTL_R |= TIMER_CTL_TAEN;  // Enable Timer0A
}

// Timer0A ISR to handle time-multiplexing of the display
void Timer0A_Handler(void) {
    // Step 9: Acknowledge Timer0A timeout interrupt
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;  // Clear the interrupt flag

    uint8_t digits[4];

    // Step 10: Split the Displayed_Number into individual digits
    digits[0] = (Displayed_Number / 1000) % 10; // Thousands place
    digits[1] = (Displayed_Number / 100) % 10;  // Hundreds place
    digits[2] = (Displayed_Number / 10) % 10;   // Tens place
    digits[3] = Displayed_Number % 10;          // Ones place

    // Step 11: Display the current digit
    displayDigit(digits[currentDigit], currentDigit);

    // Step 12: Move to the next digit
    currentDigit = (currentDigit + 1) % 4;  // Loop back after last digit
}
