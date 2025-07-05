// Timer0A.h
// Runs on Tiva-C

// Adapted from SysTick.h from the book:
/* "Embedded Systems: Introduction to MSP432 Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
   Volume 1, Program 4.7
*/

#ifndef __TIMER0A_H__
#define __TIMER0A_H__

#include <stdint.h>
extern volatile uint32_t Displayed_Number;
extern volatile uint8_t currentDigit;     // Shared variable to keep track of the current digit being displayed


// Initialize Timer0A for periodic interrupts
void EnableInterrupts();
void Timer0A_Init(uint32_t period);
void Timer0A_Handler();

#endif

