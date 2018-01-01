#ifndef SENSEFX_VARIOUS_OBJECTS_H
#define SENSEFX_VARIOUS_OBJECTS_H

#include "object.h"

class chessFloor : public object {

	public:
		int numOfUnits=8;

		chessFloor(object * parent, int numOfUnits);
		chessFloor(object * parent);
		chessFloor(int numOfUnits);
		chessFloor();

		void drawObject();
		void drawChessSingleTexture();
};

class triangleFloor : public object {

	public:
		int numOfUnits=8;

		triangleFloor(object * parent, int numOfUnits);
		triangleFloor(object * parent);
		triangleFloor(int numOfUnits);
		triangleFloor();

		void drawObject();
		void drawtriangleSingleTexture();
};


#endif // SENSEFX_VARIOUS_OBJECTS_H
