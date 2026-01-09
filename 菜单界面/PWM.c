#include <STC89C5xRC.H>
#include "PWM.h"
#include "Delay.h"

sbit PWM_PIN = P1^5;

unsigned char pwm_duty = 0;
unsigned char pwm_counter = 0;
unsigned int pwm_timer = 0;

void PWM_Init(void)
{
    PWM_PIN = 0;
    pwm_duty = 0;
    pwm_counter = 0;
    pwm_timer = 0;
}

void PWM_SetDuty(unsigned char duty)
{
    if(duty > 100) duty = 100;
    pwm_duty = duty;
}

unsigned char PWM_GetDuty(void)
{
    return pwm_duty;
}

void PWM_Update(void)
{
    pwm_timer++;
    if(pwm_timer >= 10)  // 10msÖÜÆÚ£¬100Hz PWMÆµÂÊ
    {
        pwm_timer = 0;
        pwm_counter++;
        if(pwm_counter >= 100)
            pwm_counter = 0;
            
        if(pwm_counter < pwm_duty)
            PWM_PIN = 1;
        else
            PWM_PIN = 0;
    }
}