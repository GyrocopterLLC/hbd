/*
 * rtc.h
 *
 *  Created on: Aug 31, 2015
 *      Author: David
 */

#ifndef RTC_H_
#define RTC_H_

#define RTC_TIMEOUT		100

typedef struct
{
	__IO uint16_t DAY;
	__IO uint16_t MONTH;
	__IO uint16_t YEAR;
	__IO uint16_t WEEKDAY;
}RTC_DateType;

typedef struct
{
	__IO uint16_t HOUR;
	__IO uint16_t MINUTE;
	__IO uint16_t SECOND;
}RTC_TimeType;

void rtc_init(void);
void rtc_reset(RTC_DateType* initialDate, RTC_TimeType* initialTime);
void rtc_set_date(RTC_DateType* date);
void rtc_set_time(RTC_TimeType* time);
void rtc_get_date(RTC_DateType* date);
void rtc_get_time(RTC_TimeType* time);
uint8_t rtc_int_to_BCD(uint8_t input);
uint16_t rtc_BCD_to_int(uint16_t input);

#endif /* RTC_H_ */
