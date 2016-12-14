/* ILI9340_driver.h */

#ifndef _ILI9340_H_
#define _ILI9340_H_


/*******  PORTS AND PINS  *************/
#define LOW_DATA_PINS	(0x00FF) // Pins 0-7
#define LOW_DATA_PORT	(GPIOA)
#define HIGH_DATA_PINS	(0xFF00) // Pins 8-15
#define HIGH_DATA_PORT	(GPIOB)
#define LCD_RS_PIN		9//(0x0200)//(GPIO_PIN_9)
#define LCD_RS_PORT		(GPIOA)
#define LCD_WR_PIN		10//(0x0400)//(GPIO_PIN_10)
#define LCD_WR_PORT		(GPIOA)
#define LCD_RD_PIN		11//(0x0800)//(GPIO_PIN_11)
#define LCD_RD_PORT		(GPIOA)
#define LCD_BKLT_PIN    8//(0x0100)//(GPIO_PIN_8)
#define LCD_BKLT_PORT   (GPIOA)
#define LCD_RESET_PIN	12//(0x1000)//(GPIO_PIN_12)
#define LCD_RESET_PORT	(GPIOA)
#define LCD_CS_PIN		2//(0x0004)//(GPIO_PIN_2)
#define LCD_CS_PORT		(GPIOB)

#define LOW_PORT_CLK_ENABLE()	SET_BIT(RCC->AHBENR,RCC_AHBENR_GPIOAEN)
#define HIGH_PORT_CLK_ENABLE()	SET_BIT(RCC->AHBENR,RCC_AHBENR_GPIOBEN)

/****  COMMON LCD COMMANDS*************/
#define SET_COL_ADDRESS         (0x002A)
#define SET_ROW_ADDRESS         (0x002B)
#define WRITE_DATA              (0x002C)

typedef enum
{
	BACKLIGHT_OFF = 0,
	BACKLIGHT_ON = 1
}Backlight_CommandTypeDef;

/********  FUNCTIONS  *****************/
//void ili9340_pinsetup(void);
void ili9340_backlight(Backlight_CommandTypeDef onoff);
void ili9340_write_command(uint16_t cmd);
void ili9340_write_data(uint16_t data);
void ili9340_init(void);
void ili9340_fillDisplay(uint16_t colorWord);

#endif // _ILI9340_H_
