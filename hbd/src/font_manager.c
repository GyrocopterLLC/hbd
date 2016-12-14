
#include "stm32f0xx.h"
#include "font_manager.h"

#include "Courier12.h"
#include "Courier20.h"

const uint8_t* loadFont(uint8_t fontName)
{
	if(fontName == FONT_COURIER12)
		return Courier12;
	if(fontName == FONT_COURIER20)
		return Courier20;
	return Courier20; // Default value
}
