#include <stdint.h>
#include "reg.h"
#include "blink.h"

void setup_systick(uint32_t ticks);

int main(void)
{
	setup_systick(??????); //200 ms
	blink(LED_BLUE); //should not return
}

void setup_systick(uint32_t ticks)
{
	// set reload value
	??????

	// uses external reference clock
	??????

	// enable systick exception
	??????

	// enable systick
	??????
}

void systick_handler(void)
{
	blink_count(LED_ORANGE, 1);
}
