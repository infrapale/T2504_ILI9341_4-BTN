#ifndef __IO_H__ 
#define __IO_H__
#include <Arduino.h>
#define BOARD_PICO_TFT_4KEYS

#define PIN_WIRE_SDA         (12u)
#define PIN_WIRE_SCL         (13u)

#define PIN_KEY1		          (28u)
#define PIN_KEY2		          (27u)
#define PIN_KEY3		          (17u)
#define PIN_KEY_STATUS        (2u)
#define PIN_LDR               (26u)
#define PIN_PIR               (5u)
#define PIN_


void io_initialize(void);

uint16_t io_read_ldr(void);

bool io_read_pir(void);

#endif
