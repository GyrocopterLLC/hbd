/*
 * Graphics Library
 * 
 * Basic graphics functions and primitives
 *
 * David Miller
 * May 4, 2013
 *
 */
 
//#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "ili9340_driver.h"
//#include "grstring.h"
#include "graphix.h"
#include "sfs.h"
#include "Courier12.h"


void gfx_setBounds(uint16_t xmin, uint16_t ymin, uint16_t xmax, 
                             uint16_t ymax)
{
  // check inputs
  if(xmax > GFX_XMAX) xmax = GFX_XMAX;
  if(ymax > GFX_YMAX) ymax = GFX_YMAX;
  // Minimum is zero, but since the inputs are unsigned they can't be less than zero
  // Still need to check their bounds though (in case of wraparound)
  if(ymin > GFX_YMAX) ymin = 0;
  if(xmin > GFX_XMAX) xmin = 0;
  
  ili9340_write_command(SET_COL_ADDRESS);
    ili9340_write_data((ymin&0xFF00)>>8);
    ili9340_write_data(ymin&0x00FF);
    ili9340_write_data((ymax&0xFF00)>>8);
    ili9340_write_data(ymax&0x00FF);
  ili9340_write_command(SET_ROW_ADDRESS);
    ili9340_write_data((xmin&0xFF00)>>8);
    ili9340_write_data(xmin&0x00FF);
    ili9340_write_data((xmax&0xFF00)>>8);
    ili9340_write_data(xmax&0x00FF);

}

void gfx_drawFilledRectangle(uint16_t xmin, uint16_t ymin, uint16_t xmax, 
                             uint16_t ymax, uint16_t colorWord)
{
  uint16_t i,j;
  gfx_setBounds(xmin, ymin, xmax, ymax);
  ili9340_write_command(WRITE_DATA);
  for(i=xmin; i<=xmax; i++)
  {
    for(j=ymin; j<=ymax; j++)
    {
      ili9340_write_data(colorWord);
    }
  }
}

void gfx_cls(uint16_t colorWord)
{
	gfx_drawFilledRectangle(GFX_XMIN, GFX_YMIN, GFX_XMAX, GFX_YMAX, colorWord);
}

void gfx_drawHLine(uint16_t x, uint16_t y, uint16_t xlen, uint16_t colorWord)
{
  uint16_t i;
  gfx_setBounds(x, y, x+xlen-1, y);
  ili9340_write_command(WRITE_DATA);
  for(i=0; i<xlen; i++)
  {
    ili9340_write_data(colorWord);
  }
}

void gfx_drawVLine(uint16_t x, uint16_t y, uint16_t ylen, uint16_t colorWord)
{
  uint16_t i;
  gfx_setBounds(x, y, x, y+ylen-1);
  ili9340_write_command(WRITE_DATA);
  for(i=0; i<ylen; i++)
  {
    ili9340_write_data(colorWord);
  }
}

void gfx_drawLine(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t colorWord)
{
	if(xe == xs)
	{
		gfx_drawVLine(xs, ys, ye-ys+1, colorWord);
	}
	else if(ye == ys)
	{
		gfx_drawHLine(xs, ys, xe-xs+1, colorWord);
	}
	else
	{
		int16_t dx, dy;
		int16_t sx, sy;
		int16_t err, e2;
		if(xe > xs)
		{
			dx = xe - xs;
			sx = 1;
		}
		else
		{
			dx = xs - xe;
			sx = -1;
		}
		if(ye > ys)
		{
			dy = ye - ys;
			sy = 1;
		}
		else
		{
			dy = ys - ye;
			sy = -1;
		}
		err = (dx > dy) ? dx : -dy/2;

		for(;;)
		{
			gfx_setPixel(xs, ys, colorWord);//setPixel(x0,y0);
			if(xs == xe && ys == ye) break;//if (x0==x1 && y0==y1) break;
			e2 = err;
			if (e2 >-dx) { err -= dy; xs += sx; }
			if (e2 < dy) { err += dx; ys += sy; }
		}
	}
}
  
