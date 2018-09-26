#include "User.h"
#include "Object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "DataContainer.h"

#include "glm/glm.hpp"

extern DataContainer globalData;

Head::Head() {
	this->firstPerson.attachedTo = this;
}

User::User(Object * parent, float speed, float jump) : Object(parent), MovableObject(speed,jump) {
	scale(userToWorldRatio);
	userHead.setParent(this);
	userHead.translate(glm::vec3(0.0f,0.84f,0.0f));
	userHead.scale(headToBodyRatio);
}

User::User(float speed, float jump) : User(NULL, speed, jump) {}
User::User() : User(NULL, stof(globalData.configs["user"].getParameter("SPEED")),
		stof(globalData.configs["user"].getParameter("JUMP"))) {}


void Head::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	/* Data funkcija proverava da li je vertikalna rotacija
	 * veca od 90 stepeni, ako jeste nece se rotirati dalje
	 * u suprotnom hoce */
	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	float oldHeadRotated = headRotated;

	if (pressedKeys[keybindings.rotateUpKey]) {

		headRotated += keyRotationSensitivity;

		if (headRotated > 89.0f) {
			headRotated = oldHeadRotated;
			return;
		}
	}

	if (pressedKeys[keybindings.rotateDownKey]) {

		headRotated -= keyRotationSensitivity;

		if (headRotated < -89.0f) {
			headRotated = oldHeadRotated;
			return;
		}
	}

	rotateUpKeys(pressedKeys);
}

void Head::processMouseMove(glm::vec2 delta) {

	float mouseRotationSensitivity = this->mouseObj.sensitivity;

	float oldHeadRotated = headRotated;
	headRotated += delta.y * mouseRotationSensitivity;

	if (headRotated > 89.0f) {
		headRotated = oldHeadRotated;
		return;
	}

	if (headRotated < -89.0f) {
		headRotated = oldHeadRotated;
		return;
	}

	rotateMouse(glm::vec2(0.0f,delta.y));
}

void User::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	moveKeys(pressedKeys);
	rotateLeftKeys(pressedKeys);
	jumpKeys(pressedKeys);
	toggleFlashLight(pressedKeys);

	this->userHead.processKeyboardInput(pressedKeys, x, y);
}

void User::processMouseMove(glm::vec2 delta) {
	rotateMouse(glm::vec2(delta.x,0.0f));
	this->userHead.processMouseMove(glm::vec2(0.0f, delta.y));
}

void Head::drawObject(){
}


void User::drawObject() {
}

Camera * User::fpsViewCamera() {
	return &this->userHead.firstPerson;
}

void User::toggleFlashLight(bool pressedKeys[256]) {
	char flashKey = globalData.configs["keyboard"].getParameter("TOGGLE_FLASHLIGHT").c_str()[0];

	if (pressedKeys[(int) flashKey]) {
		if (this->isFlashLightOn == false) {
			globalData.lights["user_flashlight"].enable();
			isFlashLightOn = true;
		} else {
			globalData.lights["user_flashlight"].disable();
			isFlashLightOn = false;
		}

		globalData.pressedKeys[(int) flashKey] = false;
	}
}
