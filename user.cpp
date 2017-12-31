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
	/* Rotitanje objekta ijkl */
	rotateUpKeys(pressedKeys);
}

void user::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	moveKeys(pressedKeys);
	rotateLeftKeys(pressedKeys);

	this->userHead.processKeyboardInput(pressedKeys, x, y);
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
