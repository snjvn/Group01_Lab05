
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
/**
 * main.c
 */

void GPIOInterrupt(void);
int main(void)
{
    SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x1F;             /* make PORTF configurable */
    GPIO_PORTF_DEN_R = 0x01F;            /* set PORTF pins 4 : 0 pins */
    GPIO_PORTF_DIR_R = 0x0E;            /*  */
    GPIO_PORTF_PUR_R = 0x11;            /* PORTF0 and PORTF4 are pulled up */

    NVIC_EN0_R = 0x40000000; // 30th bit controls PORTF
    GPIO_PORTF_IS_R = 0x10; // sw2 level sensitive; sw1 edge sensitive
    GPIO_PORTF_IBE_R = 0x00; //Single edge
    GPIO_PORTF_IEV_R = 0x11; //Rising edge
    GPIO_PORTF_IM_R = 0x11; // unmasking both switches

    while(1){
//        int debounce = 0;
//        while(debounce < 1000){debounce ++;} //for debouncing
        NVIC_EN0_R = 0x40000000; // 30th bit controls PORTF
        GPIO_PORTF_IM_R = 0x11; // unmasking both switches
        GPIO_PORTF_DATA_R ^= 0x08;
    }
	return 0;
}

void GPIOInterrupt(){
    if(GPIO_PORTF_RIS_R & 0x10){
        GPIO_PORTF_DATA_R = 0x08;
    }
    if(GPIO_PORTF_RIS_R & 0x01){
        GPIO_PORTF_DATA_R ^= 0x04;
        GPIO_PORTF_ICR_R = 0x01;
    }
    NVIC_EN0_R = 0x00000000; // 30th bit controls PORTF
    GPIO_PORTF_IM_R = 0x00; // masking both switches


}
