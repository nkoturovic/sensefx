#ifndef SENSEFX_MOUSE_H
#define SENSEFX_MOUSE_H

#include "config.h"

class mouse {

	private:
		void init(config mouseConfig);

	public:
		float sensitivity;

		mouse(config mouseConfig);
		mouse();
};

#endif
