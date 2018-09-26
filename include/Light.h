#ifndef SENSEFX_LIGHT_H
#define SENSEFX_LIGHT_H

#include "Object.h"
#include "glm/glm.hpp"
#include <string>
#include <map>

class Light : public Object {

	private:
		/* Ima samo 7 Light konstanti (max 7 svetala) */
		static inline int lightConstants[32];
		static inline int lightCounter = 0;
		int lightID;

	public:
		glm::vec4 position;
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 ambient;

		Light();
		Light(std::string lightPath);

		void init();
		void initLightConstants();

		float * getAmbient();
		float * getDiffuse();
		float * getSpecular();
		float * getPosition();

		void setPosition(glm::vec3);

		void loadLight(std::string lightPath);
		void enable();
		void disable();

		static std::map<std::string, Light> importAll(std::string dirPath);
};

#endif //LIGHT_H
