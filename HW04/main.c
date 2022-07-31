#include <stdint.h>
#include "blink.h"
#include "button.h"
#include "reg.h"

int main(void)
{
    button_init(BUTTON_USER);
    
    while(!READ_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET, IDRy_BIT(BUTTON_USER))){
        blink_count(LED_ORANGE, 1);
    }
    blink(LED_BLUE);
}
