#include "main.h"
#include "tftx.h"
#include "dashboard.h"
#include "aio_mqtt.h"
#include "time_func.h"
#include "atask.h"

#define MENU_BOX_NBR_OF       3
#define ROW_BOX_NBR_OF        8

#define BOX_HEIGHT_ROW        30
#define BOX_HEIGHT_MID        140
#define BOX_HEIGHT_MENU       30

box_st menu_box[MENU_BOX_NBR_OF];
box_st mid_box;
box_st row_box[ROW_BOX_NBR_OF];

typedef struct
{
    bool show_sensor_value;
    bool force_show_big_time;
    bool fast_forward;
    uint8_t sensor_indx;
    uint8_t menu_sensor_indx;
} dashboard_ctrl_st;

dashboard_ctrl_st dashboard_ctrl = {false, true, false, AIO_SUBS_TRE_ID_TEMP, 0};


char unit_label[UNIT_NBR_OF][UNIT_LABEL_LEN] =
{
  // 012345678
    "Celsius ",
    "%       ",
    "kPa     ",
    "Lux     ",
    "LDR     ",
    "V       "
};

char measure_label[UNIT_NBR_OF][MEASURE_LABEL_LEN] =
{
  // 0123456789012345
    "Temperature    ",
    "Humidity       ",
    "Air Pressure   ",
    "Light          ",
    "LDR Value      ",
    "Voltage        "
};


void dashboard_update_task(void);

//                                          123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st dashboard_task_handle        =   {"Dashboard SM   ", 1000,   0,     0,  255,    0,   1,  dashboard_update_task };

extern value_st subs_data[AIO_SUBS_NBR_OF];

void dashboard_start_task(void)
{
    atask_add_new(&dashboard_task_handle);
}

void dashboard_initialize(void)
{
  char txt[40];

  for (uint8_t i = 0; i < MENU_BOX_NBR_OF; i++)
  {
    menu_box[i].update     = true;
    menu_box[i].x          = i * tftx_get_width() / MENU_BOX_NBR_OF;
    menu_box[i].y          = tftx_get_height() - BOX_HEIGHT_MENU;
    menu_box[i].w          = tftx_get_width()/MENU_BOX_NBR_OF;
    menu_box[i].h          = BOX_HEIGHT_MENU;
    menu_box[i].frame      = COLOR_GREY;
    menu_box[i].fill       = COLOR_TEAL;
    menu_box[i].font       = FONT_SANS_12;
    menu_box[i].txt_color  = COLOR_WHITE;
    menu_box[i].txt_size   = 1;
    tftx_add_box(&menu_box[i]);
  }
  strcpy(menu_box[0].text, "Menu0");
  strcpy(menu_box[1].text, "Menu1");
  strcpy(menu_box[2].text, "Menu2");
  
  for (uint8_t i = 0; i < ROW_BOX_NBR_OF; i++)
  {
    row_box[i].update     = true;
    row_box[i].x          = 0;
    row_box[i].y          = i*BOX_HEIGHT_ROW ;
    row_box[i].w          = tftx_get_width();
    row_box[i].h          = BOX_HEIGHT_ROW ;
    row_box[i].frame      = COLOR_GREY;
    row_box[i].fill       = COLOR_BLUE;
    row_box[i].font       = FONT_SANS_12;
    row_box[i].txt_color  = COLOR_WHITE;
    row_box[i].txt_size   = 1;
    sprintf(txt,"Row %d",i);
    strcpy(row_box[i].text, txt);
    tftx_add_box(&row_box[i]);
  }
  row_box[ROW_BOX_NBR_OF -1].update = false;
  row_box[0].frame      = COLOR_GREY;
  row_box[0].fill       = COLOR_DARK_RED2;
  row_box[0].txt_color  = COLOR_WHITE;
  row_box[1].fill       = COLOR_DARK_RED;
  row_box[6].fill       = COLOR_DARK_RED;
  sprintf(txt, "%s", APP_NAME);
  tftx_set_text(&row_box[0], txt);


  mid_box.update = true;
  mid_box.x = 0;
  mid_box.y = row_box[2].y;
  mid_box.w = tftx_get_width();
  mid_box.h = row_box[0].h * 4;
  mid_box.frame = COLOR_YELLOW;
  mid_box.fill =  COLOR_DARK_GREY;
  mid_box.font = FONT_SANS_24;
  mid_box.txt_color = COLOR_YELLOW;
  mid_box.txt_size = 2;
  strcpy(mid_box.text, " 12:34");
  tftx_add_box(&mid_box); 
}

void dashboard_set_menu_label(char *label)
{
    tftx_set_text(&row_box[6], label);
}

