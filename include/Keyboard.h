#ifndef SENSEFX_KEYBOARD_H
#define SENSEFX_KEYBOARD_H

#include "Config.h"

class Keyboard {

	private:
		void init(Config keyBindings); 

	public:
		float keyRotationSensitivity;

		unsigned char moveForwardKey;
		unsigned char moveBackKey;
		unsigned char moveLeftKey;
		unsigned char moveRightKey;
		unsigned char jumpKey;

		unsigned char rotateUpKey;
		unsigned char rotateDownKey;
		unsigned char rotateLeftKey;
		unsigned char rotateRightKey;

		Keyboard(Config keyBindings);
		Keyboard();
};

#endif
