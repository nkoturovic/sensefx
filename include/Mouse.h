#ifndef SENSEFX_MOUSE_H
#define SENSEFX_MOUSE_H

#include "Config.h"

class Mouse {

	private:
		void init(Config mouseConfig);

	public:
		float sensitivity;

		Mouse(Config mouseConfig);
		Mouse();
};

#endif