void dashboard_set_menu_text(uint8_t mindx, char *label)
{
    tftx_set_text(&menu_box[mindx], label);
}
void dashboard_big_time(void)
{
    static uint8_t prev_minute = 99;
    DateTime *now = time_get_time_now();
    if ((now->minute() != prev_minute) || dashboard_ctrl.force_show_big_time)
    {
        prev_minute = now->minute();
        char s1[4];       
        sprintf(s1,"%02d",now->hour());
        String time_str = s1;
        time_str += ":";
        sprintf(s1,"%02d",now->minute());
        time_str += s1;
        tftx_set_string(&mid_box,&time_str);
    }
}


void dashboard_show_info(void)
{
    char txt[40];
    sprintf(txt, "%s", APP_NAME);
    tftx_set_text(&row_box[0], txt);
    sprintf(txt, "%s %s", __DATE__, __TIME__);
    tftx_set_text(&row_box[1], txt); 
}


void dashboard_show_common(void)
{
    String Str;
    if (!dashboard_ctrl.show_sensor_value)  
    {
        Str = MAIN_TITLE;
        tftx_set_string(&row_box[0],&Str);
        time_to_string(&Str);
        tftx_set_string(&row_box[1],&Str);
    }
}


void dashboard_update_task(void)
{
    static uint32_t next_step_ms;
    bool            update_box;
    String          Str;
    uint8_t         i; 
    
    switch (dashboard_task_handle.state)
    {
        case 0:
            dashboard_show_info();
            dashboard_task_handle.state++;
            break;
        case 1:                
            dashboard_show_common();
            dashboard_big_time();
            dashboard_ctrl.force_show_big_time = false;
            dashboard_task_handle.state++;
            break;
        case 2:
            update_box = false;
            i = (uint8_t)dashboard_ctrl.sensor_indx;
            if (millis() > subs_data[i].show_next_ms)
            {
                if ( subs_data[i].updated)
                {
                    dashboard_ctrl.show_sensor_value = true;
                    Serial.print("aio index: "); Serial.print(i); 
                    Serial.println(" = Updated ");
                    Str = subs_data[i].location;
                    tftx_set_string(&row_box[1],&Str);

                    Str = measure_label[subs_data[i].unit_index];
                    Str += " ";
                    Str = unit_label[subs_data[i].unit_index];
                    tftx_set_string(&row_box[6],&Str);

                    Str = String(subs_data[i].value);
                    Serial.println(Str);
                    tftx_set_string(&mid_box,&Str);
                    update_box = true;
                }
                subs_data[i].show_next_ms = millis() + subs_data[i].show_interval_ms;
            }

            if (dashboard_ctrl.sensor_indx < AIO_SUBS_NBR_OF - 1) dashboard_ctrl.sensor_indx++;    
            else dashboard_ctrl.sensor_indx = AIO_SUBS_TRE_ID_TEMP;
            
            if (update_box )
            {
                dashboard_task_handle.state = 3;
                next_step_ms = millis() + 10000;
            }
            else
            {
               dashboard_task_handle.state = 1;
            }
            break;  
        case 3:
            if ((millis() > next_step_ms) || dashboard_ctrl.fast_forward)
            {
                dashboard_ctrl.force_show_big_time = true;
                dashboard_task_handle.state = 1;
                dashboard_ctrl.show_sensor_value = false;
                dashboard_ctrl.fast_forward = false;
            } 
              
            break;
    }
    //Serial.printf("db %d -> %d\n", dashboard_task_handle.prev_state, dashboard_task_handle.state);
    tftx_update_boxes();
}

void dashboard_next_sensor(void)
{
/*    dashboard_ctrl.menu_sensor_indx++;
    if(dashboard_ctrl.menu_sensor_indx >= AIO_SUBS_NBR_OF) dashboard_ctrl.menu_sensor_indx = AIO_SUBS_TRE_ID_TEMP;
    subs_data[dashboard_ctrl.menu_sensor_indx].show_next_ms = 0              ;
    dashboard_ctrl.sensor_indx = dashboard_ctrl.menu_sensor_indx;
    Serial.printf("dashboard_ctrl.menu_sensor_indx=%d\n",dashboard_ctrl.menu_sensor_indx);
    dashboard_ctrl.fast_forward = true;
*/
}

void dashboard_previous_sensor(void)
{
/*    if(dashboard_ctrl.menu_sensor_indx <= 1 ) dashboard_ctrl.menu_sensor_indx = AIO_SUBS_NBR_OF -1;
    else dashboard_ctrl.menu_sensor_indx--;
    subs_data[dashboard_ctrl.menu_sensor_indx].show_next_ms = 0;
    dashboard_ctrl.sensor_indx = dashboard_ctrl.menu_sensor_indx;
    Serial.printf("dashboard_ctrl.menu_sensor_indx=%d\n",dashboard_ctrl.menu_sensor_indx);
    dashboard_ctrl.fast_forward = true;
*/    
}



