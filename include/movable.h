#ifndef SENSEFX_MOVEABLE_H
#define SENSEFX_MOVEABLE_H

#include "object.h"
#include "transformable.h"
#include "colidable.h"

class movable : public transformable, public colidable {

	private:
		glm::vec3 velocity;

		float speed = 0.25;
		float jump = 0.15;

	public:

		movable(float speed, float jump);
		movable();

		keyboard keyboardObj;
		keyboard &keybindings = keyboardObj;
		mouse mouseObj;
		void virtual processKeyboardInput(bool pressedKeys[256], int x, int y)=0;
		void move(glm::vec3 moveVector);
		void moveKeys(bool pressedKeys[256]);
		void rotateLeftKeys(bool pressedKeys[256]);
		void rotateUpKeys(bool pressedKeys[256]);
		void jumpKeys(bool pressedKeys[256]);

		void virtual processMouseMove(glm::vec2 delta)=0;
		void rotateMouse(glm::vec2 delta);
		void addToVelocity(glm::vec3 addVec);
		void setVelocity(glm::vec3 velocity);
		glm::vec3 getVelocity();
};

#endif //DRAWABLE
