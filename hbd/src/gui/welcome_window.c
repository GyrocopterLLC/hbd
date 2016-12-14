/*
 * welcome.c
 *
 *  Created on: Dec 13, 2016
 *      Author: David
 */
#include "stm32f0xx.h"
#include "gui_windows.h"

GUI_BUTTON welcome_button_list[WELCOME_NUM_BUTTONS];
WindowMessage_Type welcome_message;

ActiveWindow_Type createWelcomeWindow(void)
{
	welcome_message = NoChange;
	gfx_cls(MTL_WHITE);
	gui_ButtonCreate(&(welcome_button_list[WELCOME_USB_BUTTON]),WELCOME_USB_BUTTON,100,100,150,50);
	gui_ButtonSetColor(&(welcome_button_list[WELCOME_USB_BUTTON]),MTL_WHITE,MTL_DEEP_PURPLE);
	gui_ButtonSetText(&(welcome_button_list[WELCOME_USB_BUTTON]),"USB Vars",(uint8_t*)"Lato20.fnt");

	gui_ButtonCreate(&(welcome_button_list[WELCOME_FILE_BUTTON]),WELCOME_FILE_BUTTON,100,150,150,50);
	gui_ButtonSetColor(&(welcome_button_list[WELCOME_FILE_BUTTON]),MTL_WHITE,MTL_CYAN);
	gui_ButtonSetText(&(welcome_button_list[WELCOME_FILE_BUTTON]),"File Menu",(uint8_t*)"Lato20.fnt");

	gui_SetCallback(welcomeWindowCallback);
	return WelcomeWindow;
}

WindowMessage_Type updateWelcomeWindow(GUI_TOUCH* touch_pos)
{
	gui_ProcessTouch(welcome_button_list, WELCOME_NUM_BUTTONS, touch_pos);
	gui_ButtonUpdate(&welcome_button_list[WELCOME_USB_BUTTON]);
	gui_ButtonUpdate(&welcome_button_list[WELCOME_FILE_BUTTON]);
	return welcome_message;
}

void destroyWelcomeWindow(void)
{
	gui_ButtonDestroy(&(welcome_button_list[WELCOME_USB_BUTTON]));
	gui_ButtonDestroy(&(welcome_button_list[WELCOME_FILE_BUTTON]));
}

void welcomeWindowCallback(uint8_t buttonNum)
{
	if(buttonNum == WELCOME_USB_BUTTON)
	{
		// Time to move to the next window!
		welcome_message = ChangeToUSB;
	}
	if(buttonNum == WELCOME_FILE_BUTTON)
	{
		welcome_message = ChangeToFile;
	}
}
