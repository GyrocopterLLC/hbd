/*
 * ILI9340 driver
 * 
 * Basic functions for using a 240x320 RGB LCD based
 * on the ILI9340 chipset.
 *
 * David Miller
 * May 1, 2013
 *
 */

#include "stm32f0xx.h"
#include "ili9340_driver.h"
#include "gpio.h"
 
void lcd_delay(volatile uint32_t Count)
{
	while(Count > 0)
	{
		Count--;
	}
}

void ili9340_backlight(Backlight_CommandTypeDef onoff)
{
	if(onoff == BACKLIGHT_ON)
		LCD_BKLT_PORT->BSRR = 1<<LCD_BKLT_PIN;
	else
		LCD_BKLT_PORT->BRR = 1<<LCD_BKLT_PIN;
}

void ili9340_init(void)
{
	// GPIO Port clock enables
	LOW_PORT_CLK_ENABLE();
	HIGH_PORT_CLK_ENABLE();
	// Enable the pins!
	for(uint8_t i = 0; i<= 7; i++)
	{
		gpio_pinmode(LOW_DATA_PORT,i,GPIO_Output);
	}
	gpio_pinmode(LOW_DATA_PORT,LCD_RS_PIN,GPIO_Output);
	gpio_pinmode(LOW_DATA_PORT,LCD_RD_PIN,GPIO_Output);
	gpio_pinmode(LOW_DATA_PORT,LCD_WR_PIN,GPIO_Output);
	gpio_pinmode(LOW_DATA_PORT,LCD_RESET_PIN,GPIO_Output);
	gpio_pinmode(LOW_DATA_PORT,LCD_BKLT_PIN,GPIO_Output);

	for(uint8_t i = 8; i<=15; i++)
	{
		gpio_pinmode(HIGH_DATA_PORT,i,GPIO_Output);
	}
	gpio_pinmode(HIGH_DATA_PORT,LCD_CS_PIN,GPIO_Output);

	/* HAL Version
	GPIO_InitTypeDef gi;
	LOW_PORT_CLK_ENABLE();
	HIGH_PORT_CLK_ENABLE();

	gi.Pin = LOW_DATA_PINS | LCD_RS_PIN | LCD_RD_PIN | LCD_WR_PIN | LCD_RESET_PIN | LCD_BKLT_PIN;
	gi.Mode = GPIO_MODE_OUTPUT_PP;
	gi.Pull = GPIO_NOPULL;
	gi.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(LOW_DATA_PORT, &gi);

	gi.Pin = HIGH_DATA_PINS | LCD_CS_PIN;
	HAL_GPIO_Init(HIGH_DATA_PORT, &gi);
	 */

	LCD_CS_PORT->BRR = 1<<LCD_CS_PIN;
	LCD_RD_PORT->BSRR = 1<<LCD_RD_PIN;
	LCD_WR_PORT->BRR = 1<<LCD_WR_PIN;
	LCD_RESET_PORT->BRR = 1<<LCD_RESET_PIN;
	lcd_delay(1000);
	LCD_RESET_PORT->BSRR = 1<<LCD_RESET_PIN;
	lcd_delay(1000);


	ili9340_write_command(0x0028);//display OFF
	lcd_delay(1000);
	ili9340_write_command(0x0011);
		ili9340_write_data(0x0000);				//exit SLEEP mode
	lcd_delay(1000);
	ili9340_write_command(0x00C0);
		ili9340_write_data(0x0026);
		ili9340_write_data(0x0004);	//power control 1
	ili9340_write_command(0x00C1);
		ili9340_write_data(0x0004);				//power control 2
	ili9340_write_command(0x00C5);
		ili9340_write_data(0x0034);
		ili9340_write_data(0x0040);	//VCOM control 1
	ili9340_write_command(0x0036);
		ili9340_write_data(0x0088);				//memory access control = BGR
	ili9340_write_command(0x00B1);
		ili9340_write_data(0x0000);
		ili9340_write_data(0x0018);	//frame rate control
	ili9340_write_command(0x00B6);
		ili9340_write_data(0x000A);
		ili9340_write_data(0x00A2);	//display function control
	ili9340_write_command(0x00C7);
		ili9340_write_data(0x00C0);				//VCOM control 2
	ili9340_write_command(0x003A);
		ili9340_write_data(0x0055);				//pixel format = 16 bit per pixel
	ili9340_write_command(0x00E0);
                ili9340_write_data(0x001F);				//positive gamma correction
                ili9340_write_data(0x001B);
                ili9340_write_data(0x0018);
                ili9340_write_data(0x000B);
                ili9340_write_data(0x000F);
                ili9340_write_data(0x0009);
                ili9340_write_data(0x0046);
                ili9340_write_data(0x00B5);
                ili9340_write_data(0x0037);
                ili9340_write_data(0x000A);
                ili9340_write_data(0x000C);
                ili9340_write_data(0x0007);
                ili9340_write_data(0x0007);
                ili9340_write_data(0x0005);
                ili9340_write_data(0x0000);
	ili9340_write_command(0x00E1);
                ili9340_write_data(0x0000);				//negative gamma correction
                ili9340_write_data(0x0024);
                ili9340_write_data(0x0027);
                ili9340_write_data(0x0004);
                ili9340_write_data(0x0010);
                ili9340_write_data(0x0006);
                ili9340_write_data(0x0039);
                ili9340_write_data(0x0074);
                ili9340_write_data(0x0048);
                ili9340_write_data(0x0005);
                ili9340_write_data(0x0013);
                ili9340_write_data(0x0038);
                ili9340_write_data(0x0038);
                ili9340_write_data(0x003A);
                ili9340_write_data(0x001F);
   
	ili9340_write_command(0x002A);
                ili9340_write_data(0x0000);				//column address set
               	ili9340_write_data(0x0000);				//start 0x0000
                ili9340_write_data(0x0000);
                ili9340_write_data(0x00EF);				//end 0x00EF
	ili9340_write_command(0x002B);
                ili9340_write_data(0x0000);				//page address set
                ili9340_write_data(0x0000);				//start 0x0000
                ili9340_write_data(0x0001);
                ili9340_write_data(0x003F);				//end 0x013F

	ili9340_write_command(0x0029);//display on
	//GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN);
        // Backlight on
//        GPIO_SetBits(LCD_BKLT_PORT, LCD_BKLT_PIN); // Another time, maybe.
        
        // Without th delay, the first drawing doesn't complete
        lcd_delay(100000);
}

