#ifndef SENSEFX_MATH_OBJECTS_H
#define SENSEFX_MATH_OBJECTS_H

#include "object.h"

class axis : public object {

	private:
		int numOfUnits = 5;

	public:

		axis(object * parent, int numOfUnits);
		axis(int numOfUnits);
		axis();

		void drawObject();
};


class grid : public object {

	private:
		int numOfUnits = 5;

	public:
		bool fill = false;

		grid(object * parent, int numOfUnits);
		grid(int numOfUnits);
		grid();

		void drawObject();
};

class wireCube : public object {

	public:
		wireCube(object * parent);
		wireCube();

		void drawObject();
};


class plane : public object {

	public:
		bool drawMaterial = true;
		plane(object * parent);
		plane();

		void drawObject();
};



#endif // SENSEFX_MATH_OBJECTS_H
