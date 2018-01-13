#ifndef SENSEFX_DRAWABLE_OBJECT_H
#define SENSEFX_DRAWABLE_OBJECT_H

#include "Object.h"

class DrawableObject : public virtual Object {

	protected:
		glm::vec4 color = glm::vec4(1,1,1,1);

	public:

		DrawableObject();
		DrawableObject(Object * parent);
		DrawableObject(Object * parent, glm::vec4 color);
		DrawableObject(glm::vec4 color);

		/* Ovaj metod mora da ima svaki drawable
		 * objekat, to predstavlja njegovu graficku
		 * reprezentaciju */
		virtual void drawObject()=0;

		void draw();
		void drawChildren();
		void drawSurroundingGrid();

		glm::vec4 getColor();
		void setColor(glm::vec4 color);
};

#endif //DRAWABLE_OBJECT
