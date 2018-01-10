#ifndef SENSEFX_VARIOUS_OBJECTS_H
#define SENSEFX_VARIOUS_OBJECTS_H

#include "Object.h"
#include "DrawableObject.h"

class ChessFloor : public DrawableObject {

	public:
		int numOfUnits=8;

		ChessFloor(Object * parent, int numOfUnits);
		ChessFloor(Object * parent);
		ChessFloor(int numOfUnits);
		ChessFloor();

		void drawObject();
		void drawChessSingleTexture();
};

class TriangleFloor : public DrawableObject {

	public:
		int numOfUnits=8;

		TriangleFloor(Object * parent, int numOfUnits);
		TriangleFloor(Object * parent);
		TriangleFloor(int numOfUnits);
		TriangleFloor();
                
		void drawObject();
		void drawtriangleSingleTexture();
};


#endif // SENSEFX_VARIOUS_OBJECTS_H
