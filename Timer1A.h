/*
 * Timer1A.h
 *
 *  Created on: Oct 22, 2024
 *      Author: MakanHaghighi
 */

#ifndef TIMER1A_H_
#define TIMER1A_H_

#include <stdint.h>
extern volatile uint8_t stopwatchRunning;  // 0 = stopped, 1 = running
extern volatile uint32_t stopwatchTime;    // Time in milliseconds

void Timer1A_Init(uint32_t period);
void Timer1A_Handler();


#endif /* TIMER1A_H_ */