void gfx_setPixel(uint16_t x, uint16_t y, uint16_t colorWord)
{
  if(x > GFX_XMAX || y > GFX_YMAX)
    return;

  gfx_setBounds(x, y, x, y);
  ili9340_write_command(WRITE_DATA);
  ili9340_write_data(colorWord);
}

void gfx_drawRectangle(uint16_t xmin, uint16_t ymin, uint16_t xmax, 
                             uint16_t ymax, uint16_t colorWord)
{

	gfx_drawHLine(xmin, ymin, xmax-xmin+1, colorWord);
	gfx_drawHLine(xmin, ymax, xmax-xmin+1, colorWord);
	gfx_drawVLine(xmin, ymin+1, ymax-ymin-1, colorWord);
	gfx_drawVLine(xmax, ymin+1, ymax-ymin-1, colorWord);
}

void gfx_drawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t colorWord)
{
  // Bresenham variant, thanks TI/LuminaryMicro!
  int32_t a, b, d;
  a = 0;
  b = radius;
  d = 3-(2*radius);
  
  while(a<=b)
  {
    gfx_setPixel(x-b,y-a,colorWord);
    gfx_setPixel(x+b,y-a,colorWord);
    if(a != 0)
    {
      gfx_setPixel(x-b,y+a,colorWord);
      gfx_setPixel(x+b,y+a,colorWord);
    }
    if(a!=b)
    {
      gfx_setPixel(x-a,y-b,colorWord);
      if(a!=0)
      {
        gfx_setPixel(x+a,y-b,colorWord);
      }
      gfx_setPixel(x-a,y+b,colorWord);
      if(a!=0)
      {
        gfx_setPixel(x+a,y+b,colorWord);
      }
    }
    if(d<0)
    {
      d+= 4*a+6;
    }
    else
    {
      d+= 4*(a-b)+10;
      b-=1;
    }
    a++;
  }
}

void gfx_drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius,
                          uint16_t colorWord)
{
  // Again, thanks to TI and LuminaryMicro for some example code
  int32_t a, b, d;
  
  a=0;
  b=radius;
  d = 3-(2*radius);
  while(a<=b)
  {
	  gfx_drawHLine(x-b,y-a,2*b+1,colorWord);
    if(a!=0)
    {
    	gfx_drawHLine(x-b,y+a,2*b+1,colorWord);
    }
    if((d>=0) && (a!=b))
    {
      gfx_drawHLine(x-a,y-b,2*a+1,colorWord);
      gfx_drawHLine(x-a,y+b,2*a+1,colorWord);
    }
    if(d<0)
    {
      d+=4*a+6;
    }
    else
    {
      d+=4*(a-b)+10;
      b-=1;
    }
    a++;
  }
}

#if 0
void gfx_pushbutton(uint16_t xmin, uint16_t ymin, uint16_t xmax, uint16_t ymax,
                    uint16_t text_color, uint16_t bg_color, uint16_t border_color,
                    const char* pcString, tFont* pFont)
{
  gfx_drawRectangle(xmin, ymin, xmax, ymax, border_color);
  gfx_drawFilledRectangle(xmin+1, ymin+1, xmax-1, ymax-1, bg_color);
  grstring_drawCentered(pcString, -1, (xmin+xmax)/2, (ymin+ymax)/2, text_color,
                        pFont);
}

void gfx_pushbutton_flash(uint16_t xmin, uint16_t ymin, uint16_t xmax, uint16_t ymax,
                    uint16_t text_color, uint16_t bg_color, uint16_t border_color,
                    const char* pcString, uint16_t fontID)
{
  gfx_drawRectangle(xmin, ymin, xmax, ymax, border_color);
  gfx_drawFilledRectangle(xmin+1, ymin+1, xmax-1, ymax-1, bg_color);
  grstring_drawCentered_flash(pcString, -1, (xmin+xmax)/2, (ymin+ymax)/2, text_color, fontID);
}
#endif

