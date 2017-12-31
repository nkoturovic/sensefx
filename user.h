#ifndef SENSEFX_USER_H
#define SENSEFX_USER_H

#include "object.h"
#include "camera.h"

class head : public object {
	public:
		head();
		camera firstPerson;
		void processKeyboardInput(bool pressedKeys[256], int x, int y);

		void drawObject();
};

class body : public object {
	public:
		void drawObject();
};



class user : public object {

	public:
		head userHead;
		body userBody;

		user(object * parent);
		user();

		void drawObject();
		void processKeyboardInput(bool pressedKeys[256], int x, int y);
		camera * fpsViewCamera();
};

#endif
