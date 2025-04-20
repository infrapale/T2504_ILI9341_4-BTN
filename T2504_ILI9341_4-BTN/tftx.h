#ifndef __TFTX_H__
#define __TFTX_H__

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_DC    15
#define TFT_CS    9
#define TFT_RST   14
#define TFT_LED   10 

#define COLOR_BLACK     0x0000
#define COLOR_BLUE      0x001F
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_CYAN      0x07FF
#define COLOR_MAGENTA   0xF81F
#define COLOR_YELLOW    0xFFE0
#define COLOR_WHITE     0xFFFF

typedef enum
{
  FONT_STANDARD = 0,
  FONT_SANS_9,
  FONT_SANS_12,
  FONT_SANS_18,
  FONT_SANS_24,
  FONT_SANS_BOLD_9,
  FONT_SANS_BOLD_12,
  FONT_SANS_BOLD_18,
  FONT_SANS_BOLD_24,
  FONT_NBR_OF
} font_et;

typedef enum 
{
  BOX_FULL_SCREEN = 0,
  BOX_MENU_1,
  BOX_MENU_2,
  BOX_MENU_3,
  BOX_TOP,
  BOX_MID,
  BOX_LOW,
  BOX_NBR_OF
} box_et;

typedef struct
{
  uint16_t width;
  uint16_t height;
} display_st;

typedef struct
{
  bool    update;
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  uint16_t frame;
  uint16_t fill; 
  char text[80];
  font_et font;
  uint16_t txt_color;
  bool    txt_wrap;
} box_st;

void tftx_initialize(void);

void tftx_draw_box(box_st *boxp);

void tftx_update_boxes(void);

#endif