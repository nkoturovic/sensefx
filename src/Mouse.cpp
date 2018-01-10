#include "Mouse.h"

void Mouse::init(Config mouseConfig) {
	sensitivity = stof(mouseConfig.getParameter("SENSITIVITY"));
}

Mouse::Mouse() {
	Config defaultCfg("./configs/mouse.cfg");
	init(defaultCfg);
}
