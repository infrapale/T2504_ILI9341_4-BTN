#ifndef __AIO_MQTT_H__
#define __AIO_MQTT_H__

#define   LOCATION_LABEL_LEN   20
#include "Adafruit_MQTT.h"

typedef enum 
{
  AIO_SUBS_TIME = 0,
  AIO_SUBS_TRE_ID_TEMP,
  AIO_SUBS_TRE_ID_HUM,   
  AIO_SUBS_LA_ID_TEMP,
  AIO_SUBS_VA_OD_TEMP,
  AIO_SUBS_VA_OD_HUM,
  AIO_SUBS_VA_OD_LUX,
  AIO_SUBS_WATER_TEMP,
  AIO_SUBS_RUUVI_E6_TEMP,
  AIO_SUBS_RUUVI_EA_TEMP,
  AIO_SUBS_RUUVI_ED_TEMP,
  AIO_SUBS_NBR_OF
} aio_subs_et;

typedef enum
{
  AIO_PUBL_VA_HOME_MODE = 0,
  AIO_PUBL_VA_AC_TEMP,
  AIO_PUBL_NBR_OF
} aio_pub_et;

typedef void (*mqtt_cb)(void);

typedef struct
{
  Adafruit_MQTT_Subscribe *aio_subs;
  char                    location[LOCATION_LABEL_LEN];
  uint8_t                 unit_index;
  float                   value;
  uint8_t                 decimals;
  bool                    active;
  bool                    updated;
  uint32_t                show_interval_ms;
  uint32_t                show_next_ms;
} value_st;



void aio_mqtt_initialize(void);

void aio_mqtt_stm();

bool aio_mqtt_is_updated(uint8_t sindx);


#endif