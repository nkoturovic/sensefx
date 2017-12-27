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

		camera * activeCamera;
		object * cameraRelativeTo; 

		bool pressedKeys[256];
		int keyPressedPositionX;
		int keyPressedPositionY;
		int keyReleasedPositionX;
		int keyReleasedPositionY;
};

#endif //SENSEFX_CONTAINER_H
