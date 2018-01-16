#ifndef SENSEFX_USER_H
#define SENSEFX_USER_H

#include "Object.h"
#include "Camera.h"
#include "MovableObject.h"

/* User u sebi sadrzi glavu koja moze da se rotira samo vertikalno
 * glava je user-ovo dete i sve horizontalne rotacije user-a uticace na glavu. */

/*
-------------
|   -----   |
|   |HEAD|  |
|   -----   |
|           |
|    USER   |
|           |
|___________|
*/


class Head : public MovableObject {

	private:
		float headRotated=0.0f;

	public:
		Head();
		Camera firstPerson;
		void processKeyboardInput(bool pressedKeys[256], int x, int y);
		void processMouseMove(glm::vec2 delta);

		void drawObject();
};


class User : public MovableObject {

	private:
		glm::vec3 headToBodyRatio = glm::vec3(0.275f,0.1f,0.275f);
		glm::vec3 userToWorldRatio = glm::vec3(0.1f, 0.275f, 0.1f);
		bool isFlashLightOn = false;

	public:
		Head userHead;

		User(Object * parent, float speed, float jump);
		User(float speed, float jump);
		User();

		void drawObject();
		void processKeyboardInput(bool pressedKeys[256], int x, int y);
		void processMouseMove(glm::vec2 delta);
		void toggleFlashLight(bool pressedKeys[256]);

		Camera * fpsViewCamera();
};

#endif
