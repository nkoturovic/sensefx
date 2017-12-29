#ifndef SENSEFX_AXIS_H
#define SENSEFX_AXIS_H

#include "object.h"

class axis : public object {

	private:
		int numOfUnits = 5;

	public:

		axis(object * parent, int numOfUnits);
		axis(int numOfUnits);

		void drawObject();
};

#endif
