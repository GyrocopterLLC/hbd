#ifndef GUI_WINDOWS_H_
#define GUI_WINDOWS_H_

#include "graphix.h"
#include "gui.h"

#define WELCOME_NUM_BUTTONS			2
#define WELCOME_USB_BUTTON			0
#define WELCOME_FILE_BUTTON			1

#define USB_NUM_BUTTONS				3
#define USB_BACK_BUTTON				0
#define USB_SUBMIT_BUTTON			1
#define USB_RESET_BUTTON			2

#define FILE_NUM_BUTTONS			1
#define FILE_BACK_BUTTON			0

typedef enum
{
	WelcomeWindow,
	USBWindow,
	FileWindow
} ActiveWindow_Type;

typedef enum
{
	NoChange,
	ChangeToUSB,
	ChangeToWelcome,
	ChangeToFile
} WindowMessage_Type;

ActiveWindow_Type createWelcomeWindow(void);
WindowMessage_Type updateWelcomeWindow(GUI_TOUCH* touch_pos);
void destroyWelcomeWindow(void);
void welcomeWindowCallback(uint8_t buttonNum);

ActiveWindow_Type createUSBWindow(void);
WindowMessage_Type updateUSBWindow(GUI_TOUCH* touch_pos);
void destroyUSBWindow(void);
void usbWindowCallback(uint8_t buttonNum);

ActiveWindow_Type createFileWindow(void);
WindowMessage_Type updateFileWindow(GUI_TOUCH* touch_pos);
void destroyFileWindow(void);
void fileWindowCallback(uint8_t buttonNum);

#endif // GUI_WINDOWS_H_
