#include "mouse.h"

void mouse::init(config mouseConfig) {
	sensitivity = stof(mouseConfig.getParameter("SENSITIVITY"));
}

mouse::mouse() {
	config defaultCfg("./configs/mouse.cfg");
	init(defaultCfg);
}
