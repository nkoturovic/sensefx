#ifndef SENSEFX_DRAWABLE_OBJECT_H
#define SENSEFX_DRAWABLE_OBJECT_H

#include "Object.h"

class DrawableObject : public virtual Object {

	public:
		/* Ovaj metod mora da ima svaki drawable
		 * objekat, to predstavlja njegovu graficku
		 * reprezentaciju */
		virtual void drawObject()=0;

		void draw();
		void drawChildren();
		void drawSurroundingGrid();
};

#endif //DRAWABLE_OBJECT
