#ifndef SENSEFX_DRAWABLE_H
#define SENSEFX_DRAWABLE_H

#include "object.h"

class drawable : public virtual object {

	public:
		virtual void drawObject()=0;
		void draw();
		void drawChildren();
		void drawSurroundingGrid();
};

#endif //DRAWABLE
