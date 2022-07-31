#include <stdint.h>
#include "reg.h"

//button init
void button_init(unsigned int button)
{
    SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));

    //MODER button pin = 00 => General purpose input mode
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_1_BIT(button));
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_MODER_OFFSET, MODERy_0_BIT(button));

    //OT button pin = x => xx

    //OSPEEDR button pin = 00 => Low speed
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(button));
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(button));

    //PUPDR button pin = 10 => Pull-down
    SET_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(button));
    CLEAR_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(button));
}

unsigned int button_read(unsigned int button)
{
    button_init(button);

    if(READ_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET, IDRy_BIT(button)))
        return 1;
    else    
        return 0;
}
