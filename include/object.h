#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "keyboard.h"
#include "mouse.h"

class object {

	protected:
		float speed = 0.1;

	public:
		glm::mat4 matrix;

		std::vector <object*> children;
		object * parent = NULL;

		keyboard keyboardObj;
		keyboard &keybindings = keyboardObj;
		mouse mouseObj;

		object();
		object(object * parent);

		virtual void drawObject()=0;
		void draw();
		void drawChildren();
		void drawSurroundingGrid();
		void addChild(object *o);
		void setParent(object * parent);
		void setNoParent();

		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
		void scale(glm::vec3 scaleVec);

		glm::vec3 toWorld(glm::vec3 objVec);
		glm::vec3 toObject(glm::vec3 worldVec);
		glm::vec3 toObject(object * fromObj, glm::vec3 fromObjVec);
		glm::mat4 transformationMatrix(object * obj2);
		glm::mat4 transformationMatrix();
	
		void virtual processKeyboardInput(bool pressedKeys[256], int x, int y);
		void moveKeys(bool pressedKeys[256]);
		void rotateLeftKeys(bool pressedKeys[256]);
		void rotateUpKeys(bool pressedKeys[256]);

		void virtual processMouseMove(glm::vec2 delta);
		void rotateMouse(glm::vec2 delta);

		bool isColiding(object * obj);

};

#endif
