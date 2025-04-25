/******************************************************************************
    T2504_ILI9341_4-BTN.ino
    This code is implmenting a IOT monitor connected to my Adafruit IO feeds.
    This appplcation is running on my own PCB with a Pi Pico W, a 2.8" TFT 
    display and 4 keys for menu functions.  The PCB also includes a PCF8563 RTC.
    I have not yet add/tested the SD card reader included with display module. 
    I do not intend to use the touch screen functions on the display module.
*******************************************************************************
    In this application I am also testing a helper layer on top of the 
    Adafruit_GFX library. I called this module tftx as my imagination was somewhat 
    limited at the time I wrote the code. My plan is to use the tftx helper module 
    also for other modules. I include more information in the tftx.cpp file
*******************************************************************************


    -----------------------
    |  main ino           |
    |          -----------|
    |          |  menu    |
    -----------------------
    |  dashboard          |
    -----------------------
    |  tftx               |
    -----------------------
    |  Adafruit_GFX       |
    -----------------------
    |  Adafruit_ILI9341   |
    -----------------------

 *******************************************************************************/

#include <Wire.h>
#include <WiFi.h>
#include "SPI.h"
#include "main.h"
#include "io.h"
#include "atask.h"
#include "secrets.h"
#include "tftx.h"
#include "dashboard.h"
#include "io.h"
#include "menu.h"
#include "time_func.h"
#include "aio_mqtt.h"
#include "logger.h"


void print_debug_task(void)
{
  atask_print_status(true);
  Serial.printf("LDR= %d  PIR = %d\n", io_read_ldr(), io_read_pir());
}
//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st debug_task_handle    =   {"Debug Task     ", 5000,    0,     0,  255,    0,  1,  print_debug_task };

uint32_t  targetTime = 0; 


void setup() {
  Serial.begin(9600);
  while(!Serial){}
  //delay(3000);
  Serial.println(APP_NAME);  Serial.print(" Compiled: ");
  Serial.print(__DATE__); Serial.print(" ");
  Serial.print(__TIME__); Serial.println();
  io_initialize();
  atask_initialize();
  logger_initialize(); 
  tftx_initialize();
  dashboard_initialize();
  tftx_update_boxes(); 

  Wire.setSDA(PIN_WIRE_SDA);
  Wire.setSCL(PIN_WIRE_SCL);
  Wire.begin();
  time_begin();
  
  atask_add_new(&debug_task_handle);
  dashboard_start_task();

  menu_initialize();        // starting scan and read tasks
  aio_mqtt_initialize();    // task is stopped - for debug purpose only
  logger_do_log();
}

void setup1()
{
  //Watchdog.reset();
  aio_mqtt_initialize();
  targetTime = millis() + 100;
}

void loop(void) 
{
  atask_run();
}

// Slow running loop: WiFI an MQTT
void loop1()
{
  if (millis() > targetTime)
  {
    aio_mqtt_stm();
    targetTime = millis() + 100;
  }
}
