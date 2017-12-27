#include "object.h"
#include "vec.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>

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
	glMultMatrixf(matrix.elements);
	drawObject();
	drawChildren();
}

void object::drawChildren() {
	for_each (children.begin(), children.end(), [] (object *o) {
		o->draw();
	});
}

void object::rotate(float degrees, vec3 aroundVec) {
	glPushMatrix();
	glLoadMatrixf(matrix.elements);
	glRotatef(degrees, aroundVec.x, aroundVec.y, aroundVec.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix.elements);
	glPopMatrix();
} 

void object::translate(vec3 translateVec) {
	glPushMatrix();
	glLoadMatrixf(matrix.elements);
	glTranslatef(translateVec.x, translateVec.y, translateVec.z);

	/* Parce koda ispod proverava da li je neka od vrednosti NAN,
	 * tacnije u poslednjem redu matrice jer je imao bug kada se
	 * pritisne vise tastera odjednom */
	mat4 temp;
	glGetFloatv(GL_MODELVIEW_MATRIX, temp.elements);

	// Da li nije NAN?
	if (!(temp.matrix[3][0] != temp.matrix[3][0]))
		matrix = temp;

	glPopMatrix();
}

void object::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	vec3 moveDirection(0,0,0);

	bool shouldMove = false;

	/* Kretanje napred,nazad,levo,desno (komb.) */
	if (pressedKeys[keybindings.moveForwardKey]) {
		moveDirection+=vec3(0,0,-1);
		shouldMove = true;
	}
	if (pressedKeys[keybindings.moveBackKey]) {
		moveDirection+=vec3(0,0,1);
		shouldMove = true;
	}
	if (pressedKeys[keybindings.moveLeftKey]) {
		moveDirection+=vec3(-1,0,0);
		shouldMove = true;
	}
	if (pressedKeys[keybindings.moveRightKey]) {
		moveDirection+=vec3(1,0,0);
		shouldMove = true;
	}

	moveDirection.normalizeSelf();

	if (shouldMove) {
		this->translate(moveDirection*speed);
	}
}

