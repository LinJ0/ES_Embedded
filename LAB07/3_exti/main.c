#include <stdint.h>
#include "reg.h"
#include "blink.h"
#define USER_BUTTON 0 //PA0
int main(void)
{
	//RCC Enable GPIOA
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

	//GPIO Configurations
	//GPIO-MODER = 00 -->input
        //GPIO-OTYPER = x
        //GPIO-OSPEEDR = xx
        //GPIO-PUPDR = 10 -->pull-down
        CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(USER_BUTTON));
        CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(USER_BUTTON));
        
        SET_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(USER_BUTTON));
        CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(USER_BUTTON));

	//RCC Enable SYSCFG
	SET_BIT(RCC_BASE + RCC_APB2ENR_OFFSET, SYSCFGEN_BIT);

	//SYSCFG Configurations
        //Select EXTI0 source input PA0
	WRITE_BITS(SYSCFG_BASE + SYSCFG_EXTICR1_OFFSET, EXTI0_3_BIT, EXTI0_0_BIT, 0b0000);

	//EXTI Configurations
	SET_BIT(EXTI_BASE + EXTI_IMR_OFFSET, USER_BUTTON);    //Not mask line 0 interrupt
        CLEAR_BIT(EXTI_BASE + EXTI_RTSR_OFFSET, USER_BUTTON); //Disable rising triger
        SET_BIT(EXTI_BASE + EXTI_FTSR_OFFSET, USER_BUTTON);   //Enable falling trigger

	//NVIC Configurations
	SET_BIT(NVIC_ISER_BASE + NVIC_ISERn_OFFSET(0), 6); //Enable IRQ6

	blink(LED_BLUE);
}

void exti0_handler(void)
{
	blink_count(LED_ORANGE, 10);

	//clear pending
	SET_BIT(EXTI_BASE + EXTI_PR_OFFSET, 0); //pending bit is clear by programming it to 1
}
