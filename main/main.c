#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"
#include "font8x8_basic.h"
#include "driver/adc.h"


// JoyStick configuration

#define JOYSTICK_X_AXIS ADC1_CHANNEL_3
#define JOYSTICK_Y_AXIS ADC1_CHANNEL_0



/*
 You have to set this config value with menuconfig
 CONFIG_INTERFACE
 for i2c
 CONFIG_MODEL
 CONFIG_SDA_GPIO
 CONFIG_SCL_GPIO
 CONFIG_RESET_GPIO
 for SPI
 CONFIG_CS_GPIO
 CONFIG_DC_GPIO
 CONFIG_RESET_GPIO
*/

#define tag "SSD1306"

void app_main(void)
{
	SSD1306_t dev;
	int center, top, bottom;
	char lineChar[20];

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(tag, "INTERFACE is i2c");
	ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE


#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(tag, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(tag, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64

    // display program
	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
    ssd1306_display_text_x3(&dev, 0, "Hello", 5, false);
    vTaskDelay(3000 / portTICK_PERIOD_MS);


    // joystick program 

    while(1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(JOYSTICK_X_AXIS, ADC_ATTEN_DB_11);
        int x_axis_val = adc1_get_raw(JOYSTICK_X_AXIS);
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(JOYSTICK_Y_AXIS, ADC_ATTEN_DB_11);
        int y_axis_val = adc1_get_raw(JOYSTICK_Y_AXIS);
        printf("X-axis: %d\tY-axis: %d\n", x_axis_val, y_axis_val);
    }
    

	// Restart module
	esp_restart();
}