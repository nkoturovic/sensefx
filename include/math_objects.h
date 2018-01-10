#ifndef SENSEFX_MATH_OBJECTS_H
#define SENSEFX_MATH_OBJECTS_H

#include "Object.h"
#include "DrawableObject.h"
#include "TransformableObject.h"

class Axis : public DrawableObject, public TransformableObject {

	private:
		int numOfUnits = 5;

	public:

		Axis(Object * parent, int numOfUnits);
		Axis(int numOfUnits);
		Axis();

		void drawObject();
};


class Grid : public DrawableObject, public TransformableObject {

	private:
		int numOfUnits = 5;

	public:
		bool fill = false;

		Grid(Object * parent, int numOfUnits);
		Grid(int numOfUnits);
		Grid();

		void drawObject();
};

class WireCube : public DrawableObject, public TransformableObject {

	public:
		WireCube(Object * parent);
		WireCube();

		void drawObject();
};


class Plane : public DrawableObject, public TransformableObject {

	public:
		bool drawMaterial = true;
		Plane(Object * parent);
		Plane();

		void drawObject();
};


#endif // SENSEFX_MATH_OBJECTS_H
