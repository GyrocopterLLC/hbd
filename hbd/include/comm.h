/*
 * comm.h
 *
 *  Created on: Aug 27, 2015
 *      Author: David
 */

#ifndef COMM_H_
#define COMM_H_

#define COMM_USED

/*################# Pin and peripheral defines #######################*/

#define COMM_CLK_ENABLE()		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN)
#define COMM_GPIO_CLK_ENABLE()	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN)

#define COMM_TX_PIN				6//GPIO_PIN_6
#define COMM_RX_PIN				7//GPIO_PIN_7
#define COMM_PORT				GPIOB
#define COMM_AF					((uint8_t)0x00)

#define COMM_USART				USART1

/*######################## Parameter defines #########################*/

#define COMM_BAUDRATE			115200
#define COMM_BAUD_REG			417 //(48MHz / baudrate)
#define COMM_BUFFER_LENGTH		64


/*####################### Communication Format ######################*/

/**
 * [Start byte][Variable code][Format byte]{4 byte data payload}[Stop byte]
 */

void comm_init(void);
void comm_IRQCallback(void);

#endif /* COMM_H_ */
