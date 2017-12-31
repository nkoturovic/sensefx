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
	o->parent = this;
	children.push_back(o);
}

void object::draw() {
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(matrix));
	drawObject();
	drawChildren();
	glPopMatrix();
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

glm::mat4 object::transformationMatrix() {
	object * tmp_ptr = this;
	glm::mat4 result_matrix(1.0f);

	while (tmp_ptr != NULL) {
		result_matrix = tmp_ptr->matrix * result_matrix;
		tmp_ptr = tmp_ptr->parent;
	}

	return result_matrix;
}

void object::setNoParent() {
	setParent(NULL);
}

void object::setParent(object * parent) {

	this->parent = parent;

	if (parent != NULL)
		parent->addChild(this);
}

void object::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	/* Kretanje napred,nazad,levo,desno (komb.) */
	moveKeys(pressedKeys);

	/* Rotitanje objekta ijkl */
	rotateUpKeys(pressedKeys);
	rotateLeftKeys(pressedKeys);

}

void object::moveKeys(bool pressedKeys[256]) {

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
		this->translate(glm::normalize(glm::vec3(moveLeft, 0, moveForward))*speed);

}

void object::rotateUpKeys(bool pressedKeys[256]) {

	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	float rotateUp = 0.0f;
	
	if (pressedKeys[keybindings.rotateUpKey])
		rotateUp += keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateDownKey]) 
		rotateUp -= keyRotationSensitivity;
	
	if (rotateUp)
		this->rotate(rotateUp, glm::vec3(1,0,0));
}

void object::rotateLeftKeys(bool pressedKeys[256]) {

	float rotateLeft=0.0f;
	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateLeftKey])
		rotateLeft += keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateRightKey])
		rotateLeft -= keyRotationSensitivity;

	if (rotateLeft)
		this->rotate(rotateLeft, glm::vec3(0,1,0));
}

void object::processMouseMove(glm::vec2 delta) {
	rotateMouse(delta); 
}

void object::rotateMouse(glm::vec2 delta) {
	float mouseRotationSensitivity = mouseObj.sensitivity;
	this->rotate(delta.x*mouseRotationSensitivity, glm::vec3(0,1,0));
	this->rotate(delta.y*mouseRotationSensitivity, glm::vec3(1,0,0));
}
