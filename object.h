#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "keyboard.h"

class object {

	public:
		keyboard keybindings;

		glm::mat4 matrix;
		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
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
