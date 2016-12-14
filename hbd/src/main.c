
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define MAIN_TOUCH_UPDATE			1
#define MAIN_GUI_UPDATE				2
#define MAIN_RTC_SETTIME			4

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t tickCount = 0;

TIM_HandleTypeDef app_handle;
uint32_t main_Flags = 0;
uint32_t main_Counter = 0;
uint8_t rx_buf[32];

ActiveWindow_Type main_activeWindow;

//GUI_BUTTON main_button_list[GUI_MAX_BUTTONS];

GUI_TOUCH main_touch_pos;

//MainVars_TypeDef main_vars;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void app_init(void);

/* Private functions ---------------------------------------------------------*/
static void destroyActiveWindow(ActiveWindow_Type toBeDestroyed)
{
	switch(toBeDestroyed)
	{
	case WelcomeWindow:
		destroyWelcomeWindow();
		break;
	case USBWindow:
		destroyUSBWindow();
		break;
	case FileWindow:
		destroyFileWindow();
		break;
	}
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	RTC_TimeType timeStruct;
	WindowMessage_Type main_win_change;

	/* STM32F0xx HAL library initialization:
	- Configure the Flash prefetch
	- Systick timer is configured by default as source of time base, but user
	 can eventually implement his proper time base source (a general purpose
	 timer for example or other time source), keeping in mind that Time base
	 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	 handled in milliseconds basis.
	- Low Level Initialization
	*/
	//HAL_Init();
	// Prefetch buffer enable.
	FLASH->ACR |= FLASH_ACR_PRFTBE;
	// Start systick timer.
	SysTick_Config(48000);

	/* Configure the system clock to have a system clock = 48 Mhz */
	SystemClock_Config();

	ili9340_init();
	touch_init();
	spi_init();
#ifdef COMM_USED
	comm_init();
#endif
	app_init();
	rtc_init();
	ili9340_backlight(BACKLIGHT_ON);

	// Setting default values instead.
	//gui_TouchCalibrate();
	//HAL_Delay(1000);

	main_activeWindow = createWelcomeWindow();


	/* Infinite loop */
	while (1)
	{
		if((main_Flags & MAIN_TOUCH_UPDATE) != 0)
		{
			gui_TouchUpdate(&main_touch_pos);
			main_Flags &= ~(MAIN_TOUCH_UPDATE);
		}
		if((main_Flags & MAIN_GUI_UPDATE) != 0)
		{
			main_Flags &= ~(MAIN_GUI_UPDATE);

			if(main_activeWindow == WelcomeWindow)
			{
				main_win_change = updateWelcomeWindow(&main_touch_pos);
			}
			else if(main_activeWindow == USBWindow)
			{
				main_win_change = updateUSBWindow(&main_touch_pos);

			}
			else if(main_activeWindow == FileWindow)
			{
				main_win_change = updateFileWindow(&main_touch_pos);
			}
		}
		if((main_Flags & MAIN_RTC_SETTIME) != 0)
		{
			main_Flags &= ~(MAIN_RTC_SETTIME);
			rtc_set_time(&timeStruct);
		}

		if(main_win_change == ChangeToUSB)
		{
			main_win_change = NoChange;
			destroyActiveWindow(main_activeWindow);
			main_activeWindow = createUSBWindow();

		}
		if(main_win_change == ChangeToWelcome)
		{
			main_win_change = NoChange;
			destroyActiveWindow(main_activeWindow);
			main_activeWindow = createWelcomeWindow();
		}
		if(main_win_change == ChangeToFile)
		{
			main_win_change = NoChange;
			destroyActiveWindow(main_activeWindow);
			main_activeWindow = createFileWindow();
		}
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{

	uint32_t tempreg;

	// Set HSI (8MHz) clock as PLL source
	// Turn on HSI
	RCC->CR |= RCC_CR_HSION;
	// Wait for it to be ready
	while(((RCC->CR) & RCC_CR_HSIRDY) == 0);
	// Set HSI Calibration value to default
	RCC->CR &= ~(RCC_CR_HSITRIM);
	RCC->CR |= RCC_CR_HSITRIM_1;
	// Enable PLL for 48MHz output
	// Disable PLL
	RCC->CR &= ~(RCC_CR_PLLON);
	// Set PLL source to HSI
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC);
	// Set PLL multiplier to x12 and divider to /2
	RCC->CFGR &= ~(RCC_CFGR_PLLMUL);
	RCC->CFGR |= RCC_CFGR_PLLMUL_3 | RCC_CFGR_PLLMUL_1;
	RCC->CFGR |= RCC_CFGR_PLLXTPRE;
	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	// Wait for it to be ready
	while(((RCC->CR) & RCC_CR_PLLRDY) == 0);
	// Set Flash prefetch latency to one cycle
	FLASH->ACR = (FLASH->ACR&(~FLASH_ACR_LATENCY)) | (0x01);
	// Set AHB and APD dividers to /1
	RCC->CFGR &= ~(RCC_CFGR_PPRE);
	RCC->CFGR &= ~(RCC_CFGR_HPRE);
	// Set PLL as clock source
	tempreg = RCC->CFGR;
	tempreg &= ~(RCC_CFGR_SW);
	tempreg |= RCC_CFGR_SW_1;
	RCC->CFGR = tempreg;
	// Wait for the switchover
	while(((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_1);

}

/*
static void printTime(uint16_t xpos, uint16_t ypos, RTC_TimeType* theTime, const uint8_t* font, uint16_t color)
{
	char pBuf[16];
	d_int2str(pBuf, theTime->HOUR, 2);
	xpos = gfx_puts(xpos, ypos, pBuf, font, color);
	xpos += gfx_putc(xpos, ypos, ':', font, color)+2;
	d_int2str(pBuf, theTime->MINUTE, 2);
	xpos = gfx_puts(xpos, ypos, pBuf, font, color);
	xpos += gfx_putc(xpos, ypos, ':', font, color)+2;
	d_int2str(pBuf, theTime->SECOND, 2);
	gfx_puts(xpos, ypos, pBuf, font, color);
}
*/

/**
 * Starts an application timer to serve as a 1ms timebase.
 */
static void app_init(void)
{
	APP_TIMER_CLK_ENABLE();

	APP_TIMER->ARR = APP_PERIOD;
	APP_TIMER->PSC = 0;
	APP_TIMER->CR1 = 0;

	NVIC_SetPriority(TIM6_DAC_IRQn,3);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);

	APP_TIMER->DIER |= TIM_DIER_UIE;
	APP_TIMER->CR1 |= TIM_CR1_CEN;
}

/**
 * Implements a simple scheduler to call functions as needed.
 * Basic timer is used to generate a timebase. This function
 * is called from the timer interrupt. The scheduler does
 * not call sub-functions, this occurs in the main loop. Instead,
 * this scheduler queues a function call by setting a flag in
 * the main "status" variable.
 */
void MAIN_AppScheduler(void)
{
	main_Counter++;
	if((main_Counter % 10) == 0)
	{
		// 10 millisecond action
		// Get the touchscreen position
		//touch_get_position(&main_touch_pos);

		main_Flags |= MAIN_TOUCH_UPDATE;
	}
	if((main_Counter % 100) == 0)
	{
		// 100 millisecond action
		main_Flags |= MAIN_GUI_UPDATE;
	}
	if((main_Counter % 1000) == 0)
	{
		// Every 1 sec

		main_Counter = 0;
	}
}

void MAIN_IncTick(void)
{
	tickCount++;
}
uint32_t MAIN_GetTick(void)
{
	return tickCount;
}

void MAIN_Delay(uint32_t delayms)
{
	  uint32_t tickstart = tickCount;
	  while((tickCount - tickstart) < delayms)
	  {
	  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