void ili9340_write_command(uint16_t cmd)
{
  HIGH_DATA_PORT->BRR = HIGH_DATA_PINS;
  LOW_DATA_PORT->BRR = LOW_DATA_PINS | (1<<LCD_WR_PIN) | (1<<LCD_RS_PIN);
  LOW_DATA_PORT->BSRR = cmd&0x00FF;
  LCD_WR_PORT->BSRR = 1<<LCD_WR_PIN;
}

void ili9340_write_data(uint16_t data)
{
  
  HIGH_DATA_PORT->BRR = HIGH_DATA_PINS;
  LOW_DATA_PORT->BRR = LOW_DATA_PINS | (1<<LCD_WR_PIN);
  LCD_RS_PORT->BSRR = 1<<LCD_RS_PIN;
  LOW_DATA_PORT->BSRR = (data&0x00FF);
  HIGH_DATA_PORT->BSRR = (data&0xFF00);
  LCD_WR_PORT->BSRR = 1<<LCD_WR_PIN;
}


void ili9340_fillDisplay(uint16_t colorWord)
{
  uint32_t i, j;
  //GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN);
  ili9340_write_command(0x002A);
          ili9340_write_data(0x0000);				//column address set
          ili9340_write_data(0x0000);				//start 0x0000
          ili9340_write_data(0x0000);
          ili9340_write_data(0x00EF);				//end 0x00EF
  ili9340_write_command(0x002B);
          ili9340_write_data(0x0000);				//page address set
          ili9340_write_data(0x0000);				//start 0x0000
          ili9340_write_data(0x0001);
          ili9340_write_data(0x003F);				//end 0x013F
  ili9340_write_command(0x002C); // Memory write
  for(i = 0; i < 240; i++)
  {
    for(j = 0; j < 320; j++)
    {
      ili9340_write_data(colorWord);
    }
  }
}
