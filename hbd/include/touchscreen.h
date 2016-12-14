/*
 * touchscreen.h
 *
 *  Created on: Aug 26, 2015
 *      Author: David
 */

#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

#define TOUCH_GPIO_P_CLK_ENABLE()	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN)
#define TOUCH_GPIO_N_CLK_ENABLE()	SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOFEN)
#define TOUCH_ADC_CLK_ENABLE()		SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN)

#define TOUCH_XP_PIN				0//GPIO_PIN_0
#define TOUCH_YP_PIN				1//GPIO_PIN_1
#define TOUCH_P_PORT				GPIOB

#define TOUCH_X_CHANNEL				ADC_CHSELR_CHSEL8
#define TOUCH_Y_CHANNEL				ADC_CHSELR_CHSEL9

#define TOUCH_XN_PIN				6//GPIO_PIN_6
#define TOUCH_YN_PIN				7//GPIO_PIN_7
#define TOUCH_N_PORT				GPIOF

#define TOUCH_DETECT				1
#define TOUCH_POSITION				2

#define TOUCH_DELAY_MS				1 // Wait before starting ADC.
#define TOUCH_NUM_AVGS				16 // Number of samples to take

#define ADC_TIMEOUT					20 // Maximum time for ADC sampling (ms)

void touch_init(void);
uint8_t touch_is_pressed(void);
uint8_t touch_get_position(GUI_POS* pos);

#endif /* TOUCHSCREEN_H_ */
