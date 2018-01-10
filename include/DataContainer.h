#ifndef SENSEFX_CONTAINER_H
#define SENSEFX_CONTAINER_H

#include <vector>
#include <map>
#include "Config.h"
#include "Object.h"
#include "Camera.h"

class DataContainer {
	public:
		/* Zamena za globalne promenljive, klasa
		 * container sadrzi liste objekata 
		 * koje treba proslediti funkcijama on_*
		 * da ih obrade. */

		/* Configs map */
		std::map<std::string, Config> configs;

		std::vector <Object*> objectsToDisplay;
		std::vector <Object*> objectsToKeyboard; 
		std::vector <Object*> objectsToMouseMove;
		std::vector <Object*> objectsToGravity;

		Camera * activeCamera;

		bool pressedKeys[256];
		int keyPressedPositionX;
		int keyPressedPositionY;
		int keyReleasedPositionX;
		int keyReleasedPositionY;

		const int redisplayTimerId = 0;
		float redisplayTimerInterval = 10;

		const int mouseTimerId = 1;
		float mouseTimerInterval = 10;

		const int keyboardTimerId = 2;
		float keyboardTimerInterval = 10;

		const int gravityTimerId = 3;
		float gravityTimerInterval = 10;

		glm::vec2 mousePosition;
};

#endif //SENSEFX_CONTAINER_H
