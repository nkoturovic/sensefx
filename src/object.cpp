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

#include <limits>

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
	/* Iscrtava mrezu (kocku) oko "prostora" objekata */
	// drawSurroundingGrid();
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

void object::scale(glm::vec3 scaleVec) {
	matrix = glm::scale(matrix, scaleVec);
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

glm::vec3 object::toObject(glm::vec3 worldVec) {
	glm::mat4 transMat = transformationMatrix();
	glm::mat4 inverse = glm::inverse(transMat);
	glm::vec4 worldVec4(worldVec.x, worldVec.y, worldVec.z, 1.0f);
	glm::vec4 resultVec4 = inverse*worldVec4;
	float w = resultVec4.w;

	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 object::toObject(object * fromObj, glm::vec3 fromObjVec) {
	glm::vec4 tmpVec4(fromObjVec.x, fromObjVec.y, fromObjVec.z, 1.0f);
	glm::vec4 resultVec4 = fromObj->transformationMatrix(this)*tmpVec4;
	float w = resultVec4.w;
	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 object::toWorld(glm::vec3 objVec) {
	glm::vec4 worldVec4(objVec.x, objVec.y, objVec.z, 1.0f);
	glm::mat4 transMat = transformationMatrix();
	glm::vec4 resultVec4 = transMat*worldVec4;
	float w = resultVec4.w;

	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::mat4 object::transformationMatrix(object * obj2 ) {

	/* Mnozenje sa ovom daje world */
	glm::mat4 transMat1 = this->transformationMatrix();
	glm::mat4 transMat2 = glm::inverse(obj2->transformationMatrix());

	return transMat2*transMat1;
}

bool object::isColiding(object * obj) {

	/* Surrounding cube vertices */
	glm::vec4 vertices[] = {glm::vec4(-1.0f,-1.0f,-1.0f,1.0f), glm::vec4(-1.0f,-1.0f,1.0f,1.0f), glm::vec4(-1.0f,1.0f,-1.0f,1.0f), glm::vec4(-1.0f,1.0f,1.0f,1.0f),
 				glm::vec4(1.0f,-1.0f,-1.0f,1.0f), glm::vec4(1.0f,-1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,-1.0f,1.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f)};

	float maxFloat = std::numeric_limits<float>::max();
	glm::vec3 min(maxFloat,maxFloat,maxFloat);

	glm::mat4 transMat = this->transformationMatrix(obj);
	glm::vec4 tmp4;
	glm::vec3 tmp3;
	float tmpf;
	float w;
	
	for (int i=0; i<8; i++) {
		tmp4 = transMat*vertices[i];
		w = tmp4.w; 
		tmp3.x = tmp4.x/w;
		tmp3.y = tmp4.y/w;
		tmp3.z = tmp4.z/w;

		if ((tmpf = fabs(tmp3.x)) < min.x)
			min.x = tmpf;
		if ((tmpf = fabs(tmp3.y)) < min.y)
			min.y = tmpf;
		if ((tmpf = fabs(tmp3.z)) < min.z)
			min.z = tmpf;
	}

	if (min.x <= 1.0f && min.y <=1.0f && min.z <= 1.0f)
		return true;

	transMat = obj->transformationMatrix(this);

	for(int i=0; i<8; i++) {
		tmp4 = transMat*vertices[i];
		w = tmp4.w; 
		tmp3.x = tmp4.x/w;
		tmp3.y = tmp4.y/w;
		tmp3.z = tmp4.z/w;

		if ((tmpf = fabs(tmp3.x)) < min.x)
			min.x = tmpf;
		if ((tmpf = fabs(tmp3.y)) < min.y)
			min.y = tmpf;
		if ((tmpf = fabs(tmp3.z)) < min.z)
			min.z = tmpf;
	}

	if (min.x <= 1.0f && min.y <= 1.0f && min.z <= 1.0f)
		return true;

	return false;
}

void object::drawSurroundingGrid() {
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,-1,1);
		glVertex3f(1,-1,1);
		glVertex3f(1,-1,-1);
		glVertex3f(-1,-1,-1);

		glVertex3f(-1,1,-1);
		glVertex3f(-1,1,1);
		glVertex3f(1,1,1);
		glVertex3f(1,1,-1);
		glVertex3f(-1,1,-1);
	glEnd();

	glBegin(GL_LINES);

		glVertex3f(1,-1,-1);
		glVertex3f(1,1,-1);
		glVertex3f(-1,-1,1);
		glVertex3f(-1,1,1);
		glVertex3f(1,-1,1);
		glVertex3f(1,1,1);

	glEnd();
}
