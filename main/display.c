#include "display.h"


/// @brief Draw the play field
void draw_field() {
    // top edge
    _ssd1306_line(&display, 0, 0, 127, 0, false);
    // bottom edge
	_ssd1306_line(&display, 0, 63, 127, 63, false);
    // display edges
	ssd1306_show_buffer(&display);
}


/// @brief Draw the stick
/// @param stick stick structure
void draw_stick(Stick stick) {
    // set first column
    _ssd1306_line(&display, stick.x_pos, stick.y_pos, stick.x_pos, stick.y_pos+stick.length, false);
    // set second column
	_ssd1306_line(&display, stick.x_pos+1, stick.y_pos, stick.x_pos+1, stick.y_pos+stick.length, false);
    // draw stick
    ssd1306_show_buffer(&display);
}


/// @brief Update stick
/// @param stick stick structure
void update_stick(Stick stick) {
    // erase old position pixels if sticke moved 
    if(stick.y_speed < 0) {
        _ssd1306_line(&display, stick.x_pos, stick.y_pos + stick.length - stick.y_speed, stick.x_pos, stick.y_pos + stick.length + 1, true);
        _ssd1306_line(&display, stick.x_pos + 1, stick.y_pos + stick.length - stick.y_speed, stick.x_pos + 1,stick.y_pos + stick.length + 1, true);
    } else if(stick.y_speed > 0) {
        _ssd1306_line(&display, stick.x_pos, stick.y_pos - stick.y_speed, stick.x_pos, stick.y_pos - 1, true);
        _ssd1306_line(&display, stick.x_pos + 1, stick.y_pos - stick.y_speed, stick.x_pos + 1, stick.y_pos - 1, true);
    }
    ssd1306_show_buffer(&display);

    // draw dtick at new position
    if(stick.y_speed != 0) {
        draw_stick(stick);
    }
}


/// @brief Draw a ball
/// @param ball ball structure
void draw_ball(Ball ball) {
    // set ball pixels
    _ssd1306_pixel(&display, ball.x_pos, ball.y_pos + 1, false);
    _ssd1306_pixel(&display, ball.x_pos + 1, ball.y_pos, false);
    _ssd1306_pixel(&display, ball.x_pos, ball.y_pos, false);
    _ssd1306_pixel(&display, ball.x_pos - 1, ball.y_pos, false);
    _ssd1306_pixel(&display, ball.x_pos, ball.y_pos - 1, false);
    // draw ball
    ssd1306_show_buffer(&display);
}


/// @brief Update ball 
/// @param ball ball structure
void update_ball(Ball ball) {
    // erase old position pixels of the ball
    _ssd1306_pixel(&display, ball.x_pos_old, ball.y_pos_old, true);
    _ssd1306_pixel(&display, ball.x_pos_old + 1, ball.y_pos_old, true);
    _ssd1306_pixel(&display, ball.x_pos_old - 1, ball.y_pos_old, true);
    _ssd1306_pixel(&display, ball.x_pos_old, ball.y_pos_old + 1, true);
    _ssd1306_pixel(&display, ball.x_pos_old, ball.y_pos_old - 1, true);
    ssd1306_show_buffer(&display);
    // draw ball
    draw_ball(ball);
}


/// @brief initialize display
void init_display() {
    i2c_master_init(&display, 
                    CONFIG_SDA_GPIO,
                    CONFIG_SCL_GPIO,
                    CONFIG_RESET_GPIO);
    ssd1306_init(&display, 128, 64);
    
}


/// @brief draw all game objects
void draw_objects() {
    draw_field();
    draw_stick(pong.player);
    draw_stick(pong.bot);
    draw_ball(pong.ball);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}