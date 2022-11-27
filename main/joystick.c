#include "joystick.h"


/// @brief Function to handle joystick status
/// @param pvParameter task parameter(NULL)
void joystick_task(void *pvParameter) {
    while(1) {
        // get raw value 
        int y_axis_val = adc1_get_raw(JOYSTICK_Y_AXIS);
        // set player speed according to joystick Y axis value
        if(y_axis_val >= 0 && y_axis_val <= 500) {
            pong.player.y_speed = 6;
        } else if (y_axis_val > 500 && y_axis_val <= 1000) {
            pong.player.y_speed = 3;
        } else if (y_axis_val > 1000 && y_axis_val <= 1800) {
            pong.player.y_speed = 1;
        } else if (y_axis_val > 1800 && y_axis_val <= 2200) {
            pong.player.y_speed = 0;
        } else if (y_axis_val > 2200 && y_axis_val <= 3000) {
            pong.player.y_speed = -1;
        } else if (y_axis_val > 3000 && y_axis_val <= 3500) {
            pong.player.y_speed = -3;
        } else {
            pong.player.y_speed = -6;
        }
        // delay to make space for other tasks to run
		vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


/// @brief initialize joystick
void init_joystick() {
    // set width (0-4095)
    adc1_config_width(ADC_WIDTH_BIT_12);
    // configure the channel
    adc1_config_channel_atten(JOYSTICK_Y_AXIS, ADC_ATTEN_DB_11);
}