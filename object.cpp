#include "object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

object::object(object * parent) {
	if (parent != NULL)
		parent->addChild(this);
}

object::object() {
}

void object::addChild(object * o) {
	children.push_back(o);
}

void object::draw() {
	glMultMatrixf(glm::value_ptr(matrix));
	drawObject();
	drawChildren();
}

void object::drawChildren() {
	for_each (children.begin(), children.end(), [] (object *o) {
		o->draw();
	});
}

void object::rotate(float degrees, glm::vec3 aroundVec) {
	matrix = glm::rotate(matrix, glm::radians(degrees), aroundVec);
} 

void object::translate(glm::vec3 translateVec) {
	matrix = glm::translate(matrix, translateVec);
}

void object::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	/* Kretanje napred,nazad,levo,desno (komb.) */
	
	float moveForward = 0.0f, moveLeft = 0.0f;

	if (pressedKeys[keybindings.moveForwardKey]) 
		moveForward-=1.0f;

	if (pressedKeys[keybindings.moveBackKey]) 
		moveForward+=1.0f;

	if (pressedKeys[keybindings.moveLeftKey]) 
		moveLeft-=1.0f;

	if (pressedKeys[keybindings.moveRightKey]) 
		moveLeft+=1.0f;

	if (moveLeft || moveForward)
		this->translate(glm::vec3(moveLeft*speed, 0, moveForward*speed));



	/* Rotitanje objekta ijkl */

	float rotateUp = 0.0f, rotateLeft=0.0f;
	
	if (pressedKeys[keybindings.rotateUpKey])
		rotateUp += 1.0f;

	if (pressedKeys[keybindings.rotateDownKey]) 
		rotateUp -= 1.0f;

	if (pressedKeys[keybindings.rotateLeftKey])
		rotateLeft += 1.0f;

	if (pressedKeys[keybindings.rotateRightKey])
		rotateLeft -= 1.0f;
	

	if (rotateUp)
		this->rotate(rotateUp, glm::vec3(1,0,0));

	if (rotateLeft)
		this->rotate(rotateLeft, glm::vec3(0,1,0));

}

