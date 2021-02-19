#include "lvgl/lvgl.h"

/*******************************************************************************
 * Size: 12 px
 * Bpp: 2
 * Opts: 
 ******************************************************************************/

#ifndef FONT_PL
#define FONT_PL 1
#endif

#if FONT_PL

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+30 "0" */
    0xf, 0xc0, 0xe1, 0xc3, 0x3, 0x5c, 0xa, 0x70,
    0x28, 0xc0, 0xd3, 0x87, 0x3, 0xf0,

    /* U+31 "1" */
    0x0, 0x2, 0xe0, 0x5a, 0x0, 0xa0, 0xa, 0x0,
    0xa0, 0xa, 0x7, 0xfd,

    /* U+32 "2" */
    0x2f, 0x80, 0x43, 0x80, 0xa, 0x0, 0x38, 0x2,
    0xc0, 0x1c, 0x0, 0xd0, 0xf, 0xfc,

    /* U+33 "3" */
    0x3f, 0xe0, 0x7, 0x0, 0x30, 0x2, 0xf4, 0x0,
    0x70, 0x0, 0xc0, 0xb, 0xf, 0xe0,

    /* U+34 "4" */
    0x0, 0x0, 0xa, 0x0, 0x30, 0x2, 0x80, 0x1c,
    0x60, 0xd1, 0xc7, 0xff, 0xc0, 0x1c, 0x0, 0x60,

    /* U+35 "5" */
    0x1f, 0xe0, 0x60, 0x1, 0x80, 0xb, 0xf0, 0x0,
    0xb0, 0x0, 0xc0, 0xb, 0xf, 0xe0, 0x0, 0x0,

    /* U+36 "6" */
    0x0, 0x0, 0xb, 0x40, 0xe0, 0xa, 0x0, 0x3b,
    0xe0, 0xe0, 0xd3, 0x2, 0x8d, 0xd, 0xf, 0xd0,

    /* U+37 "7" */
    0x3f, 0xf4, 0x1, 0xc0, 0xe, 0x0, 0x30, 0x2,
    0x80, 0xc, 0x0, 0xa0, 0x3, 0x40, 0x0, 0x0,

    /* U+38 "8" */
    0xf, 0xd0, 0xd0, 0xc3, 0x43, 0x3, 0xb4, 0x28,
    0x71, 0xc0, 0xd3, 0x43, 0x47, 0xf8,

    /* U+39 "9" */
    0x1f, 0xd0, 0xd1, 0xc7, 0x3, 0x4d, 0x1d, 0x1f,
    0xb4, 0x1, 0xc0, 0x1d, 0x3, 0x80, 0x0, 0x0,

    /* U+41 "A" */
    0x3, 0x80, 0xa, 0xc0, 0xc, 0xd0, 0xc, 0xa0,
    0x28, 0x30, 0x3f, 0xf4, 0x70, 0x28, 0xa0, 0xc,

    /* U+42 "B" */
    0xff, 0x4d, 0x1c, 0xd1, 0xcf, 0xf4, 0xd0, 0xdd,
    0xa, 0xd1, 0xdf, 0xf4,

    /* U+43 "C" */
    0x7, 0xf8, 0x74, 0x3, 0x40, 0x1c, 0x0, 0x70,
    0x0, 0xd0, 0x1, 0xd0, 0x1, 0xfe,

    /* U+44 "D" */
    0xff, 0x83, 0x43, 0xcd, 0x3, 0x74, 0xa, 0xd0,
    0x2b, 0x40, 0xdd, 0xb, 0x3f, 0xe0,

    /* U+45 "E" */
    0xff, 0xf4, 0xd, 0x3, 0xfc, 0xd0, 0x34, 0xd,
    0x3, 0xff,

    /* U+46 "F" */
    0xff, 0xb4, 0xd, 0x3, 0xfc, 0xd0, 0x34, 0xd,
    0x3, 0x40,

    /* U+47 "G" */
    0x7, 0xf8, 0x1d, 0x0, 0x34, 0x0, 0x70, 0x0,
    0x70, 0xd, 0x34, 0xd, 0x1d, 0xd, 0x7, 0xfc,

    /* U+48 "H" */
    0xd0, 0x33, 0x40, 0xcd, 0x3, 0x3f, 0xfc, 0xd0,
    0x33, 0x40, 0xcd, 0x3, 0x34, 0xc,

    /* U+49 "I" */
    0xdd, 0xdd, 0xdd, 0xdd,

    /* U+4A "J" */
    0x3f, 0x7, 0x7, 0x7, 0x7, 0x6, 0xa, 0xbc,

    /* U+4B "K" */
    0xd0, 0xd3, 0x4a, 0xd, 0x70, 0x37, 0x40, 0xde,
    0x3, 0x4d, 0xd, 0x1c, 0x34, 0x28,

    /* U+4C "L" */
    0xd0, 0x34, 0xd, 0x3, 0x40, 0xd0, 0x34, 0xd,
    0x3, 0xfe,

    /* U+4D "M" */
    0x2c, 0x3, 0x4f, 0x42, 0xe3, 0xb0, 0xe8, 0xdc,
    0x6a, 0x31, 0xb1, 0x8c, 0x38, 0x63, 0x0, 0x1c,
    0xc0, 0x7,

    /* U+4E "N" */
    0xd0, 0x33, 0xc0, 0xce, 0x83, 0x33, 0x4c, 0xc3,
    0x33, 0xa, 0xcc, 0xf, 0x30, 0x1c,

    /* U+4F "O" */
    0x7, 0xf4, 0xb, 0x7, 0x43, 0x40, 0xb0, 0xc0,
    0x1c, 0x30, 0x7, 0xd, 0x2, 0xc2, 0xc1, 0xd0,
    0x2f, 0xd0,

    /* U+50 "P" */
    0xff, 0x4d, 0x1c, 0xd0, 0xdd, 0x1c, 0xff, 0x4d,
    0x0, 0xd0, 0xd, 0x0,

    /* U+52 "R" */
    0xff, 0xd, 0x2c, 0xd0, 0xcd, 0x2c, 0xdf, 0xd,
    0x70, 0xd2, 0xcd, 0xe,

    /* U+53 "S" */
    0x1f, 0xd0, 0xd0, 0x3, 0x40, 0x7, 0x90, 0x1,
    0xe0, 0x1, 0xc0, 0xb, 0x1f, 0xe0,

    /* U+54 "T" */
    0xbf, 0xf0, 0x1c, 0x0, 0x70, 0x1, 0xc0, 0x7,
    0x0, 0x1c, 0x0, 0x70, 0x1, 0xc0,

    /* U+55 "U" */
    0x30, 0xc, 0x30, 0xc, 0x30, 0xc, 0x30, 0xc,
    0x30, 0xc, 0x34, 0xc, 0x2c, 0x28, 0xb, 0xe0,

    /* U+56 "V" */
    0xb0, 0xc, 0x30, 0x2c, 0x34, 0x34, 0x28, 0x30,
    0xc, 0x60, 0xd, 0xd0, 0x7, 0xc0, 0x3, 0x80,

    /* U+57 "W" */
    0xb0, 0x34, 0x29, 0xc1, 0xe0, 0xd3, 0xa, 0xc3,
    0xd, 0x33, 0x1c, 0x29, 0xc9, 0xa0, 0x39, 0x1a,
    0x40, 0xf0, 0x3c, 0x2, 0xc0, 0xf0,

    /* U+58 "X" */
    0x30, 0x34, 0x28, 0x70, 0xc, 0xa0, 0xe, 0xc0,
    0xd, 0xd0, 0x1c, 0x70, 0x34, 0x34, 0x70, 0x1c,

    /* U+59 "Y" */
    0xb0, 0x28, 0xd0, 0xc1, 0xca, 0x3, 0xb0, 0x7,
    0x40, 0xc, 0x0, 0x30, 0x0, 0xc0,

    /* U+5A "Z" */
    0x3f, 0xf4, 0x2, 0x80, 0x1c, 0x0, 0xd0, 0xa,
    0x0, 0x30, 0x3, 0x40, 0x1f, 0xfc,

    /* U+61 "a" */
    0x2f, 0x80, 0xd, 0x1f, 0xd7, 0xd, 0x70, 0xd2,
    0xfd,

    /* U+62 "b" */
    0x30, 0x0, 0xc0, 0x3, 0xbd, 0xe, 0x1c, 0x30,
    0x34, 0xc0, 0xd3, 0xb, 0xf, 0xe0,

    /* U+63 "c" */
    0xf, 0xc3, 0x40, 0x70, 0x7, 0x0, 0x34, 0x1,
    0xfc,

    /* U+64 "d" */
    0x0, 0x30, 0x0, 0xc0, 0xbf, 0xd, 0xc, 0x70,
    0x31, 0xc0, 0xc3, 0x43, 0x7, 0xfc,

    /* U+65 "e" */
    0x1f, 0xc0, 0xc1, 0xc7, 0xff, 0x1c, 0x0, 0x34,
    0x0, 0x7f, 0x40,

    /* U+66 "f" */
    0xf, 0x4a, 0xb, 0xf0, 0xa0, 0x28, 0xa, 0x2,
    0x80, 0xa0,

    /* U+67 "g" */
    0x1f, 0xb4, 0xc2, 0x83, 0xa, 0xb, 0xe0, 0x30,
    0x0, 0xff, 0x87, 0x3, 0x28, 0x1c, 0x3f, 0x80,

    /* U+68 "h" */
    0x30, 0x0, 0xc0, 0x3, 0xbd, 0xe, 0x1c, 0x30,
    0x30, 0xc0, 0xc3, 0x3, 0xc, 0xc,

    /* U+69 "i" */
    0x34, 0x7, 0x49, 0x24, 0x92, 0x49,

    /* U+6A "j" */
    0xd, 0x0, 0x1e, 0xa, 0xa, 0xa, 0xa, 0xa,
    0xd, 0x7c,

    /* U+6B "k" */
    0x30, 0x0, 0xc0, 0x3, 0xd, 0xc, 0xa0, 0x37,
    0x0, 0xdc, 0x3, 0x1c, 0xc, 0x28,

    /* U+6C "l" */
    0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x2d,

    /* U+6D "m" */
    0x37, 0xdf, 0x83, 0x8b, 0xd, 0x30, 0x70, 0xd3,
    0x7, 0xd, 0x30, 0x70, 0xd3, 0x7, 0xd,

    /* U+6E "n" */
    0x37, 0xd0, 0xe1, 0xc3, 0x3, 0xc, 0xc, 0x30,
    0x30, 0xc0, 0xc0,

    /* U+6F "o" */
    0x1f, 0xc0, 0xd1, 0xc7, 0x3, 0x1c, 0xc, 0x34,
    0x70, 0x7f, 0x0,

    /* U+70 "p" */
    0x3b, 0xd0, 0xe0, 0xc3, 0x3, 0x4c, 0xd, 0x30,
    0x70, 0xff, 0x3, 0x0, 0xc, 0x0, 0x30, 0x0,

    /* U+72 "r" */
    0x37, 0xe, 0x3, 0x0, 0xc0, 0x30, 0xc, 0x0,

    /* U+73 "s" */
    0x2f, 0x8c, 0x3, 0xd0, 0xa, 0x1, 0xdf, 0xc0,

    /* U+74 "t" */
    0x0, 0x9, 0x7, 0xf0, 0xd0, 0x34, 0xd, 0x3,
    0x40, 0x7c,

    /* U+75 "u" */
    0x30, 0x33, 0x3, 0x30, 0x33, 0x3, 0x34, 0x30,
    0xff,

    /* U+76 "v" */
    0xa0, 0x30, 0xc2, 0x83, 0x4c, 0x6, 0x70, 0xe,
    0x40, 0x2c, 0x0,

    /* U+77 "w" */
    0xa0, 0xd1, 0xc7, 0x1e, 0x28, 0x32, 0x73, 0x42,
    0x72, 0x70, 0x1e, 0x1e, 0x0, 0xd0, 0xd0,

    /* U+78 "x" */
    0x70, 0xd3, 0x5c, 0x1e, 0x41, 0xa8, 0x30, 0xca,
    0xa,

    /* U+79 "y" */
    0xa0, 0x31, 0xc2, 0x83, 0xd, 0xa, 0x30, 0x1d,
    0x80, 0x3d, 0x0, 0x30, 0x1, 0x80, 0x3c, 0x0,

    /* U+7A "z" */
    0x7f, 0xc0, 0x38, 0x7, 0x0, 0xc0, 0x38, 0x7,
    0xfc,

    /* U+D3 "Ó" */
    0x0, 0x50, 0x0, 0xc0, 0x0, 0x0, 0xb, 0xf4,
    0x2c, 0x1d, 0x34, 0xb, 0x30, 0x7, 0x30, 0x7,
    0x34, 0xb, 0x2c, 0x1d, 0xb, 0xf4,

    /* U+F3 "ó" */
    0x2, 0x80, 0x0, 0x1, 0xfc, 0xd, 0x1c, 0x70,
    0x31, 0xc0, 0xc3, 0x47, 0x7, 0xf0,

    /* U+104 "Ą" */
    0x3, 0x80, 0xb, 0xc0, 0xc, 0xd0, 0xc, 0xa0,
    0x28, 0x30, 0x3f, 0xf4, 0x70, 0x28, 0xa0, 0xc,
    0x0, 0x24, 0x0, 0x30, 0x0, 0x2d,

    /* U+105 "ą" */
    0x2f, 0x80, 0xd, 0x1f, 0xd7, 0xd, 0x70, 0xd2,
    0xfd, 0x2, 0x40, 0x30, 0x2, 0xc0,

    /* U+106 "Ć" */
    0x0, 0x10, 0x3, 0x40, 0x0, 0x1, 0xfe, 0x1d,
    0x0, 0xd0, 0x7, 0x0, 0x1c, 0x0, 0x34, 0x0,
    0x74, 0x0, 0x7f, 0x80,

    /* U+107 "ć" */
    0x2, 0x80, 0x10, 0xf, 0xc3, 0x40, 0x70, 0x7,
    0x0, 0x34, 0x1, 0xfc,

    /* U+118 "Ę" */
    0xff, 0xf4, 0xd, 0x3, 0xfc, 0xd0, 0x34, 0xd,
    0x3, 0xff, 0x3, 0x2, 0x80, 0x78,

    /* U+119 "ę" */
    0x1f, 0x80, 0xd2, 0x87, 0x3, 0x1f, 0xfc, 0x30,
    0x0, 0x7f, 0x80, 0x18, 0x0, 0xd0, 0x1, 0xd0,

    /* U+141 "Ł" */
    0x30, 0x3, 0x40, 0x35, 0x3, 0xd0, 0x74, 0x7,
    0x40, 0x34, 0x3, 0xfe,

    /* U+142 "ł" */
    0x30, 0x30, 0x34, 0x38, 0xb0, 0x30, 0x30, 0x2d,

    /* U+143 "Ń" */
    0x1, 0x40, 0x1c, 0x0, 0x0, 0x34, 0xc, 0xf0,
    0x33, 0xa0, 0xcc, 0xd3, 0x30, 0xcc, 0xc2, 0xb3,
    0x3, 0xcc, 0x7, 0x0,

    /* U+144 "ń" */
    0x2, 0xc0, 0x4, 0x3, 0x7d, 0xe, 0x1c, 0x30,
    0x30, 0xc0, 0xc3, 0x3, 0xc, 0xc,

    /* U+15A "Ś" */
    0x0, 0x40, 0xd, 0x0, 0x0, 0x7, 0xf4, 0x34,
    0x0, 0xd0, 0x1, 0xe4, 0x0, 0x78, 0x0, 0x70,
    0x2, 0xc7, 0xf8, 0x0,

    /* U+15B "ś" */
    0x7, 0x1, 0x2, 0xf8, 0xc0, 0x3d, 0x0, 0xa0,
    0x1d, 0xfc,

    /* U+179 "Ź" */
    0x2, 0x80, 0x0, 0x3, 0xff, 0x40, 0x28, 0x1,
    0xc0, 0xd, 0x0, 0xb0, 0x3, 0x0, 0x38, 0x1,
    0xff, 0xd0,

    /* U+17A "ź" */
    0x3, 0x40, 0x0, 0x7f, 0xc0, 0x38, 0x7, 0x0,
    0xc0, 0x38, 0x7, 0xfc,

    /* U+17B "Ż" */
    0x0, 0x0, 0xd, 0x0, 0x0, 0xf, 0xfd, 0x0,
    0xa0, 0x7, 0x0, 0x34, 0x2, 0xc0, 0xc, 0x0,
    0xe0, 0x7, 0xff, 0x40,

    /* U+17C "ż" */
    0xb, 0x0, 0x10, 0x7f, 0xc0, 0x38, 0x7, 0x0,
    0xc0, 0x38, 0x7, 0xfc
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 14, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 54, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 70, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 86, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 102, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 118, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 148, .adv_w = 118, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 114, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 119, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 134, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 103, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 214, .adv_w = 97, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 131, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 132, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 53, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 76, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 266, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 95, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 157, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 131, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 140, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 109, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 364, .adv_w = 105, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 101, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 129, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 121, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 177, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 117, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 114, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 109, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 99, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 499, .adv_w = 113, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 92, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 113, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 107, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 547, .adv_w = 71, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 557, .adv_w = 106, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 573, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 587, .adv_w = 55, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 593, .adv_w = 56, .box_w = 4, .box_h = 10, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 603, .adv_w = 99, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 617, .adv_w = 57, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 625, .adv_w = 164, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 111, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 651, .adv_w = 111, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 662, .adv_w = 113, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 678, .adv_w = 71, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 87, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 69, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 704, .adv_w = 110, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 713, .adv_w = 102, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 724, .adv_w = 149, .box_w = 10, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 739, .adv_w = 98, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 748, .adv_w = 102, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 764, .adv_w = 92, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 773, .adv_w = 139, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 795, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 809, .adv_w = 118, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 831, .adv_w = 99, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 845, .adv_w = 119, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 92, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 877, .adv_w = 103, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 891, .adv_w = 107, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 907, .adv_w = 95, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 919, .adv_w = 57, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 927, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 947, .adv_w = 111, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 961, .adv_w = 105, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 981, .adv_w = 87, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 991, .adv_w = 109, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1009, .adv_w = 92, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1021, .adv_w = 109, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1041, .adv_w = 92, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_5[] = {
    0x0, 0x20, 0x31, 0x32, 0x33, 0x34, 0x45, 0x46,
    0x6e, 0x6f, 0x70, 0x71, 0x87, 0x88, 0xa6, 0xa7,
    0xa8, 0xa9
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 48, .range_length = 10, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 16, .glyph_id_start = 11,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 82, .range_length = 9, .glyph_id_start = 27,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 16, .glyph_id_start = 36,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 114, .range_length = 9, .glyph_id_start = 52,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 211, .range_length = 170, .glyph_id_start = 61,
        .unicode_list = unicode_list_5, .glyph_id_ofs_list = NULL, .list_length = 18, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 2, 3, 4, 5,
    6, 7, 0, 0, 0, 8, 9, 0,
    0, 4, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 0,
    21, 23, 24, 20, 0, 0, 25, 26,
    20, 20, 21, 21, 27, 28, 0, 0,
    29, 30, 31, 32, 33, 4, 21, 1,
    20, 3, 22, 5, 21, 9, 26, 0,
    20, 12, 28, 19, 33, 19, 33
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 2, 0, 0,
    0, 2, 0, 0, 3, 0, 0, 0,
    0, 2, 0, 0, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 0, 13, 13,
    13, 14, 15, 0, 16, 17, 0, 0,
    18, 18, 13, 18, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 2, 13, 1,
    12, 2, 13, 0, 13, 0, 0, 0,
    18, 4, 19, 11, 26, 11, 26
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, -5, 0, 0, -12, 0, -8, -4,
    0, -12, 0, 0, -1, -1, 0, 0,
    0, 0, 0, 0, 0, -4, -2, 0,
    -2, 0, 0, 0, 0, 0, -5, 0,
    -4, 0, 0, -6, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    -3, -1, 0, 0, 0, 0, -2, -4,
    -4, -4, -4, 0, -3, 0, -5, 0,
    -3, 0, -7, 0, -5, -3, -4, -7,
    -5, 0, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -9, 0, 0, -1, 0, 0,
    0, 0, 0, 0, -1, -6, -4, 0,
    -4, 0, 0, -3, -5, 0, 0, -1,
    -1, 0, 0, -3, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 2, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, -1, -4, -2, 0, -2, 0,
    0, -6, 0, 0, -18, -5, -17, -12,
    0, -21, 0, 0, 0, 0, 0, -2,
    0, 0, 0, 0, 0, -12, -4, 0,
    -4, 0, -10, 0, -4, 0, 0, 0,
    0, 0, -4, 0, -4, -3, -4, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -3, 0, 0,
    -3, 0, -2, 0, 0, -4, 0, 0,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -12, -7, 0, -2, 0, 0, 0, 0,
    0, 0, 0, -13, -16, -6, -18, 0,
    0, -14, -15, -9, -13, -12, -8, -12,
    -10, -15, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -8, -5, 0, -1,
    0, 0, 0, 0, 0, 0, 0, -9,
    -11, 0, -12, 0, 0, -6, -10, -4,
    -6, 0, 0, 0, 0, -8, -4, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -6, -7, 0, -6, 0, 0, -3,
    -7, 0, 0, 0, 0, 0, 0, -5,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -12, -7, 0, -4, 0, 0,
    0, 0, 0, 0, 0, -14, -17, -6,
    -17, 0, 0, -8, -15, -7, -10, -7,
    -7, -7, -7, -10, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    -5, 0, 0, 0, 0, 0, 0, -4,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, -1, 0,
    0, 0, 0, 0, 0, -4, -3, -2,
    -2, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    -5, 0, -3, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -2, 0, 0, 0, 7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -4, -1, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -3,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -2,
    -3, 0, -1, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, -4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, 0, 0, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 33,
    .right_class_cnt     = 26,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = gylph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 6,
    .bpp = 2,
    .kern_classes = 1,
    .bitmap_format = 0
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
lv_font_t font_pl = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FONT_PL*/

