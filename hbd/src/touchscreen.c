/*
 * touchscreen.c
 *
 *  Created on: Aug 26, 2015
 *      Author: David
 */


#include "stm32f0xx.h"
#include "gpio.h"
#include "gui.h"
#include "touchscreen.h"
#include "main.h"


static void touch_start_detect(void);
static uint16_t touch_get_position_x(void);
static uint16_t touch_get_position_y(void);

void touch_init(void)
{
	TOUCH_GPIO_P_CLK_ENABLE();
	TOUCH_GPIO_N_CLK_ENABLE();
	TOUCH_ADC_CLK_ENABLE();
	MAIN_Delay(10);

	// Begin ADC calibration
	ADC1->CR |= ADC_CR_ADCAL;
	while(ADC1->CR != 0){}
	// Turn on the ADC
	MAIN_Delay(10);
	ADC1->CR |= ADC_CR_ADEN;

	// Start in detect-touch mode
	touch_start_detect();
}

static void touch_start_detect(void)
{
	// Y+ pin set as inputs, weak pullup on
	// If the Y+ pin is low, the touchscreen is being pressed.
	gpio_pinmode(TOUCH_P_PORT,TOUCH_YP_PIN,GPIO_Input);
	gpio_pullup(TOUCH_P_PORT,TOUCH_YP_PIN,GPIO_PullUp);
	// Y- pin set as analog input (so it affects nothing)
	gpio_pinmode(TOUCH_N_PORT,TOUCH_YN_PIN,GPIO_Analog);

	// Pull the X+ and X- pins down
	gpio_pinmode(TOUCH_N_PORT,TOUCH_XN_PIN,GPIO_Output);
	gpio_pinmode(TOUCH_P_PORT,TOUCH_XP_PIN,GPIO_Output);
	TOUCH_P_PORT->BRR = 1<<TOUCH_XP_PIN;
	TOUCH_N_PORT->BRR = 1<<TOUCH_XN_PIN;

	/* HAL Version
	GPIO_InitTypeDef gi;

	// Y+ pin set as inputs, weak pullup on
	// If the Y+ pin is low, the touchscreen is being pressed.
	gi.Pin = TOUCH_YP_PIN;
	gi.Mode = GPIO_MODE_INPUT;
	gi.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(TOUCH_P_PORT, &gi);
	// Y- pin set as analog input (so it affects nothing)
	gi.Pin = TOUCH_YN_PIN;
	gi.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(TOUCH_N_PORT, &gi);

	gi.Pin = TOUCH_XN_PIN;
	gi.Mode = GPIO_MODE_OUTPUT_PP;
	gi.Speed = GPIO_SPEED_HIGH;
	gi.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TOUCH_N_PORT, &gi);
	gi.Pin = TOUCH_XP_PIN;
	gi.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(TOUCH_P_PORT, &gi);

	// Pull the X+ and X- pins down
	TOUCH_P_PORT->BRR = 1<<TOUCH_XP_PIN;
	TOUCH_N_PORT->BRR = 1<<TOUCH_XN_PIN;
	*/
}

static uint16_t touch_get_position_x(void)
{
	//
	uint32_t touch_sum=0;
	uint32_t tickstart;

	// Set Y+ and Y- as analog
	gpio_pinmode(TOUCH_N_PORT, TOUCH_YN_PIN, GPIO_Analog);
	gpio_pinmode(TOUCH_P_PORT, TOUCH_YP_PIN, GPIO_Analog);
	// Set X+ and X- as outputs
	gpio_pinmode(TOUCH_N_PORT, TOUCH_XN_PIN, GPIO_Output);
	gpio_pinmode(TOUCH_P_PORT, TOUCH_XP_PIN, GPIO_Output);
	// Set X+ positive, X- negative
	TOUCH_P_PORT->BSRR |= (1<<TOUCH_XP_PIN);
	TOUCH_N_PORT->BRR |= (1<<TOUCH_XN_PIN);


	/* HAL Version
	GPIO_InitTypeDef gi;
	// Set Y+ and Y- as analog
	gi.Pin = TOUCH_YP_PIN;
	gi.Mode = GPIO_MODE_ANALOG;
	gi.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TOUCH_P_PORT, &gi);
	gi.Pin = TOUCH_YN_PIN;
	HAL_GPIO_Init(TOUCH_N_PORT, &gi);

	// Set X+ and X- as outputs
	gi.Pin = TOUCH_XP_PIN;
	gi.Mode = GPIO_MODE_OUTPUT_PP;
	gi.Pull = GPIO_NOPULL;
	gi.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(TOUCH_P_PORT, &gi);
	gi.Pin = TOUCH_XN_PIN;
	HAL_GPIO_Init(TOUCH_N_PORT, &gi);

	// Set X+ positive, X- negative
	TOUCH_P_PORT->BSRR = TOUCH_XP_PIN;
	TOUCH_N_PORT->BRR = TOUCH_XN_PIN;
	*/

	// Start an adc conversion
	ADC1->CHSELR = TOUCH_Y_CHANNEL;

	MAIN_Delay(TOUCH_DELAY_MS);

	for(uint8_t i=0; i<TOUCH_NUM_AVGS;i++)
	{
		ADC1->CR |= ADC_CR_ADSTART;

		// Wait for completion
		tickstart = MAIN_GetTick();
		while((ADC1->ISR & ADC_ISR_EOC) == 0)
		{
			if((MAIN_GetTick() - tickstart) > ADC_TIMEOUT)
			{
				return 0;
			}
		}

		ADC1->ISR |= ADC_ISR_EOC;
		touch_sum += ADC1->DR;
	}
	return (touch_sum / TOUCH_NUM_AVGS);
}

