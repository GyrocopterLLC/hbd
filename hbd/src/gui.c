/*
 * gui.c
 *
 *  Created on: Sep 2, 2015
 *      Author: David
 */

#include "stm32f0xx.h"
#include "main.h"
#include "gui.h"
#include "touchscreen.h"
//#include "font_manager.h"

GUI_TOUCH_CALIB touch_calib = GUI_CALIB_DEFAULTS;
void (*gui_callback)(uint8_t);

/* Touch Calibration constants */
#define TOUCH_CAL_BOX_SIZE		20
#define TOUCH_CAL_LEFT			20
#define TOUCH_CAL_RIGHT			300
#define TOUCH_CAL_UP			20
#define TOUCH_CAL_DOWN			220
#define TOUCH_CAL_XCENTER		160
#define TOUCH_CAL_YCENTER		120

#define TOUCH_CAL_NUM_AVGS		10
#define TOUCH_CAL_DELAY			500 // ms

#define TOUCH_XMIN_DIV			14 // [(300-20) / 20]
#define TOUCH_YMIN_DIV			10 // [(220-20) / 20]

#define TOUCH_CAL_MARGIN		20 // Wiggle room for the final verification calibration

GUI_RESULT gui_SetCallback(void(*cb)(uint8_t))
{
	gui_callback = cb;
	return GUI_SUCCESS;
}

GUI_RESULT gui_ButtonCreate(GUI_BUTTON* btn, uint8_t id, uint16_t xp, uint16_t yp,
							uint16_t width, uint16_t height)
{
	if(btn->state & GUI_BUTTON_VISIBLE)
	{
		// Button already prepped. Should be destroyed first.
		return GUI_FAIL;
	}
	btn->btnid = id;
	btn->loc.xpos = xp;
	btn->loc.ypos = yp;
	btn->loc.xsize = width;
	btn->loc.ysize = height;
	btn->state |= GUI_BUTTON_VISIBLE | GUI_BUTTON_UPDATE;
	return GUI_SUCCESS;
}

GUI_RESULT gui_ButtonDestroy(GUI_BUTTON* btn)
{
	btn->state &= ~(GUI_BUTTON_VISIBLE);
	return GUI_SUCCESS;
}

GUI_RESULT gui_ButtonSetColor(GUI_BUTTON* btn, uint16_t fgcolor, uint16_t bgcolor)
{
	btn->fgcolor = fgcolor;
	btn->bgcolor = bgcolor;
	return GUI_SUCCESS;
}

GUI_RESULT gui_ButtonSetText(GUI_BUTTON* btn, char* textstring, const uint8_t* font)
{
	btn->text_string = textstring;
	btn->font = font;
	return GUI_SUCCESS;
}

GUI_RESULT gui_ButtonUpdate(GUI_BUTTON* btn)
{
	uint16_t xe, ye;
	uint16_t xc, yc;
	uint16_t textH, textW;
	if(btn->state & GUI_BUTTON_VISIBLE)
	{
		if(btn->state & GUI_BUTTON_UPDATE)
		{
			// Clear the update flag
			btn->state &= ~(GUI_BUTTON_UPDATE);

			uint16_t shadedColor = gfx_makeColor(gfx_redPart(btn->bgcolor)/2,
												gfx_greenPart(btn->bgcolor)/2,
												gfx_bluePart(btn->bgcolor)/2);
			xe = btn->loc.xpos + btn->loc.xsize;
			ye = btn->loc.ypos + btn->loc.ysize;
			if((btn->state & GUI_BUTTON_PRESSED) == 0)
			{
				gfx_drawFilledRectangle(btn->loc.xpos, btn->loc.ypos, xe, ye, btn->bgcolor);
			}
			else
			{
				gfx_drawFilledRectangle(btn->loc.xpos, btn->loc.ypos, xe, ye, shadedColor);
			}

			// Find the center of the box
			xc = btn->loc.xpos + (btn->loc.xsize / 2);
			yc = btn->loc.ypos + (btn->loc.ysize / 2);

			// Find the length and height of the text string
			textW = gfx_textlength_f(btn->text_string, btn->font);
			//textH = btn->font[FONT_OFFSET_HEIGHT];
			textH = gfx_fontheight_f(btn->font);

			// And the upper left corner of the text string is...
			gfx_puts_f(xc - (textW/2), yc - (textH/2), btn->text_string, btn->font, btn->fgcolor);
		}
		if(btn->state & GUI_BUTTON_CLICKED)
		{
			// Clear the clicked flag
			btn->state &= ~(GUI_BUTTON_CLICKED);
			// Trigger the callback
			if(gui_callback != ((void*)0))
				gui_callback(btn->btnid);
		}

	}
	return GUI_SUCCESS;
}

