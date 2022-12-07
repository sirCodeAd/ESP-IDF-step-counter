#ifndef Pins_H
#define Pins_H

#include "driver/gpio.h"

#include <esp_task_wdt.h>
#include <stdio.h> 

#define BUTTON_PIN      21
#define LED_PIN         16

void Init_Pins();

void blink_LEDs(int ms, int times);

#endif