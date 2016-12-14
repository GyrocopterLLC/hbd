/*
 * rtc.c
 *
 *  Created on: Aug 31, 2015
 *      Author: David
 */


#include "stm32f0xx.h"
#include "rtc.h"
#include "main.h"


/** Enables the RTC for reading/writing. Does not reset the RTC.
 *
 */
void rtc_init(void)
{
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
	PWR->CR |= PWR_CR_DBP; // Disable write protection of the backup domain
	MAIN_Delay(10);
	// Disable RTC write protection
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// Bypass shadow mode - because it works
	RTC->CR |= RTC_CR_BYPSHAD;

}

/** Resets the backup domain and enables counting off the external 32.768kHz crystal
 *
 */
void rtc_reset(RTC_DateType* initialDate, RTC_TimeType* initialTime)
{
	uint32_t tickstart;

	RCC->BDCR |= RCC_BDCR_BDRST;
	MAIN_Delay(10);
	RCC->BDCR &= ~(RCC_BDCR_BDRST);
	MAIN_Delay(10);

	RCC->BDCR |= RCC_BDCR_RTCEN + RCC_BDCR_RTCSEL_LSE + RCC_BDCR_LSEON;
	// Wait for the RCC_LSERDY bit
	tickstart = MAIN_GetTick();
	while(((RCC->BDCR) & RCC_BDCR_LSERDY) == 0)
	{
		if((MAIN_GetTick() - tickstart) > RTC_TIMEOUT)
		{
			return;
		}
	}

	// Enter initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	tickstart = MAIN_GetTick();
	while(((RTC->ISR) & RTC_ISR_INITF) == 0)
	{
		if((MAIN_GetTick() - tickstart) > RTC_TIMEOUT)
		{
			return;
		}
	}

	// Set prescalers to the default (PRE_A = 128, PRE_S = 256, so total prescaler = 32768)
	//RTC->PRER = (127 << 16) + 255;
	RTC->PRER = 0x007F00FF;

	rtc_set_date(initialDate);
	rtc_set_time(initialTime);

	// Exit initialization mode
	RTC->ISR &= ~(RTC_ISR_INIT);
}

void rtc_set_date(RTC_DateType* date)
{
	uint32_t tickstart;
	uint8_t reset_init = 0;

	uint32_t tempDR = 0;

	// Check if we need to enter initialization mode
	if((RTC->ISR & RTC_ISR_INITF) == 0)
	{
		reset_init = 1;
		RTC->ISR |= RTC_ISR_INIT;
		tickstart = MAIN_GetTick();
		while(((RTC->ISR) & RTC_ISR_INITF) == 0)
		{
			if((MAIN_GetTick() - tickstart) > RTC_TIMEOUT)
			{
				return;
			}
		}
	}

	if(date->WEEKDAY != 0) // 1-Monday through 7-Sunday
	{
		tempDR = (date->WEEKDAY) << 13;
	}

	if(date->DAY <= 31)
		tempDR += rtc_int_to_BCD(date->DAY);
	if(date->MONTH <= 12)
		tempDR += ((uint32_t)rtc_int_to_BCD(date->MONTH)) << 8;
	if(date->YEAR <= 99)
		tempDR += ((uint32_t)rtc_int_to_BCD(date->YEAR)) << 16;

	RTC->DR = tempDR;

	// Exit initialization mode if it was entered in this function
	if(reset_init != 0)
	{
		RTC->ISR &= ~(RTC_ISR_INIT);
	}
}

void rtc_set_time(RTC_TimeType* time)
{
	uint32_t tickstart;
	uint8_t reset_init = 0;

	uint32_t tempTR = 0;

	// Check if we need to enter initialization mode
	if((RTC->ISR & RTC_ISR_INITF) == 0)
	{
		reset_init = 1;
		RTC->ISR |= RTC_ISR_INIT;
		tickstart = MAIN_GetTick();
		while(((RTC->ISR) & RTC_ISR_INITF) == 0)
		{
			if((MAIN_GetTick() - tickstart) > RTC_TIMEOUT)
			{
				return;
			}
		}
	}

	if(time->SECOND <= 59)
		tempTR += rtc_int_to_BCD(time->SECOND);
	if(time->MINUTE <= 59)
			tempTR += ((uint32_t)rtc_int_to_BCD(time->MINUTE)) << 8;
	if(time->HOUR <= 23)
			tempTR += ((uint32_t)rtc_int_to_BCD(time->HOUR)) << 16;
	RTC->TR = tempTR;

	// Exit initialization mode if it was entered in this function
	if(reset_init != 0)
	{
		RTC->ISR &= ~(RTC_ISR_INIT);
	}

}

void rtc_get_date(RTC_DateType* date)
{
	date->DAY =   rtc_BCD_to_int((RTC->DR & 0x0000003F));
	date->MONTH = rtc_BCD_to_int((RTC->DR & 0x00001F00) >> 8);
	date->YEAR =  rtc_BCD_to_int((RTC->DR & 0x00FF0000) >> 16);
	date->WEEKDAY = (RTC->DR & 0x0000E000) >> 13;

}

void rtc_get_time(RTC_TimeType* time)
{
	time->SECOND = rtc_BCD_to_int((RTC->TR & 0x0000007F));
	time->MINUTE = rtc_BCD_to_int((RTC->TR & 0x00007F00) >> 8);
	time->HOUR =   rtc_BCD_to_int((RTC->TR & 0x003F0000) >> 16);
}

uint8_t rtc_int_to_BCD(uint8_t input)
{
	uint8_t outputTens = 0;
	while(input > 10)
	{
		input -= 10;
		outputTens++;
	}
	return input + (outputTens << 4);
}

uint16_t rtc_BCD_to_int(uint16_t input)
{
	return (input & 0x0F) + 10*((input&0xF0)>>4);
}
