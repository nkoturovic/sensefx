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
		std::vector <Object*> objectsToAnimation;
		std::vector <Text> textToScreenVec;
	
		Camera * activeCamera = NULL;
		User * activeUser = NULL;

		bool pressedKeys[256];
		int keyPressedPositionX;
		int keyPressedPositionY;
		int keyReleasedPositionX;
		int keyReleasedPositionY;

		bool timersPaused = false;

		const int redisplayTimerId = 0;
		float redisplayTimerInterval = 10;

		const int mouseTimerId = 1;
		float mouseTimerInterval = 10;

		const int keyboardTimerId = 2;
		float keyboardTimerInterval = 10;

		const int gravityTimerId = 3;
		float gravityTimerInterval = 10;

		const int animationTimerId = 4;
		float animationTimerInterval = 10;

		const int fxTimerId = 5;
		float fxTimerInterval = 10;

		glm::vec2 mousePosition;

		/* Promenljive i metodi vezani za file exploring */
		std::vector <Object *> fxFiles;
		std::vector <Object *> alocatedFxObjects;

		std::string fxAlocatedDir;
		std::string fxCurrentDir;

		void deallocFx();
};

#endif //SENSEFX_CONTAINER_H