static uint16_t touch_get_position_y(void)
{
	uint32_t touch_sum=0;
	uint32_t tickstart;

	// Set X+ and X- as analog
	gpio_pinmode(TOUCH_N_PORT, TOUCH_XN_PIN, GPIO_Analog);
	gpio_pinmode(TOUCH_P_PORT, TOUCH_XP_PIN, GPIO_Analog);
	// Set Y+ and Y- as outputs
	gpio_pinmode(TOUCH_N_PORT, TOUCH_YN_PIN, GPIO_Output);
	gpio_pinmode(TOUCH_P_PORT, TOUCH_YP_PIN, GPIO_Output);
	// Set Y+ positive, Y- negative
	TOUCH_P_PORT->BSRR |= (1<<TOUCH_YP_PIN);
	TOUCH_N_PORT->BRR |= (1<<TOUCH_YN_PIN);

	/** HAL Version
	GPIO_InitTypeDef gi;
	// Set X+ and X- as analog
	gi.Pin = TOUCH_XP_PIN;
	gi.Mode = GPIO_MODE_ANALOG;
	gi.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TOUCH_P_PORT, &gi);
	gi.Pin = TOUCH_XN_PIN;
	HAL_GPIO_Init(TOUCH_N_PORT, &gi);

	// Set Y+ and Y- as outputs
	gi.Pin = TOUCH_YP_PIN;
	gi.Mode = GPIO_MODE_OUTPUT_PP;
	gi.Pull = GPIO_NOPULL;
	gi.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(TOUCH_P_PORT, &gi);
	gi.Pin = TOUCH_YN_PIN;
	HAL_GPIO_Init(TOUCH_N_PORT, &gi);

	// Set X+ positive, X- negative
	TOUCH_P_PORT->BSRR = TOUCH_YP_PIN;
	TOUCH_N_PORT->BRR = TOUCH_YN_PIN;
	*/

	// Start an adc conversion
	ADC1->CHSELR = TOUCH_X_CHANNEL;
	MAIN_Delay(TOUCH_DELAY_MS);

	for(uint8_t i=0; i<TOUCH_NUM_AVGS;i++)
	{
		ADC1->CR |= ADC_CR_ADSTART;

		// Wait for completion
		tickstart = MAIN_GetTick();
		while((ADC1->ISR & ADC_ISR_EOC) == 0)
		{
			if(MAIN_GetTick() - tickstart > ADC_TIMEOUT)
			{
				return 0;
			}
		}

		ADC1->ISR |= ADC_ISR_EOC;
		touch_sum += ADC1->DR;
	}

	return (touch_sum / TOUCH_NUM_AVGS);
}

uint8_t touch_is_pressed(void)
{
	// Checks if touchscreen is pressed.
	if((TOUCH_P_PORT->IDR & (1<<TOUCH_YP_PIN)) == 0)
	{
		return GUI_TOUCH_PRESSED;
	}
	return GUI_TOUCH_RELEASED;
}

uint8_t touch_get_position(GUI_POS* pos)
{
	if(touch_is_pressed())
	{
		pos->ypos = (0x1000 - touch_get_position_x());
		pos->xpos = touch_get_position_y();
		touch_start_detect();
		return GUI_TOUCH_PRESSED;
	}
	else
		return GUI_TOUCH_RELEASED;
}
