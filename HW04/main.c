#include "blink.h"
#include "button.h"

int main(void)
{   
    unsigned int led_flag = 0;
    unsigned int pre_state = 0;

    for(;;pre_state = button_read(BUTTON_USER)){
        (led_flag)? blink_count(LED_BLUE, 1) : blink_count(LED_ORANGE, 1);
        led_flag ^= (pre_state == button_read(BUTTON_USER))? 0 : button_read(BUTTON_USER);
    }
}
