

/*
 *
 * Gamegirl
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : ggc.c
 * Date                : 09.03.2015
 * Font size in bytes  : 11256
 * Font width          : 10
 * Font height         : 13
 * Font first char     : 32
 * Font last char      : 128
 * Font used chars     : 96
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

//#include <inttypes.h>
//#include <avr/pgmspace.h>

#ifndef GAMEGIRL_H
#define GAMEGIRL_H

#define GAMEGIRL_WIDTH 10
#define GAMEGIRL_HEIGHT 13

static const uint8_t Gamegirl[] = {
    0x2B, 0xF8, // size
    0x0A, // width
    0x0D, // height
    0x20, // first char
    0x60, // char count
    
    // char widths
    0x00, 0x05, 0x08, 0x0A, 0x0B, 0x0B, 0x0A, 0x04, 0x08, 0x08, 
    0x0B, 0x0A, 0x04, 0x0A, 0x04, 0x08, 0x0A, 0x06, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x04, 0x04, 0x05, 0x06, 
    0x05, 0x0A, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x06, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x08, 
    0x08, 0x08, 0x08, 0x0C, 0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x06, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x04, 0x0A, 0x0B, 0x00, 
    
    // font data
    0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, // 33
    0x1C, 0x7C, 0x7C, 0x00, 0x1C, 0x1C, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 34
    0x60, 0x60, 0xF8, 0xF8, 0x60, 0x60, 0xF8, 0xF8, 0x60, 0x60, 0x18, 0x18, 0x38, 0x38, 0x18, 0x18, 0x38, 0x38, 0x18, 0x18, // 35
    0x60, 0x60, 0xF8, 0xF8, 0xD8, 0xFC, 0xFC, 0xD8, 0xD8, 0xD8, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xF8, 0xF8, 0x30, 0x38, 0x38, 0x18, // 36
    0x78, 0x78, 0x78, 0x78, 0x00, 0xC0, 0xE0, 0x60, 0x18, 0x18, 0x00, 0x00, 0x00, 0x30, 0x38, 0x18, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, // 37
    0xD8, 0xD8, 0x6C, 0x6C, 0x6C, 0xFC, 0xFC, 0x18, 0xC0, 0xC0, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x18, 0x30, 0x30, // 38
    0x7C, 0x7C, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00, // 39
    0xF8, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0x18, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, // 40
    0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x38, 0x18, // 41
    0x18, 0x78, 0x78, 0xF8, 0xFF, 0xFF, 0xF8, 0xF8, 0x78, 0x18, 0x18, 0x30, 0x38, 0x38, 0x18, 0x18, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, // 42
    0xC0, 0xC0, 0xC0, 0xF8, 0xF8, 0xF8, 0xF8, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, // 43
    0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0xF8, 0xF8, // 44
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 45
    0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, // 46
    0x00, 0x00, 0x00, 0xC0, 0xE0, 0x60, 0x18, 0x18, 0x30, 0x38, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, // 47
    0xF8, 0xF8, 0xFC, 0xFC, 0x0C, 0x0C, 0xFC, 0xFC, 0xF8, 0xF8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 48
    0x18, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x30, 0x38, 0x38, 0x38, 0x38, 0x30, // 49
    0x18, 0x18, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC, 0xFC, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, // 50
    0x0C, 0x0C, 0x6C, 0x6C, 0x6C, 0xFC, 0xFC, 0xFC, 0xD8, 0xD8, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 51
    0xF8, 0xF8, 0x1C, 0x1C, 0x0C, 0xFC, 0xFC, 0xFC, 0xC0, 0xC0, 0x18, 0x18, 0x18, 0x18, 0x18, 0x38, 0x38, 0x38, 0x18, 0x18, // 52
    0x7C, 0x7C, 0x7C, 0x7C, 0x6C, 0xEC, 0xEC, 0xEC, 0xC0, 0xC0, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 53
    0xF8, 0xF8, 0xFC, 0xFC, 0x6C, 0x6C, 0xEC, 0xEC, 0xC0, 0xC0, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 54
    0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xEC, 0xFC, 0x7C, 0x1C, 0x1C, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, // 55
    0xD8, 0xD8, 0x6C, 0x6C, 0x6C, 0xFC, 0xFC, 0xFC, 0xD8, 0xD8, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 56
    0x78, 0x78, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0x00, 0x00, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 57
    0x78, 0x78, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, // 58
    0x78, 0x78, 0x78, 0x78, 0x38, 0x38, 0xF8, 0xF8, // 59
    0xC0, 0xE0, 0x60, 0x18, 0x18, 0x00, 0x18, 0x18, 0x30, 0x30, // 60
    0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, // 61
    0x18, 0x78, 0x60, 0xC0, 0xC0, 0x30, 0x38, 0x18, 0x00, 0x00, // 62
    0x18, 0x18, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC, 0xFC, 0x78, 0x78, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, // 63
    0xE0, 0xE0, 0x18, 0xFC, 0xEC, 0x6C, 0xEC, 0xEC, 0x18, 0xF8, 0xE0, 0x18, 0x18, 0x30, 0xF8, 0xD8, 0xD8, 0xD8, 0xD8, 0x18, 0x18, 0x00, // 64
    0xF8, 0xF8, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0x38, 0x38, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, // 65
    0xFC, 0xFC, 0xFC, 0xFC, 0x64, 0x64, 0xFC, 0xFC, 0xD8, 0xD8, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 66
    0xF8, 0xF8, 0xFC, 0xFC, 0x04, 0x04, 0x1C, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 67
    0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0x38, 0x38, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 68
    0xFC, 0xFC, 0xFC, 0xFC, 0x6C, 0x6C, 0x6C, 0x6C, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 69
    0xFC, 0xFC, 0xFC, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 70
    0xF8, 0xF8, 0xFC, 0xFC, 0x04, 0xC4, 0xDC, 0xDC, 0xD8, 0xD8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x38, 0x38, // 71
    0xFC, 0xFC, 0x60, 0x60, 0x60, 0x60, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, // 72
    0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x30, 0x38, 0x38, 0x38, 0x38, 0x30, // 73
    0xC0, 0xC0, 0xC0, 0xCC, 0x0C, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x18, 0x18, 0x38, 0x38, 0x30, 0x38, 0x38, 0x18, 0x00, 0x00, // 74
    0xFC, 0xFC, 0xFC, 0xFC, 0xE0, 0xF8, 0xFC, 0x1C, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x00, 0x18, 0x38, 0x38, 0x30, 0x30, // 75
    0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 76
    0xFC, 0xFC, 0x78, 0xF8, 0xE0, 0x78, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, // 77
    0xFC, 0xFC, 0x78, 0xF8, 0xE0, 0xC0, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x00, 0x00, 0x00, 0x18, 0x38, 0x38, 0x38, 0x38, // 78
    0xF8, 0xF8, 0xFC, 0xFC, 0x0C, 0x0C, 0xFC, 0xFC, 0xF8, 0xF8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 79
    0xFC, 0xFC, 0xFC, 0xFC, 0xCC, 0xCC, 0xFC, 0xFC, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 80
    0xF8, 0xF8, 0xFC, 0xFC, 0x0C, 0xCC, 0xCC, 0x0C, 0xF8, 0xF8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x18, 0x30, 0x30, // 81
    0xFC, 0xFC, 0xFC, 0xFC, 0xCC, 0xCC, 0xFC, 0xFC, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, 0x00, 0x18, 0x38, 0x30, 0x30, 0x30, // 82
    0x18, 0x18, 0x7C, 0x7C, 0x6C, 0xEC, 0xEC, 0xEC, 0xC0, 0xC0, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 83
    0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, // 84
    0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 85
    0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x18, 0x38, 0x30, 0x38, 0x38, 0x18, 0x00, 0x00, // 86
    0xFC, 0xFC, 0xC0, 0xE0, 0xE0, 0xC0, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x18, 0x18, 0x00, 0x18, 0x38, 0x38, 0x38, 0x38, // 87
    0x0C, 0x0C, 0xD8, 0xF8, 0xE0, 0xF8, 0xFC, 0x1C, 0x0C, 0x0C, 0x38, 0x38, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 0x30, // 88
    0x1C, 0x1C, 0x7C, 0xFC, 0xE0, 0xE0, 0xFC, 0x7C, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, // 89
    0x0C, 0x0C, 0xCC, 0xEC, 0xEC, 0xFC, 0xFC, 0x7C, 0x1C, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, // 90
    0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, // 91
    0x18, 0x78, 0x60, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x30, // 92
    0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x38, 0x38, // 93
    0x18, 0x1C, 0x0C, 0x03, 0x0F, 0x0C, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 94
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, // 95
    0x1C, 0x1C, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, // 96
    0xF8, 0xF8, 0xCC, 0xCC, 0xCC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0x38, 0x38, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, // 97
    0xFC, 0xFC, 0xFC, 0xFC, 0x64, 0x64, 0xFC, 0xFC, 0xD8, 0xD8, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 98
    0xF8, 0xF8, 0xFC, 0xFC, 0x04, 0x04, 0x1C, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 99
    0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0x38, 0x38, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 100
    0xFC, 0xFC, 0xFC, 0xFC, 0x6C, 0x6C, 0x6C, 0x6C, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 101
    0xFC, 0xFC, 0xFC, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 102
    0xF8, 0xF8, 0xFC, 0xFC, 0x04, 0xC4, 0xDC, 0xDC, 0xD8, 0xD8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x38, 0x38, // 103
    0xFC, 0xFC, 0x60, 0x60, 0x60, 0x60, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, // 104
    0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x30, 0x38, 0x38, 0x38, 0x38, 0x30, // 105
    0xC0, 0xC0, 0xC0, 0xCC, 0x0C, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x18, 0x18, 0x38, 0x38, 0x30, 0x38, 0x38, 0x18, 0x00, 0x00, // 106
    0xFC, 0xFC, 0xFC, 0xFC, 0xE0, 0xF8, 0xFC, 0x1C, 0x0C, 0x0C, 0x38, 0x38, 0x38, 0x38, 0x00, 0x18, 0x38, 0x38, 0x30, 0x30, // 107
    0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, // 108
    0xFC, 0xFC, 0x78, 0xF8, 0xE0, 0x78, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, // 109
    0xFC, 0xFC, 0x78, 0xF8, 0xE0, 0xC0, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x00, 0x00, 0x00, 0x18, 0x38, 0x38, 0x38, 0x38, // 110
    0xF8, 0xF8, 0xFC, 0xFC, 0x0C, 0x0C, 0xFC, 0xFC, 0xF8, 0xF8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x38, 0x18, 0x18, // 111
    0xFC, 0xFC, 0xFC, 0xFC, 0xCC, 0xCC, 0xFC, 0xFC, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 112
    0xF8, 0xF8, 0xFC, 0xFC, 0x0C, 0xCC, 0xCC, 0x0C, 0xF8, 0xF8, 0x18, 0x18, 0x38, 0x38, 0x30, 0x30, 0x38, 0x18, 0x30, 0x30, // 113
    0xFC, 0xFC, 0xFC, 0xFC, 0xCC, 0xCC, 0xFC, 0xFC, 0x78, 0x78, 0x38, 0x38, 0x38, 0x38, 0x00, 0x18, 0x38, 0x30, 0x30, 0x30, // 114
    0x18, 0x18, 0x7C, 0x7C, 0x6C, 0xEC, 0xEC, 0xEC, 0xC0, 0xC0, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 115
    0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, // 116
    0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x18, 0x18, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x18, 0x18, // 117
    0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x18, 0x38, 0x30, 0x38, 0x38, 0x18, 0x00, 0x00, // 118
    0xFC, 0xFC, 0xC0, 0xE0, 0xE0, 0xC0, 0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x18, 0x18, 0x00, 0x18, 0x38, 0x38, 0x38, 0x38, // 119
    0x0C, 0x0C, 0xD8, 0xF8, 0xE0, 0xF8, 0xFC, 0x1C, 0x0C, 0x0C, 0x38, 0x38, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 0x30, // 120
    0x1C, 0x1C, 0x7C, 0xFC, 0xE0, 0xE0, 0xFC, 0x7C, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, // 121
    0x0C, 0x0C, 0xCC, 0xEC, 0xEC, 0xFC, 0xFC, 0x7C, 0x1C, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, 0x30, 0x30, // 122
    0x60, 0x60, 0xF8, 0xFC, 0xFC, 0xFC, 0xFC, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x18, 0x38, 0x38, 0x38, 0x38, 0x30, 0x30, 0x30, // 123
    0xFC, 0xFC, 0xFC, 0xFC, 0x38, 0x38, 0x38, 0x38, // 124
    0x0C, 0x0C, 0x0C, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0x60, 0x60, 0x30, 0x30, 0x30, 0x38, 0x38, 0x38, 0x38, 0x18, 0x00, 0x00, // 125
    0x18, 0x18, 0x1C, 0x1C, 0x0C, 0x1C, 0x1C, 0x18, 0x1C, 0x1C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 126
    
};

#endif