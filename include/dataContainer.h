#ifndef SENSEFX_CONTAINER_H
#define SENSEFX_CONTAINER_H

#include <vector>
#include <map>
#include "config.h"
#include "object.h"
#include "camera.h"

class dataContainer {
	public:
		/* Zamena za globalne promenljive, klasa
		 * container sadrzi liste objekata 
		 * koje treba proslediti funkcijama on_*
		 * da ih obrade. */

		/* configs map */
		std::map<std::string, config> configs;

		std::vector <object*> toDisplay;
		std::vector <object*> toKeyboard; 
		std::vector <object*> toMouseMove;

		camera * activeCamera;

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

		glm::vec2 mousePosition;
};

#endif //SENSEFX_CONTAINER_H