uint16_t gfx_color(uint8_t red, uint8_t green, uint8_t blue)
{
  uint16_t temp;
  // Red is only 5 bits
  red = red >> 3;
  // Green is 6
  green = green >> 2;
  // And blue is also 5
  blue = blue >> 3;
  
  temp = red << 11;
  temp += green << 5;
  temp += blue;
  return temp;
}

void gfx_RoundedBox(gfx_BoxPropType* BOXx, uint16_t xPos, uint16_t yPos)
{
	// Fill the top and bottom between the quarter circles
	// then fill the remaining rectangle in the middle
	// LIKE A HAMBURGER
	// Below: The "T" and "B" sections are the buns
	// 		  and the "M" section is the juicy ground beef
	//
	// ____TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT____
	// __TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT__
	// _TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT_
	// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	// MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
	// BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
	// _BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB_
	// __BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB__
	// ____BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB____

	// But if you asked for GUI_OUTLINE, you just get the ghost of burger past
	//
	// ____TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT____
	// __T__________________________________________T__
	// _T____________________________________________T_
	// T______________________________________________T
	// M______________________________________________M
	// M______________________________________________M
	// M______________________________________________M
	// M______________________________________________M
	// M______________________________________________M
	// B______________________________________________B
	// _B____________________________________________B_
	// __B__________________________________________B__
	// ____BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB____

	if(BOXx->fill == GFX_FILLED)
	{
		int16_t a, b, d;
		uint16_t xL, xR, yT, yB;
		a = 0;
		b = BOXx->cornerRadius;
		d = 3-(2*BOXx->cornerRadius);
		xL = xPos + BOXx->cornerRadius;
		xR = xPos + BOXx->xSize - BOXx->cornerRadius;
		yT = yPos + BOXx->cornerRadius;
		yB = yPos + BOXx->ySize - BOXx->cornerRadius;

		while(a <= b)
		{
			gfx_drawHLine(xL-b,yT-a,(xR-xL+2*b), BOXx->color);
			gfx_drawHLine(xL-b,yB+a,(xR-xL+2*b),BOXx->color);
			if(a!=b)
			{
				gfx_drawHLine(xL-a,yT-b, (xR-xL+2*a), BOXx->color);
				gfx_drawHLine(xL-a,yB+b, (xR-xL+2*a), BOXx->color);
			}
			if(d<0)
			{
				d+= 4*a+6;
			}
			else
			{
				d+= 4*(a-b)+10;
				b--;
			}
			a++;
		}

		// Now the top and bottom parts are done, fill the middle rectangle
		gfx_drawFilledRectangle(xPos, yT + 1, BOXx->xSize + xPos - 1, yB - 1, BOXx->color);
	}
	else if(BOXx->fill == GFX_OUTLINE)
	{
		// Just the outlines of the rectangles
		gfx_drawHLine(xPos + BOXx->cornerRadius, yPos, BOXx->xSize - 2*(BOXx->cornerRadius), BOXx->color);
		gfx_drawHLine(xPos + BOXx->cornerRadius, yPos + BOXx->ySize - 1, BOXx->xSize - 2*(BOXx->cornerRadius), BOXx->color);
		gfx_drawVLine(xPos, yPos + BOXx->cornerRadius, BOXx->ySize - 2*(BOXx->cornerRadius), BOXx->color);
		gfx_drawVLine(xPos + BOXx->xSize - 1, yPos + BOXx->cornerRadius, BOXx->ySize - 2*(BOXx->cornerRadius), BOXx->color);

		// And the outlines of quarter circles
		gfx_drawQCircle(xPos + BOXx->cornerRadius - 1, yPos + BOXx->cornerRadius - 1, BOXx->cornerRadius, GFX_CIRCLE_TL, BOXx->color);
		gfx_drawQCircle(xPos + BOXx->xSize - BOXx->cornerRadius, yPos + BOXx->cornerRadius - 1, BOXx->cornerRadius, GFX_CIRCLE_TR, BOXx->color);
		gfx_drawQCircle(xPos + BOXx->cornerRadius - 1, yPos + BOXx->ySize - BOXx->cornerRadius, BOXx->cornerRadius, GFX_CIRCLE_BL, BOXx->color);
		gfx_drawQCircle(xPos + BOXx->xSize - BOXx->cornerRadius, yPos + BOXx->ySize - BOXx->cornerRadius, BOXx->cornerRadius, GFX_CIRCLE_BR, BOXx->color);
	}
}

