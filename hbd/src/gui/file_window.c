/*
 * file_window.c
 *
 *  Created on: Dec 13, 2016
 *      Author: David
 */

#include "stm32f0xx.h"
#include "gui_windows.h"
#include "sfs.h"


GUI_BUTTON file_button_list[FILE_NUM_BUTTONS];
WindowMessage_Type file_message;
uint8_t fileIDs[8];

static void loadFontOrImage(GUI_TOUCH* touch_pos)
{
	uint8_t tempFileName[SFS_MAX_FN];
	uint16_t x,y;

	gfx_BoxPropType gbox;
	gbox.color = MTL_PINK;
	gbox.cornerRadius = 3;
	gbox.fill = GFX_FILLED;
	gbox.xSize = 10;
	gbox.ySize = 10;

	// Find if one of the selection boxes was pressed
	if(touch_pos->state == GUI_TOUCH_PRESSED)
	{
		if(touch_pos->pos.xpos < 20)
		{
			for(uint8_t i = 0; i < 8; i++)
			{
				if(fileIDs[i] != 0)
				{
					if((touch_pos->pos.ypos > (5+i*25)) && (touch_pos->pos.ypos <= (25+i*25)))
					{
						// Load that font or image!
						gbox.color = MTL_DEEP_PURPLE;
						gfx_RoundedBox(&gbox, 3, 8+i*25);
						// Find the '.'
						gfx_drawFilledRectangle(GFX_XMAX/2,0,GFX_XMAX,GFX_YMAX/2,MTL_LIME);
						sfs_filename(fileIDs[i],tempFileName);
						for(uint8_t j = 0; j < SFS_MAX_FN; j++)
						{
							if(tempFileName[j] == '.')
							{
								if(tempFileName[j+1] == 'b')
								{
									// Draw the image!
									gfx_image_size(tempFileName,&x,&y);
									x = GFX_XMAX - x;
									y = 0;
									gfx_image_draw(x,y,tempFileName);
								}
								else if(tempFileName[j+1] == 'f')
								{
									// Show font sample!
									x = gfx_textlength_f("AaBbCc",tempFileName);
									x = GFX_XMAX - x - 5;
									y = gfx_fontheight_f(tempFileName);
									gfx_drawFilledRectangle(x,0,GFX_XMAX,y+5,MTL_WHITE);
									gfx_puts_f(x,5,"AaBbCc",tempFileName,MTL_BLACK);
								}
								break;
							}
						}
						for(uint8_t j = 0; j < 8; j++)
						{
							if((j != i) && (fileIDs[j] != 0))
							{
								gbox.color = MTL_PINK;
								gfx_RoundedBox(&gbox, 3, 8+j*25);
							}
						}
					}
				}
			}
		}
	}

}

ActiveWindow_Type createFileWindow(void)
{
	gfx_BoxPropType gbox;
	gbox.color = MTL_PINK;
	gbox.cornerRadius = 3;
	gbox.fill = GFX_FILLED;
	gbox.xSize = 10;
	gbox.ySize = 10;


	uint8_t fileNameBuf[SFS_MAX_FN];
	file_message = NoChange;
	gfx_cls(MTL_LIME);
	gui_ButtonCreate(&(file_button_list[FILE_BACK_BUTTON]),FILE_BACK_BUTTON,0,210,100,30);
	gui_ButtonSetColor(&(file_button_list[FILE_BACK_BUTTON]),MTL_WHITE,MTL_DEEP_ORANGE);
	gui_ButtonSetText(&(file_button_list[FILE_BACK_BUTTON]),"BACK",(uint8_t*)"Lato20.fnt");

	// Load file names
	uint8_t foundFiles = 0;
	for(uint8_t fID = 1; fID < 64; fID++)
	{
		if(sfs_filename(fID,fileNameBuf) == 1)
		{
			gfx_puts_f(20,5+foundFiles*25,(char*)fileNameBuf,(uint8_t*)"Lato20.fnt",MTL_BLACK);
			gfx_RoundedBox(&gbox, 3, 8+foundFiles*25);
			fileIDs[foundFiles] = fID;
			foundFiles++;
		}
	}

	gui_SetCallback(fileWindowCallback);

	return FileWindow;
}

WindowMessage_Type updateFileWindow(GUI_TOUCH* touch_pos)
{
	gui_ProcessTouch(file_button_list, FILE_NUM_BUTTONS, touch_pos);
	gui_ButtonUpdate(&file_button_list[FILE_BACK_BUTTON]);

	loadFontOrImage(touch_pos);

	return file_message;
}

void destroyFileWindow(void)
{
	gui_ButtonDestroy(&(file_button_list[FILE_BACK_BUTTON]));
}

void fileWindowCallback(uint8_t buttonNum)
{
	if(buttonNum == FILE_BACK_BUTTON)
	{
		file_message = ChangeToWelcome;
	}
}
