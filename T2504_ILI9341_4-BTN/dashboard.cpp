#include "tftx.h"

#define MENU_BOX_NBR_OF  3
#define INFO_BOX_NBR_OF  4

#define MENU_HEIGHT     30
#define TOP_BOX_HEIGHT  30
#define MID_BOX_HEIGHT  100
#define LOW_BOX_HEIGHT  40

box_st menu_box[MENU_BOX_NBR_OF];
box_st top_box;
box_st mid_box;
box_st low_box;


void dashboard_initialize(void)
{
  for (uint8_t i = 0; i < MENU_BOX_NBR_OF; i++)
  {
    menu_box[i].update     = true;
    menu_box[i].x          = i * tftx_get_width() / 3;
    menu_box[i].y          = tftx_get_height() - MENU_HEIGHT;
    menu_box[i].w          = tftx_get_width()/3;
    menu_box[i].h          = MENU_HEIGHT;
    menu_box[i].frame      = COLOR_YELLOW;
    menu_box[i].fill       = COLOR_BLUE;
    menu_box[i].font       = FONT_SANS_12;
    menu_box[i].txt_color  = COLOR_CYAN;
    menu_box[i].txt_size   = 1;
    tftx_add_box(&menu_box[i]);
  }
  strcpy(menu_box[0].text, "Powr");
  strcpy(menu_box[1].text, "Opt");
  strcpy(menu_box[2].text, "Test");

  top_box.update = true;
  top_box.x = 0;
  top_box.y = 0;
  top_box.w = tftx_get_width();
  top_box.h = TOP_BOX_HEIGHT;
  top_box.frame = COLOR_WHITE;
  top_box.fill =  COLOR_BLACK;
  top_box.font = FONT_SANS_12;
  top_box.txt_color = COLOR_WHITE;
  top_box.txt_size = 1;
  strcpy(top_box.text, "Sending");
  tftx_add_box(&top_box);

  mid_box.update = true;
  mid_box.x = 0;
  mid_box.y = top_box.y + top_box.h + 1;
  mid_box.w = tftx_get_width();
  mid_box.h = MID_BOX_HEIGHT;
  mid_box.frame = COLOR_YELLOW;
  mid_box.fill =  COLOR_BLACK;
  mid_box.font = FONT_SANS_24;
  mid_box.txt_color = COLOR_YELLOW;
  mid_box.txt_size = 2;
  strcpy(mid_box.text, "12:34");
  tftx_add_box(&mid_box);

  low_box.update = true;
  low_box.x = 0;
  low_box.y = mid_box.y + mid_box.h + 1;
  low_box.w = tftx_get_width();
  low_box.h = LOW_BOX_HEIGHT;
  low_box.frame = COLOR_CYAN;
  low_box.fill =  COLOR_RED;
  low_box.font = FONT_SANS_18;
  low_box.txt_color = COLOR_BLACK;
  low_box.txt_size = 1;
  strcpy(low_box.text, "Err 42");
  tftx_add_box(&low_box);




}