void gfx_drawQCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint8_t quadrant, uint16_t color)
{
	int16_t a, b, d;
	a = 0;
	b = radius;
	d = 3-(2*radius);

	while(a <= b)
	{
		if(quadrant == GFX_CIRCLE_TR)
		{
			gfx_setPixel(x0+b,y0-a, color);
			gfx_setPixel(x0+a,y0-b, color);
		}
		else if(quadrant == GFX_CIRCLE_TL)
		{
			gfx_setPixel(x0-b,y0-a, color);
			gfx_setPixel(x0-a,y0-b, color);
		}
		else if(quadrant == GFX_CIRCLE_BL)
		{
			gfx_setPixel(x0-b,y0+a, color);
			gfx_setPixel(x0-a,y0+b, color);
		}
		else if(quadrant == GFX_CIRCLE_BR)
		{
			gfx_setPixel(x0+b,y0+a, color);
			gfx_setPixel(x0+a,y0+b, color);
		}
		if(d<0)
		{
			d+= 4*a+6;
		}
		else
		{
			d+= 4*(a-b)+10;
			b--;
		}
		a++;
	}
}

uint16_t gfx_textlength(char* string, const uint8_t* font)
{
	uint16_t str_width = 0;
	char c;
	// Iterate through the string, sum up the widths
	while(*string != 0)
	{
		c = *string;
		if(c == (char)0x20)
		{
			// Spaces are fixed number of pixels.
			str_width += FONT_SPACE_WIDTH;
		}
		else
		{
			if((c < font[FONT_OFFSET_FIRSTCHAR]) || (c > (font[FONT_OFFSET_FIRSTCHAR] + font[FONT_OFFSET_NUMCHARS])))
			{
				// A period will be printed for this non-printable character
				c = '.';
			}
			str_width += font[FONT_OFFSET_WIDTHS + (uint32_t)c - font[FONT_OFFSET_FIRSTCHAR]];
		}
		string++;
	}

	return str_width;
}

uint16_t gfx_textlength_f(char* string, const uint8_t* font)
{
	Font_Type f;
	uint16_t str_width = 0;
	uint8_t temp;
	char c;

	if(sfs_filesize(font) == 0)
	{
		// Use default font instead
		return gfx_textlength(string, Courier12);
	}

	sfs_read(font,(uint8_t*)&f,0,sizeof(Font_Type));
	// Iterate through the string, summing up the widths
	while(*string != 0)
	{
		c = *string;
		if(c == (char)0x20)
		{
			str_width += FONT_SPACE_WIDTH;
		}
		else
		{
			if((c < f.first_char) || (c > (f.first_char + f.num_chars)))
			{
				c = '.';
			}
			sfs_read(font,&temp,FONT_OFFSET_WIDTHS + (uint32_t)c - f.first_char, 1);
			str_width += temp + TEXT_SPACING;
		}
		string++;
	}
	return str_width - ((str_width > TEXT_SPACING) ? TEXT_SPACING : 0);
}

uint16_t gfx_makeColor(uint8_t red, uint8_t green, uint8_t blue)
{
	uint16_t temp;
	temp = ((uint16_t)(red & 0xF8)) << 8;
	temp += ((uint16_t)(green & 0xFC)) << 3;
	temp += ((uint16_t)(blue & 0xF8)) >> 3;
	return temp;
}

uint8_t gfx_redPart(uint16_t colorWord)
{
	return (colorWord & 0xF800) >> 8;
}
uint8_t gfx_greenPart(uint16_t colorWord)
{
	return (colorWord & 0x07E0) >> 3;
}
uint8_t gfx_bluePart(uint16_t colorWord)
{
	return (colorWord & 0x001F) << 3;
}

