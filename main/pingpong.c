#include "pingpong.h"


/// @brief initialize game objects 
void init_game_objects() {
    pong.field.x0 = 1;
    pong.field.x1 = 126;
    pong.field.y0 = 1;
    pong.field.y1 = 62;
    pong.field.width = 126;
    pong.field.height = 62;

    pong.ball.x_pos = 64;
    pong.ball.y_pos = 32;
    pong.ball.x_speed = 5;
    pong.ball.y_speed = 5;
    pong.ball.radius = 1;

    pong.player.length = 12;
    pong.player.x_pos = 0;
    pong.player.y_pos = pong.field.y0;
    pong.player.y_speed = 0;

    pong.bot.length = 12;
    pong.bot.x_pos = 126;
    pong.bot.y_pos = 32;
    pong.bot.y_speed = 5;

    pong.score1 = 0;
    pong.score2 = 0;
}


/// @brief Process score
void process_score() {

    if(pong.score1 == 5) {
        // clear the screen
        ssd1306_clear_screen(&display, false);
        // print text
        ssd1306_display_text(&display, 3, "!!!PLAYER WON!!!", 16, false);   
        // wait
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // clear the screen
        ssd1306_clear_screen(&display, false);
        // reset score
        pong.score1 = 0;
        pong.score2 = 0;
    } else if (pong.score2 == 5) {
        // clear the screen
        ssd1306_clear_screen(&display, false);
        // print text
        ssd1306_display_text(&display, 3, "!!!AI BOT WON!!!", 16, false);
        // wait
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // clear the screen
        ssd1306_clear_screen(&display, false);
        // reset score
        pong.score1 = 0;
        pong.score2 = 0;
    } else {
        // make string with score
        char score[5];
        snprintf(score, 5, " %d:%d", pong.score1, pong.score2);
        // clear the screen
        ssd1306_clear_screen(&display, false);
        // print score
        ssd1306_display_text_x3(&display, 3, score, strlen(score), false);
        // wait
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // clear the screen
        ssd1306_clear_screen(&display, false);

    }
    // set ball position to the center of the screen
    pong.ball.x_pos = 64;
    pong.ball.y_pos = 32;
    // redraw game objects
    draw_objects();
}


/// @brief process ball
void process_ball() {
    // save old position
    pong.ball.x_pos_old = pong.ball.x_pos;
    pong.ball.y_pos_old = pong.ball.y_pos;

    // check left border
    if((pong.ball.x_pos - pong.ball.radius + pong.ball.x_speed) <= 2)
    {   
        
        if(((pong.ball.y_pos - pong.ball.radius) <= pong.player.y_pos + pong.player.length) &&
           ((pong.ball.y_pos + pong.ball.radius) >= pong.player.y_pos))
        {
            // ball hits stick
            if(pong.ball.y_pos < pong.player.y_pos + 6) {
                pong.ball.y_speed = -5;
            } else {
                pong.ball.y_speed = 5;
            }
            pong.ball.x_pos = 3;
            pong.ball.x_speed*=-1;
        } else {
            // ball hits left border
            pong.score2++;
            process_score();
            return;
        }
        
    } else if((pong.ball.x_pos - pong.ball.radius + pong.ball.x_speed) >= 125) {
        // check right border
        if(((pong.ball.y_pos - pong.ball.radius) <= pong.bot.y_pos + pong.bot.length) &&
           ((pong.ball.y_pos + pong.ball.radius) >= pong.bot.y_pos))
        {
            // ball hits stick
            if(pong.ball.y_pos < pong.bot.y_pos + 6) {
                pong.ball.y_speed = -5;
            } else {
                pong.ball.y_speed = 5;
            }
            pong.ball.x_pos = 124;
            pong.ball.x_speed*=-1;
        } else {
            // ball hits right border
            pong.score1++;
            process_score();
            return;
        }
    } else {
        // ball between borders
        pong.ball.x_pos+=pong.ball.x_speed;
    }
    // check y axis ball position
    if(((pong.ball.y_pos + pong.ball.radius + pong.ball.y_speed) >= pong.field.y1) ||
        ((pong.ball.y_pos - pong.ball.radius + pong.ball.y_speed) <= pong.field.y0)) {
        // ball hits top/bottom border
        if(pong.ball.y_speed > 0) {
            pong.ball.y_pos=pong.field.y1 - 1;
        } else {
            pong.ball.y_pos=pong.field.y0 + 1;
        }
        pong.ball.y_speed*=-1;
    } else {
        // ball between top and bottom borders
        pong.ball.y_pos+=pong.ball.y_speed;
    }
}


/// @brief process player
void process_player() {

    if(!(((pong.player.y_pos + pong.player.y_speed + pong.player.length) > pong.field.y1) ||
       ((pong.player.y_pos + pong.player.y_speed) <= pong.field.y0))) {
        // stick between top and bottom borders
        pong.player.y_pos+= pong.player.y_speed;
    } else {
        if(pong.player.y_speed > 0) {
            // set stick to the lowest position
            pong.player.y_pos = pong.field.y1 - pong.player.length;
        } else {
            // set stick to the highest position
            pong.player.y_pos = pong.field.y0;
        }
    }
}


/// @brief process bot
void process_bot() {
    // change bot stick speed according to ball position 
    if((pong.bot.y_pos + 4) < pong.ball.y_pos) {
        pong.bot.y_speed = 4;
    } else if ((pong.bot.y_pos + 8) > pong.ball.y_pos) {
        pong.bot.y_speed = -4;
    } else {
        pong.bot.y_speed = 0;
    }
    // change bot stick position 
    if(!(((pong.bot.y_pos + pong.bot.y_speed + pong.bot.length) > pong.field.y1) ||
       ((pong.bot.y_pos + pong.bot.y_speed) < pong.field.y0))) {
        // bot stick between top and bottom border
        pong.bot.y_pos+= pong.bot.y_speed;
    } 
    else {
        if(pong.bot.y_speed > 0) {
            // set stick to the lowest position
            pong.bot.y_pos = pong.field.y1 - pong.bot.length;
        } else {
            // set stick to the highest position
            pong.bot.y_pos = pong.field.y0;
        }
    }
}


/// @brief Handle display
/// @param pvParameter task parameter
void display_task(void *pvParameter) {
    while(1) {
        // process player
        process_player();
        // update player stick position
        update_stick(pong.player);
        // process bot
        process_bot();
        // update bot stick position
        update_stick(pong.bot);
        // process ball
        process_ball();
        // update ball position
        update_ball(pong.ball);
        // delay to make space for other tasks to run
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


/// @brief start the game
void start_game() {
    // init game objects 
    init_game_objects();
    // draw game objects
    draw_objects();
    // create display task
    xTaskCreate(&display_task, "display_task", 2048, NULL, 5, NULL);
    // create joystick task
    xTaskCreate(&joystick_task, "joystick_task", 2048, NULL, 5, NULL);
}