#include "user.h"
#include "object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/glm.hpp>

head::head() {
	this->firstPerson.attachedTo = this;
}

void head::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	/* Data funkcija proverava da li je vertikalna rotacija
	 * veca od 90 stepeni, ako jeste nece se rotirati dalje
	 * u suprotnom hoce */
	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	float oldHeadRotated = headRotated;

	if (pressedKeys[keybindings.rotateUpKey]) {

		headRotated += keyRotationSensitivity;

		if (headRotated > 90.0f) {
			headRotated = oldHeadRotated;
			return;
		}
	}

	if (pressedKeys[keybindings.rotateDownKey]) {

		headRotated -= keyRotationSensitivity;

		if (headRotated < -90.0f) {
			headRotated = oldHeadRotated;
			return;
		}
	}

	rotateUpKeys(pressedKeys);
}

void head::processMouseMove(glm::vec2 delta) {

	float mouseRotationSensitivity = this->mouseObj.sensitivity;

	float oldHeadRotated = headRotated;
	headRotated += delta.y * mouseRotationSensitivity;

	if (headRotated > 90.0f) {
		headRotated = oldHeadRotated;
		return;
	}

	if (headRotated < -90.0f) {
		headRotated = oldHeadRotated;
		return;
	}

	rotateMouse(glm::vec2(0.0f,delta.y));
}

void body::processMouseMove(glm::vec2 delta) { };

void user::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	moveKeys(pressedKeys);
	rotateLeftKeys(pressedKeys);

	this->userHead.processKeyboardInput(pressedKeys, x, y);
}

void user::processMouseMove(glm::vec2 delta) {
	rotateMouse(glm::vec2(delta.x,0.0f));
	this->userHead.processMouseMove(glm::vec2(0.0f, delta.y));
}

void head::drawObject(){
	glColor3f(1,0,0);
	glutWireCube(0.2);
}
void body::drawObject(){
	glColor3f(0,1,0);
	glutWireCube(0.8);
}

user::user(object * parent) : object(parent) {
	userBody.setParent(this);
	userHead.setParent(this); 
}

user::user() : user(NULL) {}

void user::drawObject() {

	glPointSize(5);
	glColor3f(0,0,1);

	glBegin(GL_POINTS);
		glVertex3f(0,0,0);
	glEnd();
}

camera * user::fpsViewCamera() {
	return &this->userHead.firstPerson;
}
