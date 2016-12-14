#include "stm32f0xx.h"
#include "gpio.h"


void gpio_pinmode(GPIO_TypeDef *port, uint32_t pin, GPIO_Mode pinmode)
{
	port->MODER &= ~(0x3 << (2*pin));

	switch(pinmode)
	{
	case GPIO_Output:
		gpio_pullup(port,pin,GPIO_NoPull);
		port->OSPEEDR |= (0x03 << (2*pin));
		port->MODER |= (0x01 << (2*pin));
		break;
	case GPIO_AlternateFunction:
		port->OSPEEDR |= (0x03 << (2*pin));
		port->MODER |= (0x02 << (2*pin));
		break;
	case GPIO_Analog:
		gpio_pullup(port,pin,GPIO_NoPull);
		port->MODER |= (0x03 << (2*pin));
		break;
	case GPIO_Input:
		break;
	}
}

void gpio_af(GPIO_TypeDef *port, uint32_t pin, uint8_t gpioaf)
{
	if(pin < 8)
	{
		port->AFR[1] &= ~(0x0F << (4*(pin-8)));
		port->AFR[1] |= (gpioaf << (4*(pin-8)));
	}
	else
	{
		port->AFR[0] &= ~(0x0F << (4*pin));
		port->AFR[0] |= (gpioaf << (4*pin));
	}
}

void gpio_pullup(GPIO_TypeDef *port, uint32_t pin, GPIO_PullMode pullmode)
{
	port->PUPDR &= ~(0x03 << (2*pin));

	switch(pullmode)
	{
	case GPIO_PullUp:
		port->PUPDR |= (0x01 << (2*pin));
		break;
	case GPIO_PullDown:
		port->PUPDR |= (0x02 << (2*pin));
		break;
	case GPIO_NoPull:
		break;
	}
}
