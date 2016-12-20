/*
 * comm.c
 *
 *  Created on: Aug 27, 2015
 *      Author: David
 */

#include "stm32f0xx.h"
#include "comm.h"
#include "gpio.h"
#include "main.h"

#ifdef COMM_USED

#define MIN(x,y)		((x) < (y) ? (x) : (y))
#define MAX(x,y)		((x) > (y) ? (x) : (y))

CommBuffer_Type s_RxBuffer;
CommBuffer_Type s_TxBuffer;

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
	COMM_USART->CR1 = USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_TE;
	COMM_USART->CR2 = 0;
	COMM_USART->CR3 = 0;
	COMM_USART->BRR = COMM_BAUD_REG;

	COMM_USART->CR1 |= USART_CR1_UE;

	// Interrupt configuration
	NVIC_SetPriority(USART1_IRQn, 2);
	NVIC_EnableIRQ(USART1_IRQn);

	// Initialize variables
	s_RxBuffer.Done = 0;
	s_RxBuffer.RdPos = 0;
	s_RxBuffer.WrPos = 0;
	s_TxBuffer.Done = 1;
	s_TxBuffer.RdPos = 0;
	s_TxBuffer.WrPos = 0;
}

uint8_t comm_recv(uint8_t* buf, uint8_t count)
{
	uint8_t buffer_remaining = 0;
	uint8_t place = 0;
	// Copy up to "count" bytes from the read buffer
	if(!s_RxBuffer.Done)
	{
		// No bytes read, return zero.
		return 0;
	}
	if(s_RxBuffer.WrPos <= s_RxBuffer.RdPos)
	{
		buffer_remaining = COMM_BUFFER_LENGTH - s_RxBuffer.RdPos + s_RxBuffer.WrPos;
	}
	else
	{
		buffer_remaining = s_RxBuffer.WrPos - s_RxBuffer.RdPos;
	}
	while((place < count) && (place < buffer_remaining))
	{
		buf[place++] = s_RxBuffer.Buffer[s_RxBuffer.RdPos++];
		if(s_RxBuffer.RdPos > COMM_BUFFER_LENGTH)
			s_RxBuffer.RdPos = 0;
	}
	// Clear "done" flag if no more bytes to read
	if(count >= buffer_remaining) s_RxBuffer.Done = 0;
	// Return the number of read bytes.
	return place;
}

uint8_t comm_txmt(uint8_t* buf, uint8_t count)
{
	uint8_t place = 0;
	if(s_TxBuffer.Done)
	{
		// In the rare chance that the transmitter is just finishing,
		// wait for the shift register to empty
		uint32_t timeout = MAIN_GetTick();
		while(!(COMM_USART->ISR & USART_ISR_TXE))
		{
			if(MAIN_GetTick() > (timeout + COMM_TXMT_TIMEOUT))
			{
				return 0;
			}
		}

		// Safe to simply restart the buffer
		s_TxBuffer.RdPos = 0;
		s_TxBuffer.WrPos = 0;
		while((place < COMM_BUFFER_LENGTH) && (place < count))
		{
			s_TxBuffer.Buffer[s_TxBuffer.WrPos++] = buf[place++];
		}
		// Start the transfer


		COMM_USART->TDR = s_TxBuffer.Buffer[s_TxBuffer.RdPos++];
		COMM_USART->CR1 |= (USART_CR1_TXEIE);
		s_TxBuffer.Done = 0;
	}
	else
	{
		// Can we fit more data in the buffer?
		uint8_t buffer_used;
		if(s_TxBuffer.WrPos <= s_TxBuffer.RdPos)
		{
			buffer_used = COMM_BUFFER_LENGTH - s_TxBuffer.RdPos + s_TxBuffer.WrPos;
		}
		else
		{
			buffer_used = s_TxBuffer.WrPos - s_TxBuffer.RdPos;
		}
		uint8_t buffer_remaining = COMM_BUFFER_LENGTH - buffer_used;

		if(count <= buffer_remaining)
		{
			while((place < buffer_remaining) && (place < count))
			{
				s_TxBuffer.Buffer[s_TxBuffer.WrPos++] = buf[place++];
			}
		}
		else
		{
			return 0;
		}
	}
	// Return the number of written bytes.
	return place;
}

void comm_IRQCallback(void)
{
	uint8_t comm_dummy;
	if((COMM_USART->ISR & USART_ISR_RXNE) != 0)
	{
		// Receive the new character
		comm_dummy = COMM_USART->RDR;
		// Add it to our buffer
		s_RxBuffer.Buffer[s_RxBuffer.WrPos++] = comm_dummy;
		s_RxBuffer.Done = 1;
		if(s_RxBuffer.WrPos >= COMM_BUFFER_LENGTH)
		{
			// Wrap around the circular buffer
			s_RxBuffer.WrPos = 0;
		}
	}
	if( ((COMM_USART->CR1 & USART_CR1_TXEIE) != 0) && ((COMM_USART->ISR & USART_ISR_TXE) != 0))
	{
		// Check if more data to send
		if( ((s_TxBuffer.RdPos + 1) == s_TxBuffer.WrPos ) ||
				((s_TxBuffer.RdPos == COMM_BUFFER_LENGTH) && (s_TxBuffer.WrPos == 0)))
		{
			// We are done after this byte!
			s_TxBuffer.Done = 1;
			// Turn off the transmit data register empty interrupt
			COMM_USART->CR1 &= ~(USART_CR1_TXEIE);
			// Turn on the transmit complete interrupt
			COMM_USART->CR1 |= USART_CR1_TCIE;
		}

		COMM_USART->TDR = s_TxBuffer.Buffer[s_TxBuffer.RdPos++];
		if(s_TxBuffer.RdPos > COMM_BUFFER_LENGTH)
		{
			// Wrap around the read pointer
			s_TxBuffer.RdPos = 0;
		}
	}
	if( ((COMM_USART->CR1 & USART_CR1_TCIE) != 0) && ((COMM_USART->ISR & USART_ISR_TC) != 0) )
	{
		// Trasmission complete, turn off the transmitter and the interrupt
		COMM_USART->CR1 &= ~(USART_CR1_TCIE);
		// Clear the flag
		COMM_USART->ICR |= USART_ICR_TCCF;
	}
}

#endif
