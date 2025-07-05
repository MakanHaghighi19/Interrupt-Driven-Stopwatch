#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer1A.h"

// External declarations for shared variables
extern volatile uint32_t Displayed_Number;    // Declare extern for stopwatch time
extern volatile uint8_t stopwatchRunning;  // Declare extern for stopwatch running state

// Initialize Timer1A for 1 ms periodic interrupts
void Timer1A_Init(uint32_t period) {
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;  // Activate Timer1
    TIMER1_CTL_R &= ~TIMER_CTL_TAEN;            // Disable Timer1A during setup
    TIMER1_CFG_R = 0x00000000;                  // Configure for 32-bit timer mode
    TIMER1_TAMR_R = 0x02;                       // Periodic mode
    TIMER1_TAILR_R = period - 1;                // Set the reload value for 1 ms
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;          // Clear any prior interrupts
    TIMER1_IMR_R |= TIMER_IMR_TATOIM;           // Arm timeout interrupt
    NVIC_PRI5_R = (NVIC_PRI5_R & 0xFFFF1FFF) | 0x00006000; // Priority 3
    NVIC_EN0_R |= 1 << 21;                      // Enable IRQ 21 in NVIC (Timer1A)
    TIMER1_CTL_R |= TIMER_CTL_TAEN;             // Enable Timer1A
}

// Timer1A ISR for incrementing the stopwatch
void Timer1A_Handler(void) {
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;  // Acknowledge the interrupt
    if (stopwatchRunning) {
        Displayed_Number++;                // Increment the stopwatch time by 1 ms
    }
}
