#include <stdint.h>
#include "reg.h"
#define USART_TX 6 //PB6
#define USART_RX 7 //PB7

void init_usart1(void)
{
	//PB6 PB7
	//RCC EN GPIOB
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTB));
	//GPIO Configurations - PB6
        //MODE = 10 --> AF
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_1_BIT(USART_TX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_0_BIT(USART_TX));
	//OTY = 0
        CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OTYPER_OFFSET, OTy_BIT(USART_TX)); 
	//SPEED = 01 --> Medium
        CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(USART_TX)); 
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(USART_TX)); 
	//PUPDR = 00 --> push pull
        CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(USART_TX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(USART_TX));    
	//AF low reg = 7 --> USART1
        WRITE_BITS(GPIO_BASE(GPIO_PORTB) + GPIOx_AFRL_OFFSET, AFRLy_3_BIT(USART_TX), AFRLy_0_BIT(USART_TX), 7);

        //GPIO Configurations - PB7
        //MODE = 10 --> AF
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_1_BIT(USART_RX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_0_BIT(USART_RX));  
	//OTY = 0
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OTYPER_OFFSET, OTy_BIT(USART_RX));  
	//SPEED = 01 --> Medium
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(USART_RX)); 
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(USART_RX));
	//PUPDR = 00 --> push pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(USART_RX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(USART_RX));    
	//AFRL(for pin 0~7) = 7 --> USART1
	WRITE_BITS(GPIO_BASE(GPIO_PORTB) + GPIOx_AFRL_OFFSET, AFRLy_3_BIT(USART_RX), AFRLy_0_BIT(USART_RX), 7);
	
	//RCC EN USART1
	SET_BIT(RCC_BASE + RCC_APB2ENR_OFFSET, USART1EN);

	//Baud
	const unsigned int BAUD = 115200;
	const unsigned int SYSCLK_MHZ = 168;
	const double USARTDIV = SYSCLK_MHZ * 1.0e6 / 16 / BAUD; //USARTDIV = xxx.yyy
        //DIV_MANTISSA = xxx
        //DIV_FRACTION = 0.yyy * 16
	const uint32_t DIV_MANTISSA = (uint32_t) USARTDIV;
	const uint32_t DIV_FRACTION = (uint32_t) ((USARTDIV - DIV_MANTISSA)*16);

	//USART  Configurations
	//1. USART Enable
        //2. USART Baud Setting
        //3. Transmit Enable
        //4. Receive Enable
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, UE_BIT);
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, TE_BIT);
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, RE_BIT);
        //Set mantissa of BAUD
        WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_MANTISSA_11_BIT, DIV_MANTISSA_0_BIT, DIV_MANTISSA);
        //Set fraction of BAUD
        WRITE_BITS(USART1_BASE + USART_BRR_OFFSET, DIV_FRACTION_3_BIT, DIV_FRACTION_0_BIT, DIV_FRACTION);

}

void usart1_send_char(const char ch)
{
	//Write char to RDR (in USART DR)
        //Waiting for TXE become 1
        while(!READ_BIT(USART1_BASE + USART_SR_OFFSET, TXE_BIT));
        REG(USART1_BASE + USART_DR_OFFSET) = ch;
}

char usart1_receive_char(void)
{
	//Waiting for RXNE become 1
	//Read char from RDR (in USART DR)
        while(!READ_BIT(USART1_BASE + USART_SR_OFFSET, RXNE_BIT));
        return (char) REG(USART1_BASE + USART_DR_OFFSET);
}

int main(void)
{
	init_usart1();

	char *hello = "Hello world!\r\n";

	//send Hello world
	while (*hello != '\0')
		usart1_send_char(*hello++);


	//receive char and resend it
	char ch;
	while (1)
	{
		ch = usart1_receive_char();

		if (ch == '\r')
			usart1_send_char('\n');

		usart1_send_char(ch);
	}
}
