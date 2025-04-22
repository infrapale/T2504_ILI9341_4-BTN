
#include "main.h"
#include "menu.h"
#include "atask.h"
#include "BtnPinOnOff.h"
#include "dashboard.h"
#include "tftx.h"
#define NBR_MENU_KEYS  4


void dummy_cb()
{
   // 
}

void menu_test1(void)
{
   Serial.printf("Test 1\n");
}

menu_row_st menu[MENU_NBR_OF] =
{
  [MENU_MAIN] =
  {
    "Main Menu", 
    {
      {"Main",MENU_MAIN, dummy_cb },
      {"Time",MENU_TIME, menu_test1 },
      {"Sensor", MENU_SENSOR, dummy_cb},
      {"Outd", MENU_OUTDOOR, dummy_cb}
    }
  },
  [MENU_TIME] =
  {
    "Time", 
    {
      {"Main",MENU_MAIN, dummy_cb },
      {"Time",MENU_TIME, dummy_cb},
      {"Set", MENU_TIME, dummy_cb},
      {"Main", MENU_MAIN, dummy_cb}
    }
  },
  [MENU_SENSOR] =
  {
    "Sensors", 
    {
      {"Main",MENU_MAIN, dummy_cb },
      {"Prev.",MENU_SENSOR, dashboard_previous_sensor},
      {"Next", MENU_SENSOR, dashboard_next_sensor},
      {"Main", MENU_MAIN, dummy_cb}
     }
  },
  [MENU_OUTDOOR] =
  {
    "Outdoor Sensors", 
    {
      {"Temp",MENU_MAIN, dummy_cb },
      {"Trend",MENU_SENSOR, dummy_cb},
      {"Limit", MENU_SENSOR, dummy_cb},
      {"Main", MENU_MAIN, dummy_cb}
    }
  }
};

menu_box_st menu_box[4] =
{
    {   5, 180, 312, 16, NULL },
    {   5, 200, 100, 32, NULL },
    { 110, 200, 100, 32, NULL },
    { 216, 200, 100, 32, NULL},  
};

menu_ctrl_st menu_ctrl = {ATASK_NOT_DEFINED,  MENU_MAIN};

BtnPinOnOff  menu_btn[NBR_MENU_KEYS];

void menu_read_machine(void);

//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st menu_key_task_handle =   {"Menu Key Task  ", 100,    0,     0,  255,    0,   1, menu_read_machine };
atask_st menu_key_scan_handle =   {"Menu Key Scan  ", 10,     0,     0,  255,    0,   1, menu_button_scan };


void menu_initialize(void)
{
  menu_btn[0].Init(PIN_KEY1,'3', true);
  menu_btn[1].Init(PIN_KEY2,'2', true);
  menu_btn[2].Init(PIN_KEY3,'1', true);
  menu_btn[3].Init(PIN_KEY_STATUS,'0', false);

  atask_add_new(&menu_key_task_handle);
  atask_add_new(&menu_key_scan_handle);
  menu_draw();

}


void menu_read_machine(void)
{
  char c = menu_read_button();
  if (c != 0x00) 
  {
      if ((c & 0b10000000) == 0) 
          Serial.printf("On");
      else 
          Serial.printf("Off");
      Serial.printf(" %c\n",c & 0b01111111);
      c &=  0b01111111;
      menu_btn_pressed(c);
  }

}

void menu_draw(void)
{

    //dashboard_set_menu_label(char *label);
    //dashboard_set_menu_text(uint8_t mindx, char *label);

    dashboard_set_menu_label(menu[menu_ctrl.active].row_label);

    for (uint8_t i = 1; i <= 3; i++)
    {
        dashboard_set_menu_text(i-1, menu[menu_ctrl.active].menu_item[i].label);
    }

}

void menu_button_scan(void)
{
    for( uint8_t i= 0; i < NBR_MENU_KEYS; i++)
    {
        menu_btn[i].Scan();
    }
}

char menu_read_button(void)
{
    char c = 0x00;
    for( uint8_t i= 0; i < NBR_MENU_KEYS; i++)
    {
        c = menu_btn[i].Read();
        if (c != 0x00) break;
    }
    return c;
}

void menu_btn_pressed(char pressed)
{
    uint8_t bindx = pressed - '0';
    if ((bindx >= 0) && (bindx <=3))
    {
        Serial.printf("active %d - pressed %d\n", menu_ctrl.active, bindx);
        menu[menu_ctrl.active].menu_item[bindx].cb_func();
        menu_ctrl.active = menu[menu_ctrl.active].menu_item[bindx].next;
        menu_draw();
    }

}
