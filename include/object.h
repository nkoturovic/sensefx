#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "keyboard.h"
#include "mouse.h"

class object {

	protected:
		float speed = 0.1;
		std::vector <object*> checkColisionList;

	public:
		glm::vec3 velocity; // Za sada belezi samo gravitaciju (po y)
		glm::mat4 matrix;


		std::vector <object*> children;
		object * parent = NULL;

		keyboard keyboardObj;
		keyboard &keybindings = keyboardObj;
		mouse mouseObj;

		object();
		object(object * parent);

		virtual void drawObject()=0;
		void draw();
		void drawChildren();
		void drawSurroundingGrid();
		void addChild(object *o);
		void setParent(object * parent);
		void setNoParent();

		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
		void scale(glm::vec3 scaleVec);

		/* Metodi point samo tacku menjaju, bez da od nje se oduzme 
		 * polozaj koordinatnog pocetka (time bi se dobio
		 * identican vektor u nasem sistemu) - a ovde se 
		 * dobija polozaj "udajlene tacke" umesto bliskog
		 * vektora */
		glm::vec3 pointToWorldSys(glm::vec3 objVec);
		glm::vec3 pointToObjectSys(glm::vec3 worldVec);
		glm::vec3 pointToObjectSys(object * fromObj, glm::vec3 fromObjVec);
		glm::mat4 transformationMatrix(object * obj2);
		glm::mat4 transformationMatrix();

		/* Funkcije ispod "analogno kopiraju vektor!!"
		 * pogledaj u isto mesto u koje ja gledam ume-
		 * sto da gledas u mene (moju tacku) !!!
		 * (uzima se u obzir koordinatni pocetak) */

		/* Maksima - Pogledajte svi u istom pravcu umesto
		 * pogledajte svi u istu tacku!!!!!! */
		glm::vec3 vecToWorldSys(glm::vec3 objVec);
		glm::vec3 vecToObjectSys(glm::vec3 worldVec);
		glm::vec3 vecToObjectSys(object * fromObj, glm::vec3 fromObjVec);
	
		void virtual processKeyboardInput(bool pressedKeys[256], int x, int y);
		void move(glm::vec3 moveVector);
		void moveKeys(bool pressedKeys[256]);
		void rotateLeftKeys(bool pressedKeys[256]);
		void rotateUpKeys(bool pressedKeys[256]);

		void virtual processMouseMove(glm::vec2 delta);
		void rotateMouse(glm::vec2 delta);

		bool isColiding(object * obj);
		void addToCheckColisionList(object * o);
		void removeFromCheckColisionList(object * o);
};

#endif
