/* graphix.h */

#ifndef _GRAPHIX_H_
#define _GRAPHIX_H_

//#include "grstring.h"

/******** DEFINES/ENUMS ***************/

#define GFX_FILLED	1
#define GFX_OUTLINE	2

#define GFX_CIRCLE_TR	0
#define GFX_CIRCLE_TL	1
#define GFX_CIRCLE_BL	2
#define GFX_CIRCLE_BR	3

/* Material Design Colors */
#define MTL_RED			0xF206
#define MTL_PINK		0xE8EC
#define MTL_PURPLE		0x9936
#define MTL_DEEP_PURPLE	0x61D6
#define MTL_INDIGO		0x3A96
#define MTL_BLUE		0x24BE
#define MTL_LIGHT_BLUE	0x055E
#define MTL_CYAN		0x05FA
#define MTL_TEAL		0x04B1
#define MTL_GREEN		0x4D6A
#define MTL_LIGHT_GREEN	0x8E09
#define MTL_LIME		0xCEE7
#define MTL_YELLOW		0xFF47
#define MTL_AMBER		0xFE00
#define MTL_ORANGE		0xFCC0
#define MTL_DEEP_ORANGE	0xFAA4
#define MTL_BROWN		0x7AA9
#define MTL_GREY		0x9CF3
#define MTL_BLUE_GREY	0x63F1
#define MTL_WHITE		0xFFFF
#define MTL_BLACK		0x0000

#define FONT_OFFSET_HEIGHT		3
#define FONT_OFFSET_WIDTHS		6
#define FONT_OFFSET_FIRSTCHAR	4
#define FONT_OFFSET_NUMCHARS	5
#define FONT_SPACE_WIDTH		5

#define TEXT_SPACING			2

#define MAX_GLYPH_WIDTH			12
#define MAX_WIDTH_HEIGHT_BYTES	4

#define IMAGE_OFFSET_XDIM		2
#define IMAGE_OFFSET_YDIM		4
#define IMAGE_OFFSET_DATA		6

/********  STRUCTURES *****************/
typedef uint8_t* gfx_FontType;

typedef struct
{
	uint16_t fgcolor; // Foreground color, used for lines and text and things
	uint16_t bgcolor; // Background color, usually for filling in blanks
	gfx_FontType font;
} gfx_SettingsType;

typedef struct
{
	uint16_t	xSize;
	uint16_t	ySize;
	uint16_t	color;
	uint16_t	cornerRadius;
	uint8_t		fill;

} gfx_BoxPropType;

typedef struct
{
	uint16_t filesize;
	uint8_t width;
    uint8_t height;
    uint8_t first_char;
    uint8_t num_chars;
}Font_Type;

/********  FUNCTIONS  *****************/
void gfx_drawFilledRectangle(uint16_t xmin, uint16_t ymin, uint16_t xmax, 
                             uint16_t ymax, uint16_t colorWord);
void gfx_cls(uint16_t colorWord);
void gfx_drawRectangle(uint16_t xmin, uint16_t ymin, uint16_t xmax, 
                             uint16_t ymax, uint16_t colorWord);
void gfx_drawQCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint8_t quadrant,  uint16_t color);
void gfx_drawCircle(uint16_t x, uint16_t y, uint16_t radius, 
                    uint16_t colorWord);
void gfx_drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius,
                          uint16_t colorWord);
void gfx_setBounds(uint16_t xmin, uint16_t ymin, uint16_t xmax, 
                             uint16_t ymax);
uint16_t gfx_color(uint8_t red, uint8_t green, uint8_t blue);
void gfx_drawHLine(uint16_t xmin, uint16_t xmax, uint16_t y, 
                   uint16_t colorWord);
void gfx_drawVLine(uint16_t x, uint16_t ymin, uint16_t ymax, 
                   uint16_t colorWord);
void gfx_drawLine(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t colorWord);
void gfx_setPixel(uint16_t x, uint16_t y, uint16_t colorWord);

void gfx_RoundedBox(gfx_BoxPropType* BOXx, uint16_t xPos, uint16_t yPos);

uint16_t gfx_fontheight_f(const uint8_t* font);
uint16_t gfx_textlength(char* string, const uint8_t* font);
uint16_t gfx_textlength_f(char* string, const uint8_t* font);
uint16_t gfx_makeColor(uint8_t red, uint8_t green, uint8_t blue);
uint8_t gfx_redPart(uint16_t colorWord);
uint8_t gfx_greenPart(uint16_t colorWord);
uint8_t gfx_bluePart(uint16_t colorWord);

uint16_t gfx_putc(uint16_t xPos, uint16_t yPos, char c, const uint8_t* font, uint16_t color);
uint16_t gfx_puts(uint16_t xPos, uint16_t yPos, char* string, const uint8_t* font, uint16_t color);

uint16_t gfx_putc_f(uint16_t xPos, uint16_t yPos, char c, const uint8_t* font, uint16_t color);
uint16_t gfx_puts_f(uint16_t xPos, uint16_t yPos, char* string, const uint8_t* font, uint16_t color);

void gfx_image_size(const uint8_t* imageName, uint16_t* xSize, uint16_t* ySize);
void gfx_image_draw(uint16_t xPos, uint16_t yPos, const uint8_t* imageName);

void d_int2str(char* string, uint32_t num, uint8_t length);
/*
void gfx_pushbutton(uint16_t xmin, uint16_t ymin, uint16_t xmax, uint16_t ymax,
                    uint16_t text_color, uint16_t bg_color, uint16_t border_color,
                    const char* pcString, tFont* pFont);
void gfx_pushbutton_flash(uint16_t xmin, uint16_t ymin, uint16_t xmax, uint16_t ymax,
                    uint16_t text_color, uint16_t bg_color, uint16_t border_color,
                    const char* pcString, uint16_t fontID);
*/

//**** VARIABLES AND SHIT  ************/
#define GFX_XMIN                (0)
#define GFX_XMAX                (319)
#define GFX_YMIN                (0)
#define GFX_YMAX                (239)

#endif // _GRAPHIX_H_
