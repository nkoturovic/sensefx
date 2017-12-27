#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include "vec.h"
#include "mat.h"
#include "keyboard.h"

class object {

	public:
		keyboard keybindings;

		mat4 matrix;
		void translate(vec3 translateVec);
		void rotate(float degrees, vec3 aroundVec);
		float speed = 0.1;

		std::vector <object*> children;

		virtual void drawObject()=0;
		object();
		object(object * parent);
		void draw();
		void drawChildren();
		void addChild(object *o);
		void processKeyboardInput(bool pressedKeys[256], int x, int y);
};

#endif
