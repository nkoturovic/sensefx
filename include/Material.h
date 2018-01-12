#ifndef SENSEFX_MATERIAL_H
#define SENSEFX_MATERIAL_H

#include <glm/glm.hpp>

#include <string>
#include <map>

class Material {

	public:
		glm::vec4 ambientCoefs;
		glm::vec4 diffuseCoefs;
		glm::vec4 specularCoefs;
		glm::vec4 emissionCoefs;
		float shininess;

		Material();
		Material(std::string materialPath);

		float * getAmbientCoefs();
		float * getDiffuseCoefs();
		float * getSpecularCoefs();
		float * getEmissionCoefs();
		float getShininess();

		void loadMaterial(std::string materialPath);
		static std::map <std::string, Material> importAll(std::string dirPath);
};

#endif //MATERIAL_H
