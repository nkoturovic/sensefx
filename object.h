#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "keyboard.h"

class object {

	protected:
		float speed = 0.01;
		float keyRotationSpeed = 2;

	public:
		keyboard keybindings;

		glm::mat4 matrix;
		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
		
		std::vector <object*> children;
		object * parent = NULL;

		virtual void drawObject()=0;
		object();
		object(object * parent);
		void draw();
		void drawChildren();
		void addChild(object *o);
		void virtual processKeyboardInput(bool pressedKeys[256], int x, int y);
		glm::mat4 transformationMatrix();

		void moveKeys(bool pressedKeys[256]);
		void rotateLeftKeys(bool pressedKeys[256]);
		void rotateUpKeys(bool pressedKeys[256]);

		void setParent(object * parent);
		void setNoParent();
};

#endif