uint16_t gfx_putc(uint16_t xPos, uint16_t yPos, char c, const uint8_t* font, uint16_t color)
{
	uint16_t font_height;
	uint16_t bytes_per_col;
	uint16_t font_width;
	uint32_t font_start;
	uint16_t i,j,k;

	// Default to a 5-pixel space...
	if(c == (char)0x20)
		return FONT_SPACE_WIDTH;

	// Make sure it's a printable character
	if((c < font[FONT_OFFSET_FIRSTCHAR]) || (c > (font[FONT_OFFSET_FIRSTCHAR] + font[FONT_OFFSET_NUMCHARS])))
	{
		// Change to a printable character. Period.
		c = '.';
	}

	// Get some info about this here font
	font_height = font[FONT_OFFSET_HEIGHT];
	// Number of bytes required for a full column of data
	bytes_per_col = font_height / 8 + 1;

	// And now, the width of our character in question
	font_width = font[FONT_OFFSET_WIDTHS + (uint32_t)c - font[FONT_OFFSET_FIRSTCHAR]];

	// Fantastic, and finally let's find the position of the font data
	// Have to sum up all the widths before this font
	font_start = 0;
	for(i=0; i<(uint32_t)c - font[FONT_OFFSET_FIRSTCHAR]; i++)
	//for(i=font[FONT_OFFSET_FIRSTCHAR]; i<(uint32_t)c; i++)
	{
		font_start += font[FONT_OFFSET_WIDTHS + i];
	}
	font_start *= bytes_per_col;
	font_start += FONT_OFFSET_WIDTHS + font[FONT_OFFSET_NUMCHARS];
	// Should be enough. Let's draw the character now!
	// Draws left to right, then goes down a row or more (if character height is bigger than a byte [8])
	uint16_t current_byte;
	uint16_t bits_to_draw;
	for(i=0;i<bytes_per_col;i++)
	{
		for(j=0;j<font_width;j++)
		{
			current_byte = font[font_start + j + font_width*i];
			if(i == bytes_per_col - 1)
			{
				// Check how many bits of the last byte to draw
				bits_to_draw = font_height % 8;
				if(bits_to_draw == 0) bits_to_draw = 8; // In case height is a multiple of 8
			}
			else
			{
				bits_to_draw = 8;
			}
			for(k=0;k<bits_to_draw;k++)
			{
				if(current_byte & 0x80)
				{
					gfx_setPixel(xPos + j, yPos + (bits_to_draw - 1 - k) + 8*i, color);
				}
				current_byte = current_byte << 1;
			}
		}
	}

	return font_width;
}

uint16_t gfx_putc_f(uint16_t xPos, uint16_t yPos, char c, const uint8_t* font, uint16_t color)
{
	Font_Type f;
	uint8_t bytes_per_col;
	uint8_t glyph_width;
	uint32_t glyph_start;
	uint8_t temp;
	uint8_t glyph_buffer[MAX_GLYPH_WIDTH*MAX_WIDTH_HEIGHT_BYTES];

	// If it's a space, simply move the cursor over by the space distance
	if(c == (char)0x20)
		return FONT_SPACE_WIDTH;

	// Load font info
	sfs_read(font, (uint8_t*)&f, 0, sizeof(Font_Type));

	// Replace unprintable characters
	if((c < f.first_char) || (c > (f.first_char + f.num_chars)))
	{
		c = '.';
	}

	// Get font info
	bytes_per_col = (f.height / 8) + 1;
	// Load this glyph's width
	sfs_read(font, &glyph_width, FONT_OFFSET_WIDTHS + c - f.first_char, 1);
	// Find where this glyph starts
	glyph_start = 0;
	for(uint8_t i = 0; i < (c - f.first_char); i++)
	{
		sfs_read(font,&temp, FONT_OFFSET_WIDTHS + i, 1);
		glyph_start += temp;
	}
	// Load the glyph buffer
	sfs_read(font, (uint8_t*)&glyph_buffer, FONT_OFFSET_WIDTHS + f.num_chars + glyph_start*bytes_per_col,
			bytes_per_col*glyph_width);

	uint8_t current_byte;
	uint8_t bits_to_draw;
	for(uint8_t i = 0; i < bytes_per_col; i++)
	{
		for(uint8_t j = 0; j < glyph_width; j++)
		{
			if(i == (bytes_per_col - 1))
			{
				bits_to_draw = f.height % 8;
				if(bits_to_draw == 0) bits_to_draw = 8; // If height is a multiple of 8
				bits_to_draw = (bits_to_draw == 0 ? 8 : bits_to_draw);
			}
			else
				bits_to_draw = 8;

			current_byte = glyph_buffer[i*glyph_width + j];
			for(uint8_t k=0;k<bits_to_draw;k++)
			{
				if(current_byte & 0x80)
				{
					gfx_setPixel(xPos + j, yPos + (bits_to_draw - 1 - k) + 8*i, color);
				}
				current_byte = current_byte << 1;
			}
		}
	}

	return glyph_width;
}

