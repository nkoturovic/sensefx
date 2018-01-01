#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "keyboard.h"
#include "mouse.h"

class object {

	protected:
		float speed = 0.01;

	public:
		keyboard keyboardObj;
		keyboard &keybindings = keyboardObj; 
		mouse mouseObj;

		glm::mat4 matrix;
		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
		void scale(glm::vec3 scaleVec);
		
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

		void virtual processMouseMove(glm::vec2 delta);
		void rotateMouse(glm::vec2 delta);

		void setParent(object * parent);
		void setNoParent();

		glm::vec3 toWorld(glm::vec3 obj);
		glm::vec3 toObject(glm::vec3 world);


};

#endif
