#include "keyboard.h"
#include "config.h"

keyboard::keyboard(config keyBindings) {
	moveForwardKey = keyBindings.getParameter("MOVE_FORWARD").c_str()[0];
	moveBackKey = keyBindings.getParameter("MOVE_BACK").c_str()[0];
	moveLeftKey = keyBindings.getParameter("MOVE_LEFT").c_str()[0];
	moveRightKey = keyBindings.getParameter("MOVE_LEFT").c_str()[0];
}

keyboard::keyboard() {
	moveForwardKey = 'w'; 
	moveBackKey = 's';
	moveLeftKey = 'a';
	moveRightKey = 'd';
};
