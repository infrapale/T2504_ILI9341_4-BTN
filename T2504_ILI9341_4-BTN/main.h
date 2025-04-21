#ifndef __MAIN_H__
#define __MAIN_H__


#define BOARD_PICO_TFT_4KEYS
#define PIRPANA
//#define LILLA_ASTRID
//#define VILLA_ASTRID

#include "io.h"
#include "atask.h"

#define LABEL_LEN           12
#define TXT_LEN             40
#define TIME_ZONE_OFFS      3
#define UNIT_LABEL_LEN      10
#define MEASURE_LABEL_LEN   16

#define APP_NAME   "T2504_ILI9341_4-BTN"
#define MAIN_TITLE "Villa Astrid Console"

typedef enum 
{
  AIO_PUBL_VA_HOME_MODE = 0,
  AIO_PUBL_VA_AC_TEMP,
  AIO_PUBL_NBR_OF
} aio_publ_et;


typedef enum
{
    UNIT_TEMPERATURE = 0,
    UNIT_HUMIDITY,
    UNIT_AIR_PRESSURE,
    UNIT_LIGHT,
    UNIT_LDR,
    UNIT_VOLTAGE,
    UNIT_TIME,
    UNIT_NBR_OF
} unit_et;


typedef struct date_time
{
    uint16_t  year;
    uint8_t   month;
    uint8_t   day;
    uint8_t   hour;
    uint8_t   minute;
    uint8_t   second;
 } date_time_st;

#endif