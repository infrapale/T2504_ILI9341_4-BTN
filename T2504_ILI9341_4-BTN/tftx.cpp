// https://learn.adafruit.com/adafruit-gfx-graphics-library
// https://github.com/adafruit/Adafruit-GFX-Library

#include "tftx.h"

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>	
#include <Fonts/FreeMonoOblique9pt7b.h>
#include "Font72x53rle.h"

#include <SPI.h>
#include "font72.h"

#define TFT_WIDTH    160
#define TFT_HEIGHT   128


//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC,TFT_RST);
display_st display;
int16_t font_voffs[FONT_NBR_OF] =
{
  [FONT_STANDARD] = 5,
  [FONT_SANS_9] = 12,
  [FONT_SANS_12] = 22,
  [FONT_SANS_18] = 28,
  [FONT_SANS_24] = 40,
  [FONT_SANS_BOLD_9] = 12,
  [FONT_SANS_BOLD_12] = 22,
  [FONT_SANS_BOLD_18] = 18,
  [FONT_SANS_BOLD_24] = 24,
  [FONT_72x53RLE] = 40
};


box_st *boxp[BOX_MAX_NBR_OF];

void tftx_initialize(void)
{
  pinMode(TFT_LED, OUTPUT);  
  digitalWrite(TFT_LED, HIGH);

  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

  tft.setRotation(3);
  display.width = tft.width();
  display.height = tft.height();
  display.last_box = -1;
  Serial.printf("Screen size: %d x %d\n", display.width, display.height);
  

  for (uint8_t i = 0; i < BOX_MAX_NBR_OF; i++)
  {
    boxp[i] = NULL;
  }

  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized");
  tft.fillScreen(COLOR_BLACK);
  tft.setRotation(3);
  
}
uint16_t tftx_get_width(void)
{
  return display.width;
}

uint16_t tftx_get_height(void)
{
  return display.height;
}


void tftx_add_box(box_st *box_ptr)
{
  if (display.last_box < 0) display.last_box = 0; 
  if (display.last_box < BOX_MAX_NBR_OF)
  {
    boxp[display.last_box++] = box_ptr;
  }
}

void tftx_set_font(font_et font)
{
    switch(font)
    {
      case FONT_STANDARD:
        tft.setFont();
        break;
      case FONT_SANS_9:
        tft.setFont(&FreeSans9pt7b);
        break;
      case FONT_SANS_12:
        tft.setFont(&FreeSans12pt7b);
        break;
      case FONT_SANS_18:
        tft.setFont(&FreeSans18pt7b);
        break;
      case FONT_SANS_24:
        tft.setFont(&FreeSans24pt7b);
        break;
      case FONT_SANS_BOLD_9:
        tft.setFont(&FreeSansBold9pt7b);
        break;
      case FONT_SANS_BOLD_12:
        tft.setFont(&FreeSansBold12pt7b);
        break;
      case FONT_SANS_BOLD_18:
        tft.setFont(&FreeSansBold18pt7b);
        break;
      case FONT_SANS_BOLD_24:
        tft.setFont(&FreeSansBold24pt7b);
        break;
      case FONT_72x53RLE:
        tft.setFont(&FreeSansBold9pt7b);
        //tft.setFont(&chrtbl_f72);
        //tft.drawBitmap( )
      default:
        tft.setFont(&FreeSansBold9pt7b);
        break;
    }
}

void tftx_draw_box(box_st *boxp)
{
    tft.fillRect(boxp->x, boxp->y, boxp->w, boxp->h, boxp->fill);
    tft.drawRect(boxp->x, boxp->y, boxp->w, boxp->h, boxp->frame);
}

void tftx_update_boxes(void)
{
  for (uint8_t i = 0; i < BOX_NBR_OF; i++)
  {
    if (boxp[i])
    {
      Serial.println(boxp[i]->text);
      if (boxp[i]->update)
      {
        tftx_draw_box(boxp[i]);
        tft.setTextWrap(boxp[i]->txt_wrap);
        tftx_set_font(boxp[i]->font);
        tft.setTextSize(boxp[i]->txt_size);
        tft.setCursor(boxp[i]->x + 2, boxp[i]->y + font_voffs[boxp[i]->font] * (int16_t)boxp[i]->txt_size);
        //tft.setCursor(menu_box[i].x + 2, menu_box[i].y + 18);
        tft.setTextColor(boxp[i]->txt_color);
        tft.print(boxp[i]->text);
      }
    } 
  }
  //tft.drawBitmap(80,120,test_bm,32,32,COLOR_WHITE);
  //tftx_draw_box(&box_test);
}