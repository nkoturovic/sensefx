#ifndef SENSEFX_CONTAINER_H
#define SENSEFX_CONTAINER_H

#include <vector>
#include <map>

#include "Config.h"
#include "Object.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Light.h"
#include "Material.h"
#include "User.h"
#include "Text.h"

class DataContainer {
	public:
		/* Zamena za globalne promenljive, klasa
		 * container sadrzi liste objekata 
		 * koje treba proslediti funkcijama on_*
		 * da ih obrade. */

		/* Screen size (inicijalna vrednost je 800x600) */
		/* Ovo menja on_reshape funkcija */
		glm::vec2 screenSize = glm::vec2(800, 600);

		/* Configs map */
		std::map<std::string, Config> configs;
		std::map<std::string, Texture2D> textures;
		std::map<std::string, Model> models;
		std::map<std::string, Material> materials;
		std::map<std::string, Light> lights;

		std::vector <Object*> objectsToDisplay;
		std::vector <Object*> objectsToKeyboard; 
		std::vector <Object*> objectsToMouseMove;
		std::vector <Object*> objectsToGravity;
		std::vector <Text> textToScreenVec;

		std::vector <Object *> simColisionList;
		std::string simCurrentDir;

		Camera * activeCamera;
		User * activeUser;

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

		const int fxSimulationTimerId = 4;
		float fxSimulationTimerInterval = 10;

		glm::vec2 mousePosition;
		DataContainer& operator=(DataContainer rhs);
};

#endif //SENSEFX_CONTAINER_H
