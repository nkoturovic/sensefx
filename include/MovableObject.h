#ifndef SENSEFX_MOVEABLE_H
#define SENSEFX_MOVEABLE_H

#include "Object.h"
#include "TransformableObject.h"
#include "ColidableObject.h"
#include "Keyboard.h"
#include "Mouse.h"

class MovableObject : public TransformableObject, public ColidableObject {

	private:
		/* Velocity je trenutna brzina objekta,
		 * a speed koliko se inace objekat brzo krece */
		glm::vec3 velocity;

		float speed = 0.25;
		float jump = 0.15;

	public:

		Keyboard keyboardObj;
		Keyboard &keybindings = keyboardObj;
		Mouse mouseObj;

		MovableObject(float speed, float jump);
		MovableObject();

		/* Svaki metod koji je movable mora da implementira
		 * procesuiranje zahteva poslatih sa tastature */
		void virtual processKeyboardInput(bool pressedKeys[256], int x, int y)=0;

		/* Pomocne funkcije koje se mogu koristiti u
		 * nasledjenom virtual processKeyboardInput metodu */
	 	void move(glm::vec3 moveVector);
		void moveKeys(bool pressedKeys[256]);
		void rotateLeftKeys(bool pressedKeys[256]);
		void rotateUpKeys(bool pressedKeys[256]);
		void jumpKeys(bool pressedKeys[256]);

		/* Svaki metod koji je movable mora da implementira
		 * procesuiranje zahteva poslatih sa misa */
		void virtual processMouseMove(glm::vec2 delta)=0;

		/* Pomocne funkcije koje se mogu koristiti u
		 * nasledjenom virtual processMouseMove metodu */
		void rotateMouse(glm::vec2 delta);

		void addToVelocity(glm::vec3 addVec);
		void setVelocity(glm::vec3 velocity);
		glm::vec3 getVelocity();
};

#endif //MOVABLE
