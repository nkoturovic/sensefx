#ifndef SENSEFX_KEYBOARD_H
#define SENSEFX_KEYBOARD_H

#include "config.h"

class keyboard {

	public:

		unsigned char moveForwardKey;
		unsigned char moveBackKey;
		unsigned char moveLeftKey;
		unsigned char moveRightKey;

		keyboard(config keyBindings);
		keyboard();
};

#endif