static void gui_TouchCalibox(GUI_POS* pos,uint16_t xcenter, uint16_t ycenter, uint16_t size)
{
	//GUI_POS temp_pos;
	uint32_t xsum = 0;
	uint32_t ysum = 0;
	// Draw the box

	gfx_drawRectangle(xcenter-size/2, ycenter-size/2, xcenter+size/2, ycenter+size/2, MTL_BLACK);
	gfx_drawLine(xcenter, ycenter-size/2, xcenter, ycenter+size/2, MTL_BLACK);
	gfx_drawLine(xcenter-size/2, ycenter, xcenter+size/2, ycenter, MTL_BLACK);

	// Take a few measurements
	for(uint8_t i = 0; i < TOUCH_CAL_NUM_AVGS; i++)
	{
		while( touch_get_position(pos) == 0) {}
		xsum += pos->xpos;
		ysum += pos->ypos;
	}
	// Divide and return
	pos->xpos = xsum / TOUCH_CAL_NUM_AVGS;
	pos->ypos = ysum / TOUCH_CAL_NUM_AVGS;

	// Clear the box
	gfx_drawFilledRectangle(xcenter-size/2, ycenter-size/2, xcenter+size/2, ycenter+size/2, MTL_WHITE);
}

GUI_RESULT gui_TouchCalibrate(void)
{
	GUI_POS pos1, pos2, pos3, pos4;
	uint32_t xsum;
	uint32_t ysum;
	// Draw boxes (sequentially) in each corner, take touchscreen readings for each one.
	// Draw a final box in the screen center to make sure calibration is correct.

	gfx_cls(MTL_WHITE);
	// Show some instructions.
	//gfx_puts(50, 60, "Calibrating Touchscreen", loadFont(FONT_COURIER20), CLR_BLACK);
	gfx_puts_f(50, 60, "Calibrating Touchscreen", (uint8_t*)"Courier12.fnt", MTL_BLACK);
	//gfx_puts(50, 80, "Press each box once.", loadFont(FONT_COURIER12), CLR_BLACK);
	gfx_puts_f(50, 80, "Press each box once.", (uint8_t*)"Courier12.fnt", MTL_BLACK);

	// Draw the boxes
	//160, 120

	gui_TouchCalibox(&pos1, TOUCH_CAL_LEFT, TOUCH_CAL_UP, TOUCH_CAL_BOX_SIZE);
	while(touch_is_pressed() == GUI_TOUCH_PRESSED) {}
	MAIN_Delay(TOUCH_CAL_DELAY);
	gui_TouchCalibox(&pos2, TOUCH_CAL_RIGHT, TOUCH_CAL_UP, TOUCH_CAL_BOX_SIZE);
	while(touch_is_pressed() == GUI_TOUCH_PRESSED) {}
	MAIN_Delay(TOUCH_CAL_DELAY);
	gui_TouchCalibox(&pos3, TOUCH_CAL_RIGHT, TOUCH_CAL_DOWN, TOUCH_CAL_BOX_SIZE);
	while(touch_is_pressed() == GUI_TOUCH_PRESSED) {}
	MAIN_Delay(TOUCH_CAL_DELAY);
	gui_TouchCalibox(&pos4, TOUCH_CAL_LEFT, TOUCH_CAL_DOWN, TOUCH_CAL_BOX_SIZE);
	while(touch_is_pressed() == GUI_TOUCH_PRESSED) {}
	MAIN_Delay(TOUCH_CAL_DELAY);

	// Calculate the calibration factors
	if((pos3.xpos + pos2.xpos) < (pos4.xpos + pos1.xpos))
	{
		return GUI_FAIL;
	}
	if((pos4.ypos + pos3.ypos) < (pos2.ypos + pos1.ypos))
	{
		return GUI_FAIL;
	}

	// Per-pixel x direction
	xsum = (pos3.xpos + pos2.xpos - pos4.xpos - pos1.xpos) / 2;
	// xsum is the average distance between 200 and 100 (100 pixels)

	// Calculate xmin: the value at pixel '0'
	touch_calib.xmin = (pos4.xpos + pos1.xpos)/2 - (xsum/TOUCH_XMIN_DIV);
	// xscale is 1/distance-per-pixel (left-shifted by 16)
	touch_calib.xscale = (280 << 16) / xsum;

	ysum = (pos4.ypos + pos3.ypos - pos2.ypos - pos1.ypos) / 2;
	touch_calib.ymin = (pos2.ypos + pos1.ypos)/2 - (ysum / TOUCH_YMIN_DIV);
	touch_calib.yscale = (200 << 16) / ysum;

	// One last box
	gfx_cls(MTL_WHITE);
	//gfx_puts(50,60,"Confirm calibration",loadFont(FONT_COURIER20), CLR_BLACK);
	gfx_puts_f(50,60,"Confirm calibration",(uint8_t*)"Courier20.fnt", MTL_BLACK);

	gui_TouchCalibox(&pos1, TOUCH_CAL_XCENTER, TOUCH_CAL_YCENTER, TOUCH_CAL_BOX_SIZE);
	while(touch_is_pressed() == GUI_TOUCH_PRESSED) {}
	// Apply calibrations
	xsum = ((pos1.xpos - touch_calib.xmin)*(touch_calib.xscale))>>16;
	ysum = ((pos1.ypos - touch_calib.ymin)*(touch_calib.yscale))>>16;

	if(xsum > (TOUCH_CAL_XCENTER-TOUCH_CAL_MARGIN) && xsum < (TOUCH_CAL_XCENTER+TOUCH_CAL_MARGIN))
	{
		if(ysum > (TOUCH_CAL_YCENTER-TOUCH_CAL_MARGIN) && ysum < (TOUCH_CAL_YCENTER+TOUCH_CAL_MARGIN))
			return GUI_SUCCESS;
	}

	return GUI_FAIL;
}

