#include "Keyboard.h"
#include "Config.h"

void Keyboard::init (Config keyBindings) {
	moveForwardKey = keyBindings.getParameter("MOVE_FORWARD").c_str()[0];
	moveBackKey = keyBindings.getParameter("MOVE_BACK").c_str()[0];
	moveLeftKey = keyBindings.getParameter("MOVE_LEFT").c_str()[0];
	moveRightKey = keyBindings.getParameter("MOVE_RIGHT").c_str()[0];
	jumpKey = keyBindings.getParameter("JUMP").c_str()[0];

	keyRotationSensitivity = std::stof(keyBindings.getParameter("KEYS_ROTATION_SENSITIVITY"));

	rotateUpKey = keyBindings.getParameter("ROTATE_UP_KEY").c_str()[0];
	rotateDownKey = keyBindings.getParameter("ROTATE_DOWN_KEY").c_str()[0];
	rotateLeftKey = keyBindings.getParameter("ROTATE_LEFT_KEY").c_str()[0];
	rotateRightKey = keyBindings.getParameter("ROTATE_RIGHT_KEY").c_str()[0];
}

Keyboard::Keyboard(Config keyBindings) {
	init(keyBindings);
}

Keyboard::Keyboard() {
	Config defaultCfg("./configs/keyboard.cfg");
	init(defaultCfg);
};
