#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"


/// @brief field structure
typedef struct {
    int width;
    int height;
    int x0;
    int x1;
    int y0;
    int y1;
}Field;


/// @brief stick structure
typedef struct {
    int x_pos;
    int y_pos;
    int y_speed;
	int length;
}Stick;


/// @brief ball structure
typedef struct Ball{
    int x_pos;
    int y_pos;

    int x_pos_old;
    int y_pos_old;

    int x_speed;
    int y_speed;

	int radius;
}Ball;


/// @brief game structure
typedef struct {
    Field field;
    Ball ball;
    Stick player;
    Stick bot;
    int score1;
    int score2;
}Pong;

// display variable
SSD1306_t display;
// game variable
Pong pong;


void init_display();
void draw_field();
void draw_stick(Stick stick);
void update_stick(Stick stick);
void draw_ball(Ball ball);
void update_ball(Ball ball);
void draw_objects();