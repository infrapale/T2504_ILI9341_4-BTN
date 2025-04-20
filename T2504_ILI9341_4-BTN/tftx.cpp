// https://learn.adafruit.com/adafruit-gfx-graphics-library

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
#include <SPI.h>

#define TFT_WIDTH    160
#define TFT_HEIGHT   128
#define MENU_HEIGHT  16
#define TOP_BOX_HEIGHT 24
#define MID_BOX_HEIGHT 50
#define LOW_BOX_HEIGHT 36

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC,TFT_RST);
display_st display;
int16_t font_voffs[FONT_NBR_OF] =
{
  [FONT_STANDARD] = 5,
  [FONT_SANS_9] = 12,
  [FONT_SANS_12] = 12,
  [FONT_SANS_18] = 28,
  [FONT_SANS_24] = 40,
  [FONT_SANS_BOLD_9] = 12,
  [FONT_SANS_BOLD_12] = 12,
  [FONT_SANS_BOLD_18] = 18,
  [FONT_SANS_BOLD_24] = 24,
};
box_st box[BOX_NBR_OF];

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
  Serial.printf("Screen size: %d x %d\n", display.width, display.height);
  

  for (uint8_t i = 0; i < BOX_NBR_OF; i++)
  {
    box[i].update = true;
  }

  for (uint8_t i = BOX_MENU_1; i <= BOX_MENU_3; i++)
  {
    box[i].update     = true;
    box[i].x          = (i-1) * display.width / 3;
    box[i].y          = display.height - MENU_HEIGHT;
    box[i].w          = display.width/3;
    box[i].h          = MENU_HEIGHT;
    box[i].frame      = COLOR_YELLOW;
    box[i].fill       = COLOR_BLUE;
    box[i].font       = FONT_STANDARD;
    box[i].txt_color  = COLOR_CYAN;
  }
  strcpy(box[BOX_MENU_1].text, "Powr");
  strcpy(box[BOX_MENU_2].text, "Opt");
  strcpy(box[BOX_MENU_3].text, "Test");

  box[BOX_TOP].update = true;
  box[BOX_TOP].x = 0;
  box[BOX_TOP].y = 0;
  box[BOX_TOP].w = display.width;
  box[BOX_TOP].h = TOP_BOX_HEIGHT;
  box[BOX_TOP].frame = COLOR_WHITE;
  box[BOX_TOP].fill =  COLOR_BLACK;
  box[BOX_TOP].font = FONT_SANS_12;
  box[BOX_TOP].txt_color = COLOR_WHITE;
  strcpy(box[BOX_TOP].text, "Sending");

  box[BOX_MID].update = true;
  box[BOX_MID].x = 0;
  box[BOX_MID].y = box[BOX_TOP].y + box[BOX_TOP].h + 1;
  box[BOX_MID].w = display.width;
  box[BOX_MID].h = MID_BOX_HEIGHT;
  box[BOX_MID].frame = COLOR_YELLOW;
  box[BOX_MID].fill =  COLOR_BLACK;
  box[BOX_MID].font = FONT_SANS_24;
  box[BOX_MID].txt_color = COLOR_YELLOW;
  strcpy(box[BOX_MID].text, "#1234");

  box[BOX_LOW].update = true;
  box[BOX_LOW].x = 0;
  box[BOX_LOW].y = box[BOX_MID].y + box[BOX_MID].h + 1;
  box[BOX_LOW].w = display.width;
  box[BOX_LOW].h = LOW_BOX_HEIGHT;
  box[BOX_LOW].frame = COLOR_CYAN;
  box[BOX_LOW].fill =  COLOR_RED;
  box[BOX_LOW].font = FONT_SANS_18;
  box[BOX_LOW].txt_color = COLOR_BLACK;
  strcpy(box[BOX_LOW].text, "Err 42");



  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized");
  tft.fillScreen(COLOR_BLACK);
  tft.setRotation(3);
  
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
      if (box[i].update)
      {
        tftx_draw_box(&box[i]);
        tft.setTextWrap(box[i].txt_wrap);
        tft.setCursor(box[i].x + 2, box[i].y + font_voffs[box[i].font]);
        //tft.setCursor(menu_box[i].x + 2, menu_box[i].y + 18);
        tftx_set_font(box[i].font);
        tft.setTextColor(box[i].txt_color);
        tft.print(box[i].text);
      }
  }
  //tftx_draw_box(&box_test);
}