/*
 * comm.c
 *
 *  Created on: Aug 27, 2015
 *      Author: David
 */

#include "stm32f0xx.h"
#include "comm.h"
#include "gpio.h"

#ifdef COMM_USED

uint8_t comm_rx_buffer[COMM_BUFFER_LENGTH];
uint32_t comm_rx_place = 0;

uint8_t comm_tx_buffer[COMM_BUFFER_LENGTH];
uint32_t comm_tx_place = 0;
uint32_t comm_tx_tosend = 0;

uint8_t comm_status = 0;

void comm_init(void)
{
	// Start by turning on the clocks
	COMM_CLK_ENABLE();
	COMM_GPIO_CLK_ENABLE();

	// Set up the GPIOs
	gpio_pinmode(COMM_PORT,COMM_TX_PIN,GPIO_AlternateFunction);
	gpio_af(COMM_PORT,COMM_TX_PIN,COMM_AF);
	gpio_pinmode(COMM_PORT,COMM_RX_PIN,GPIO_AlternateFunction);
	gpio_af(COMM_PORT,COMM_RX_PIN,COMM_AF);

	// Set up the USART peripheral
	COMM_USART->CR1 = USART_CR1_RE | USART_CR1_RXNEIE;
	COMM_USART->CR2 = 0;
	COMM_USART->CR3 = 0;
	COMM_USART->BRR = COMM_BAUD_REG;

	COMM_USART->CR1 |= USART_CR1_UE;

	// Interrupt configuration
	NVIC_SetPriority(USART1_IRQn, 2);
	NVIC_EnableIRQ(USART1_IRQn);
}

void comm_IRQCallback(void)
{
	uint8_t comm_dummy;
	if((COMM_USART->ISR & USART_ISR_RXNE) != 0)
	{
		// Receive the new character
		comm_dummy = COMM_USART->RDR;
		// Add it to our buffer
		if(comm_rx_place < COMM_BUFFER_LENGTH)
		{
			comm_rx_buffer[comm_rx_place++] = comm_dummy;
		}
	}
	if((COMM_USART->ISR & USART_ISR_TXE) != 0)
	{
		// Check if more data to send
		if((comm_tx_place < COMM_BUFFER_LENGTH)&&(comm_tx_place < comm_tx_tosend))
		{
			// Send next byte
			COMM_USART->TDR = comm_tx_buffer[comm_tx_place++];
		}
		else
		{
			// No more data in the buffer, disable the TXE interrupt
			COMM_USART->CR1 &= ~(USART_CR1_TXEIE);
		}
	}
}

#endif