uint16_t gfx_puts(uint16_t xPos, uint16_t yPos, char* string, const uint8_t* font, uint16_t color)
{
	while(*string != 0)
	{
		xPos += gfx_putc(xPos, yPos, *string, font, color) + TEXT_SPACING;
		string++;
	}
	return xPos;
}

uint16_t gfx_puts_f(uint16_t xPos, uint16_t yPos, char* string, const uint8_t* font, uint16_t color)
{
	if(sfs_filesize(font) == 0)
	{
		// Oops, no font!
		// Use default font instead
		return gfx_puts(xPos,yPos,string,Courier12,color);
	}
	while(*string != 0)
	{
		xPos += gfx_putc_f(xPos, yPos, *string, font, color) + TEXT_SPACING;
		string++;
	}
	return xPos;
}

uint16_t gfx_fontheight_f(const uint8_t* font)
{
	uint8_t temp;
	sfs_read(font, &temp, FONT_OFFSET_HEIGHT,1);
	return ((uint16_t)temp);
}

void gfx_image_size(const uint8_t* imageName, uint16_t* xSize, uint16_t* ySize)
{
	if(sfs_filesize(imageName) != 0)
	{
		sfs_read(imageName, (uint8_t*)xSize, IMAGE_OFFSET_XDIM, 2);
		sfs_read(imageName, (uint8_t*)ySize, IMAGE_OFFSET_YDIM, 2);
	}
}

void gfx_image_draw(uint16_t xPos, uint16_t yPos, const uint8_t* imageName)
{
	uint16_t pixVal;
	// Total dimensions
	uint16_t xSize=0, ySize=0;
	gfx_image_size(imageName, &xSize, &ySize);

	if((xSize != 0) && (ySize != 0))
	{
		// Read a pixel, show a pixel
		for(uint16_t x = 0; x < xSize; x++)
		{
			for(uint16_t y = 0; y < ySize; y++)
			{
				sfs_read(imageName, (uint8_t*)&pixVal, IMAGE_OFFSET_DATA + 2*(y + (x*ySize)), 2);
				gfx_setPixel(xPos + x, yPos + y, pixVal);
			}
		}
	}
}

void d_int2str(char* string, uint32_t num, uint8_t length)
{
	uint8_t place;
	for(place = length; place > 0; place--)
	{
		string[place-1] = (num % 10) + '0';
		num /= 10;
	}
	// and null terminate
	string[length] = 0;
}

/*
uint8_t d_int2str(char* string, int32_t num)
{
	uint8_t neg = 0;
	uint8_t i, j;
	char temp;
	if(num < 0)
	{
		neg = 1;
		string[0]='-';
		num = -num;
	}
	string[neg] = '0' + (num % 10);
	num = num / 10;
	i = 0;
	while(num > 0)
	{
		// Fill digit characters in reverse order
		string[neg + i] = '0' + (num % 10);
		num = num / 10;
	}
	// Null terminate the string
	string[neg + i] = 0;

	// Flip the string around
	j = 0;
	for(j = 0;j < (i/2); j++)
	{
		temp = string[neg+i-j-1];
		string[neg+i-j-1] = string[neg+j];
		string[neg+j] = temp;
	}
	return neg+i;
}
*/
