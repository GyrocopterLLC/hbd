/*
 * gui.h
 *
 *  Created on: Sep 2, 2015
 *      Author: David
 */

#ifndef GUI_H_
#define GUI_H_

#include "graphix.h"
#include "ili9340_driver.h"
#include "rtc.h"
#include "comm.h"

/* Settings
 *
 */
//#define GUI_MAX_BUTTONS			8

/* Primitives
 *
 * This section contains GUI elements (aka widgets) that can be displayed to the screen.
 * Each element has its own struct type definition, all which contain a basic set of
 * parameters (position, size, color), and specific definitions for that widget.
 */

typedef enum
{
	GUI_FAIL = 0,
	GUI_SUCCESS = 1
} GUI_RESULT;

// Widget/window states
#define GUI_BUTTON_UPDATE			1
#define GUI_BUTTON_PRESSED			2
#define GUI_BUTTON_CLICKED			4
#define GUI_BUTTON_VISIBLE			8

// Touchscreen states
#define GUI_TOUCH_RELEASED		0
#define GUI_TOUCH_PRESSED		1

typedef struct
{
	uint16_t xpos;
	uint16_t ypos;
} GUI_POS;

typedef struct
{
	// Xpos and Ypos are the upper left corner of the widget
	uint16_t xpos;
	uint16_t ypos;
	// Xsize and Ysize are the width and height, respectively
	uint16_t xsize;
	uint16_t ysize;
} GUI_RECT;

typedef struct
{
	uint8_t btnid;
	uint8_t state;
	GUI_RECT loc;
	uint16_t fgcolor;
	uint16_t bgcolor;
	char* text_string;
	const uint8_t* font;
} GUI_BUTTON;

typedef struct
{
	uint8_t state;
	GUI_POS pos;
} GUI_TOUCH;

typedef struct
{
	uint16_t xmin;
	uint16_t ymin;
	uint16_t xscale;
	uint16_t yscale;
} GUI_TOUCH_CALIB;

#define GUI_CALIB_DEFAULTS		{530,700,6600,5400}

GUI_RESULT gui_SetCallback(void(*cb)(uint8_t));
GUI_RESULT gui_ButtonCreate(GUI_BUTTON* btn, uint8_t id, uint16_t xp, uint16_t yp,
							uint16_t width, uint16_t height);
GUI_RESULT gui_ButtonDestroy(GUI_BUTTON* btn);
GUI_RESULT gui_ButtonSetColor(GUI_BUTTON* btn, uint16_t fgcolor, uint16_t bgcolor);
GUI_RESULT gui_ButtonSetText(GUI_BUTTON* btn, char* textstring, const uint8_t* font);
GUI_RESULT gui_ButtonUpdate(GUI_BUTTON* btn);
GUI_RESULT gui_TouchCalibrate(void);
GUI_RESULT gui_TouchUpdate(GUI_TOUCH* tpos);
GUI_RESULT gui_ProcessTouch(GUI_BUTTON* btnlist, uint8_t numButtons, GUI_TOUCH* tpos);

#endif /* GUI_H_ */
