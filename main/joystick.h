#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "display.h"

// Define ADC channel for Y axis of the joystick
#define JOYSTICK_Y_AXIS ADC1_CHANNEL_0


void joystick_task(void *pvParameter);
void init_joystick();