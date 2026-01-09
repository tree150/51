#ifndef __PWM_H__
#define __PWM_H__

void PWM_Init(void);
void PWM_SetDuty(unsigned char duty);
void PWM_Update(void);
unsigned char PWM_GetDuty(void);

#endif