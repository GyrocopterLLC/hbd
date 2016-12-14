#ifndef GPIO_H_
#define GPIO_H_

typedef enum
{
	GPIO_Input,
	GPIO_Output,
	GPIO_AlternateFunction,
	GPIO_Analog
}GPIO_Mode;

typedef enum
{
	GPIO_NoPull,
	GPIO_PullUp,
	GPIO_PullDown
}GPIO_PullMode;


void gpio_pinmode(GPIO_TypeDef *port, uint32_t pin, GPIO_Mode pinmode);
void gpio_af(GPIO_TypeDef *port, uint32_t pin, uint8_t gpioaf);
void gpio_pullup(GPIO_TypeDef *port, uint32_t pin, GPIO_PullMode pullmode);

#endif //GPIO_H_
