/*
 * usb_window.c
 *
 *  Created on: Dec 13, 2016
 *      Author: David
 */

#include "stm32f0xx.h"
#include "gui_windows.h"
#include "comm.h"

#define NUM_USB_VARIABLES	10
char* usb_variables[NUM_USB_VARIABLES] = {"Ia","Ib","Ic","Ta","Tb","Tc","Throttle","Ramp","HallAngle","HallSpeed"};
uint8_t usb_variable_selections[NUM_USB_VARIABLES];

GUI_BUTTON usb_button_list[USB_NUM_BUTTONS];
WindowMessage_Type usb_message;

static void usbWindowCheckVariablePress(GUI_TOUCH* touch_pos);

ActiveWindow_Type createUSBWindow(void)
{
	usb_message = NoChange;
	gui_ButtonDestroy(&(usb_button_list[USB_BACK_BUTTON]));
	gui_ButtonDestroy(&(usb_button_list[USB_SUBMIT_BUTTON]));
	gui_ButtonDestroy(&(usb_button_list[USB_RESET_BUTTON]));

	gfx_cls(MTL_WHITE);

	gfx_puts_f(30,5,"Choose up to 5 variables.",(uint8_t*)"Lato20.fnt",MTL_BLACK);

	for(uint8_t i=0; i < NUM_USB_VARIABLES; i++)
	{
		usb_variable_selections[i] = 0;

		if(i<5)
		{
			gfx_drawRectangle(0,25+(i%5)*25,109,49+(i%5)*25,MTL_BLACK);
			gfx_drawRectangle(110,25+(i%5)*25,129,49+(i%5)*25,MTL_BLACK);
			gfx_puts_f(5,30+(i%5)*25,usb_variables[i],(uint8_t*)"Lato20.fnt",MTL_BLACK);
		}
		else
		{
			gfx_drawRectangle(130,25+(i%5)*25,239,49+(i%5)*25,MTL_BLACK);
			gfx_drawRectangle(240,25+(i%5)*25,259,49+(i%5)*25,MTL_BLACK);
			gfx_puts_f(135,30+(i%5)*25,usb_variables[i],(uint8_t*)"Lato20.fnt",MTL_BLACK);
		}

	}

	gui_ButtonCreate(&(usb_button_list[USB_BACK_BUTTON]),USB_BACK_BUTTON,0,210,105,30);
	gui_ButtonSetColor(&(usb_button_list[USB_BACK_BUTTON]),MTL_WHITE,MTL_RED);
	gui_ButtonSetText(&(usb_button_list[USB_BACK_BUTTON]),"BACK",(uint8_t*)"Lato20.fnt");

	gui_ButtonCreate(&(usb_button_list[USB_SUBMIT_BUTTON]),USB_SUBMIT_BUTTON,105,210,110,30);
	gui_ButtonSetColor(&(usb_button_list[USB_SUBMIT_BUTTON]),MTL_WHITE,MTL_GREEN);
	gui_ButtonSetText(&(usb_button_list[USB_SUBMIT_BUTTON]),"SUBMIT",(uint8_t*)"Lato20.fnt");

	gui_ButtonCreate(&(usb_button_list[USB_RESET_BUTTON]),USB_RESET_BUTTON,215,210,105,30);
	gui_ButtonSetColor(&(usb_button_list[USB_RESET_BUTTON]),MTL_WHITE,MTL_PINK);
	gui_ButtonSetText(&(usb_button_list[USB_RESET_BUTTON]),"RESET",(uint8_t*)"Lato20.fnt");

	gui_SetCallback(usbWindowCallback);
	return USBWindow;
}

WindowMessage_Type updateUSBWindow(GUI_TOUCH* touch_pos)
{
	gui_ProcessTouch(usb_button_list, USB_NUM_BUTTONS, touch_pos);
	gui_ButtonUpdate(&usb_button_list[USB_BACK_BUTTON]);
	gui_ButtonUpdate(&usb_button_list[USB_SUBMIT_BUTTON]);
	gui_ButtonUpdate(&usb_button_list[USB_RESET_BUTTON]);
	usbWindowCheckVariablePress(touch_pos);

	return usb_message;
}

void destroyUSBWindow(void)
{
	gui_ButtonDestroy(&(usb_button_list[USB_BACK_BUTTON]));
	gui_ButtonDestroy(&(usb_button_list[USB_SUBMIT_BUTTON]));
	gui_ButtonDestroy(&(usb_button_list[USB_RESET_BUTTON]));
}

void usbWindowCallback(uint8_t buttonNum)
{
	uint8_t sendBuf[8];
	if(buttonNum == USB_BACK_BUTTON)
	{
		// Time to move to the next window!
		usb_message = ChangeToWelcome;
	}
	if(buttonNum == USB_SUBMIT_BUTTON)
	{
		gfx_puts_f(5,160,"Submitted!",(uint8_t*)"Lato20.fnt",MTL_BLACK);
		for(uint8_t i = 0; i < NUM_USB_VARIABLES; i++)
		{
			if(usb_variable_selections[i] != 0)
			{
				sendBuf[0] = 'U';
				sendBuf[1] = '0' + usb_variable_selections[i];
				sendBuf[2] = '0' + i;
				sendBuf[3] = 0x0D; // \r
				sendBuf[4] = 0x0A; // \n
				comm_txmt(sendBuf,5);
			}
		}
	}
	if(buttonNum == USB_RESET_BUTTON)
	{
		for(uint8_t i = 0; i<NUM_USB_VARIABLES;i++)
		{
			gfx_drawFilledRectangle(111+(i/5)*130,26+(i%5)*25,128+(i/5)*130,48+(i%5)*25,MTL_WHITE);
			usb_variable_selections[i] = 0;
		}
	}
}

static void usbWindowCheckVariablePress(GUI_TOUCH* touch_pos)
{
	uint8_t row=0, col=0;
	uint8_t xpos, ypos;
	uint8_t numstr[2];
	numstr[1] = 0;

	if(touch_pos->state == GUI_TOUCH_PRESSED)
	{
		xpos = (touch_pos->pos).xpos;
		ypos = (touch_pos->pos).ypos;
		// Find which row we're in
		if(ypos >= 25 && ypos < 50)
			row = 1;
		else if(ypos >= 50 && ypos < 75)
			row = 2;
		else if(ypos >= 75 && ypos < 100)
			row = 3;
		else if(ypos >= 100 && ypos < 125)
			row = 4;
		else if(ypos >= 125 && ypos < 150)
			row = 5;
		// Are we in the first column?
		if(xpos >= 0 && xpos < 109)
			col = 1;
		else if(xpos >= 130 && xpos < 239)
			col = 2;

		if((row!=0) && (col!=0))
		{
			// We picked something!
			uint8_t selection_num = row + (col-1)*5 - 1;
			if(usb_variable_selections[selection_num] == 0)
			{
				// Check the biggest number so far
				uint8_t max = 0;
				for(uint8_t i=0; i<NUM_USB_VARIABLES;i++)
				{
					if(usb_variable_selections[i] > max)
						max = usb_variable_selections[i];
				}
				if(max < 5)
				{
					usb_variable_selections[selection_num] = max+1;
					numstr[0] = '0'+max+1;
					gfx_puts_f(115 + (selection_num/5)*130, 30+(selection_num%5)*25,numstr,(uint8_t*)"Lato20.fnt",MTL_BLACK);
				}
			}
		}
	}
}
