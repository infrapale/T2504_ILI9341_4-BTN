#include <Arduino.h>
#include "main.h"
#include "io.h"

void io_initialize(void)
{
  analogReadResolution(12);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LDR, INPUT);

}

uint16_t io_read_ldr(void)
{
  return analogRead(PIN_LDR);
}

bool io_read_pir(void)
{
  return (digitalRead(PIN_PIR) == HIGH);
}