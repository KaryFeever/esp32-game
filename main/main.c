#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "joystick.h"
#include "pingpong.h"


/// @brief main function of the program
void app_main() {
	// initialise display
	init_display();
	// initialise joystick
	init_joystick();
	// start the game
	start_game();
}