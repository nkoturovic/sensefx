#ifndef SENSEFX_MATH_OBJECTS_H
#define SENSEFX_MATH_OBJECTS_H

#include "object.h"
#include "drawable.h"
#include "transformable.h"

class axis : public drawable, public transformable {

	private:
		int numOfUnits = 5;

	public:

		axis(object * parent, int numOfUnits);
		axis(int numOfUnits);
		axis();

		void drawObject();
};


class grid : public drawable, public transformable {

	private:
		int numOfUnits = 5;

	public:
		bool fill = false;

		grid(object * parent, int numOfUnits);
		grid(int numOfUnits);
		grid();

		void drawObject();
};

class wireCube : public drawable, public transformable {

	public:
		wireCube(object * parent);
		wireCube();

		void drawObject();
};


class plane : public drawable, public transformable {

	public:
		bool drawMaterial = true;
		plane(object * parent);
		plane();

		void drawObject();
};


#endif // SENSEFX_MATH_OBJECTS_H
