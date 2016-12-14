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

UART_HandleTypeDef comm_handle;
uint8_t comm_rx_buffer[COMM_BUFFER_LENGTH];
uint32_t comm_rx_place = 0;

uint8_t comm_status = 0;

static uint8_t comm_hash_keys[NUM_COMM_VARS] = {	'S', // Speed
													'I', // Current
													'V', // Voltage
													'E', // Errors
													'C'  // Configuration
												};

int32_t comm_hash_values[NUM_COMM_VARS];

static void comm_parse_data(void);

void comm_init(void)
{
	GPIO_InitTypeDef gi;

	// Start by turning on the clocks
	COMM_CLK_ENABLE();
	COMM_GPIO_CLK_ENABLE();

	// Set up the GPIOs
	gpio_pinmode(COMM_PORT,COMM_TX_PIN,GPIO_AlternateFunction);
	gpio_af(COMM_PORT,COMM_TX_PIN,COMM_AF);
	gpio_pinmode(COMM_PORT,COMM_RX_PIN,GPIO_AlternateFunction);
	gpio_af(COMM_PORT,COMM_RX_PIN,COMM_AF);

	// Set up the USART peripheral
	COMM_USART->CR1 = USART_CR1_TE | USART_CR1_RE;
	COMM_USART->BRR = COMM_BAUD_REG;

// TODO: Remove all this crud, finish usart setup
	gi.Pin = COMM_TX_PIN | COMM_RX_PIN;
	gi.Mode = GPIO_MODE_AF_PP;
	gi.Speed = GPIO_SPEED_HIGH;
	gi.Pull = GPIO_NOPULL;
	gi.Alternate = COMM_AF;
	HAL_GPIO_Init(COMM_PORT, &gi);

	// Set up the USART peripheral
	comm_handle.Instance = COMM_USART;
	comm_handle.Init.BaudRate = COMM_BAUDRATE;
	comm_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	comm_handle.Init.Mode = UART_MODE_TX_RX;
	comm_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	comm_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	comm_handle.Init.Parity = UART_PARITY_NONE;
	comm_handle.Init.StopBits = UART_STOPBITS_1;
	comm_handle.Init.WordLength = UART_WORDLENGTH_8B;

	HAL_UART_Init(&comm_handle);

	// Interrupt configuration
	HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void comm_start_rx(void)
{
	//HAL_UART_Receive_IT(&comm_handle, &comm_dummy, 1);
	COMM_USART->CR1 |= USART_CR1_RXNEIE;
}

void comm_IRQCallback(void)
{
	uint8_t comm_dummy;
	if((COMM_USART->ISR & USART_ISR_RXNE) != 0)
	{
		comm_dummy = COMM_USART->RDR;
		if((comm_status & COMM_STATUS_RECEIVING) == 0)
		{
			// Starting fresh. Check if a start byte came in.
			if(comm_dummy == COMM_START_BYTE)
			{
				comm_status |= COMM_STATUS_RECEIVING;
				comm_rx_place = 0;
			}
		}
		else
		{
			if(comm_rx_place <= 5)
			{
				comm_rx_buffer[comm_rx_place++] = comm_dummy;
			}
			else
			{
				comm_status &= ~(COMM_STATUS_RECEIVING);
				if(comm_dummy == COMM_STOP_BYTE)
				{
					// Transfer complete!
					comm_parse_data();
				}
			}
		}
		// Finish by re-enabling the receive interrupt
		COMM_USART->CR1 |= USART_CR1_RXNEIE;
		//HAL_UART_Receive_IT(&comm_handle, &comm_dummy, 1);
	}
}

static void comm_parse_data(void)
{
	uint8_t dataPlace = NUM_COMM_VARS;
	float temp;
	uint8_t* pTemp = (uint8_t*)&temp;
	// A complete data packet has arrived, now let's file it in the right bin
	// First byte: variable name
	for(uint8_t i = 0; i < NUM_COMM_VARS; i++)
	{
		if(comm_rx_buffer[0] == comm_hash_keys[i])
		{
			dataPlace = i;
		}
	}
	if(dataPlace >= NUM_COMM_VARS) // Wasn't a valid name
		return;
	switch(comm_rx_buffer[1])
	{
	case COMM_BINARY:
		// Directly copy the buffer into the integer storage variable
		comm_hash_values[dataPlace] = comm_rx_buffer[5] + (comm_rx_buffer[4]<<8) + (comm_rx_buffer[3]<<16) + (comm_rx_buffer[2]<<24);
		break;
	case COMM_CHARACTER:
		// Convert from ASCII to binary
		comm_hash_values[dataPlace] = comm_rx_buffer[5] - '0';
		comm_hash_values[dataPlace] += 10*(comm_rx_buffer[4] - '0');
		comm_hash_values[dataPlace] += 100*(comm_rx_buffer[3] - '0');
		comm_hash_values[dataPlace] += 1000*(comm_rx_buffer[2] - '0');
		break;

	/*
	case COMM_FLOATING_POINT:
		// Convert from a float to fixed point Q16

		// Directly drop in the 4 bytes of the single-precision floating point variable
		pTemp[0] = comm_rx_buffer[2];
		pTemp[1] = comm_rx_buffer[3];
		pTemp[2] = comm_rx_buffer[4];
		pTemp[3] = comm_rx_buffer[5];
		// Convert to fixed point by multiplying by 2^16
		comm_hash_values[dataPlace] = ((int32_t)(temp*65536.0f));
		break;
		*/
	default:
		// No idea what to do
		comm_hash_values[dataPlace] = 0;
	}
}

int32_t comm_get_data(uint8_t varName)
{
	for(uint8_t i = 0; i < NUM_COMM_VARS; i++)
	{
		if(varName == comm_hash_keys[i])
		{
			return comm_hash_values[i];
		}
	}
	return 0;
}

#endif
