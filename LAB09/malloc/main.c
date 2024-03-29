#include <stdint.h>
#include <stdlib.h> //in order to use malloc()
#include "reg.h"
#include "blink.h"
#define USART_TX 6 //PB6
#define USART_RX 7 //PB7
#define HEAP_MAX (64 * 1024) //64 KB
void *_sbrk(int incr)
{
	extern uint8_t _mybss_vma_end; //Defined by the linker script
	static uint8_t *heap_end = NULL;
	uint8_t *prev_heap_end;

	if (heap_end == NULL)
		heap_end = &_mybss_vma_end;

	prev_heap_end = heap_end;
	if (heap_end + incr > &_mybss_vma_end + HEAP_MAX)
		return (void *)-1;

	heap_end += incr;
	return (void *)prev_heap_end;
}

void init_usart1(void)
{
	//PB6 PB7
	//RCC EN GPIOB
	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTB));
	
        /*
         * GPIO Configurations
         * MODE = 10 --> AF
	 * OTY = 0
	 * SPEED = 01 --> Medium
	 * PUPDR = 00 --> push pull
	 * AFRL = 7 --> USART1
         */
	
        //GPIO Configurations - PB6
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_1_BIT(USART_TX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_0_BIT(USART_TX));
        CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OTYPER_OFFSET, OTy_BIT(USART_TX)); 
        CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(USART_TX)); 
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(USART_TX)); 
        CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(USART_TX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(USART_TX));    
        WRITE_BITS(GPIO_BASE(GPIO_PORTB) + GPIOx_AFRL_OFFSET, AFRLy_3_BIT(USART_TX), AFRLy_0_BIT(USART_TX), 7);

        //GPIO Configurations - PB7
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_1_BIT(USART_RX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_MODER_OFFSET, MODERy_0_BIT(USART_RX));  
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OTYPER_OFFSET, OTy_BIT(USART_RX));  
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(USART_RX)); 
	SET_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(USART_RX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(USART_RX));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTB) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(USART_RX));    
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

	/*
         * USART + Interrupt Configurations
	 * 1. USART Enable --> UE
         * 2. USART Baud Setting
         * 3. Transmit Enable --> TE
         * 4. Receive Enable --> RE
         * 5. UASRT Interrupt Enable --> RXNEIE
         * 6. NVIC - IRQ37 --> (m+(32*n)) | m=5, n=1
         */

	// USART Configurations
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, UE_BIT);        //1.
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, TE_BIT);        //3.
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, RE_BIT);        //4.
        SET_BIT(USART1_BASE + USART_CR1_OFFSET, RXNEIE_BIT);    //5.
        SET_BIT(NVIC_ISER_BASE + NVIC_ISERn_OFFSET(1), 5);      //6.
        //2.
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
        //Without set RXNEIE,waiting for RXNE become 1
        //while(!READ_BIT(USART1_BASE + USART_SR_OFFSET, RXNE_BIT));
        //If enable RXNEIE, call usart1_handler() when RXNE become 1 
        
        return (char) REG(USART1_BASE + USART_DR_OFFSET);
}

void usart1_handler()
{
        if(READ_BIT(USART1_BASE + USART_SR_OFFSET, ORE_BIT)){   //if overrun, send '!'
                usart1_send_char('!');
                blink_count(LED_RED, 20);
        }else if(READ_BIT(USART1_BASE + USART_SR_OFFSET, RXNE_BIT)){
                char ch;                

                ch = usart1_receive_char();
                if(ch == '\r') usart1_send_char('\n');
                usart1_send_char(ch);
                //Transmission Complete, Clear to Send or Transmit Data Register empty interrupt.
        }
}


int main(void)
{
	init_usart1();

        char *ch = malloc(3 * sizeof(char));

        if(ch != NULL){
                ch[0] = 'A';
                ch[1] = 'B';
                ch[2] = 'C';
                
                usart1_send_char(*ch);
                usart1_send_char(*(ch + 1));
                usart1_send_char(*(ch + 2));
        
                free(ch);        
        }

        
        blink(LED_BLUE);
}