GUI_RESULT gui_TouchUpdate(GUI_TOUCH* tpos)
{
	uint32_t xtemp, ytemp;

	// Get raw result
	tpos->state = touch_get_position(&tpos->pos);

	if((tpos->state & GUI_TOUCH_PRESSED) && (touch_calib.xscale!=0))
	{
		// Apply calibrations
		xtemp = (tpos->pos.xpos - touch_calib.xmin)*(touch_calib.xscale);
		ytemp = (tpos->pos.ypos - touch_calib.ymin)*(touch_calib.yscale);

		tpos->pos.xpos = xtemp >> 16;
		tpos->pos.ypos = ytemp >> 16;
	}
	return GUI_SUCCESS;
}

GUI_RESULT gui_ProcessTouch(GUI_BUTTON* btnlist, uint8_t numButtons, GUI_TOUCH* tpos)
{
	for(uint8_t i = 0; i < numButtons; i++)
	{
		if(btnlist[i].state & GUI_BUTTON_VISIBLE)
		{
			if(((tpos->pos.xpos) >= (btnlist[i].loc.xpos)) &&
				((tpos->pos.ypos) >= (btnlist[i].loc.ypos)) &&
				((tpos->pos.xpos) <= (btnlist[i].loc.xpos + btnlist[i].loc.xsize)) &&
				((tpos->pos.ypos) <= (btnlist[i].loc.ypos + btnlist[i].loc.ysize)))
			{
				// Touch has occurred within the bounds of this button.
				if(tpos->state == GUI_TOUCH_PRESSED)
				{
					if((btnlist[i].state & GUI_BUTTON_PRESSED) == 0)
					{
						btnlist[i].state |= (GUI_BUTTON_PRESSED | GUI_BUTTON_UPDATE);
					}
				}
				else
				{
					// Touch was released on this button.
					if(btnlist[i].state & GUI_BUTTON_PRESSED)
					{
						// Button was pressed and now has been released
						btnlist[i].state &= ~(GUI_BUTTON_PRESSED);
						btnlist[i].state |= (GUI_BUTTON_CLICKED | GUI_BUTTON_UPDATE);
					}
				}
			}
			else
			{
				// Touch wasn't in this button's boundaries.
				if(btnlist[i].state & GUI_BUTTON_PRESSED)
				{
					// Now the button is released, but without being clicked.
					btnlist[i].state &= ~(GUI_BUTTON_PRESSED);
					btnlist[i].state |= GUI_BUTTON_UPDATE;
				}
			}
		}
	}
	return GUI_SUCCESS;
}
