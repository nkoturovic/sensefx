#ifndef SENSEFX_MATERIAL_H
#define SENSEFX_MATERIAL_H

#include "glm/glm.hpp"

#include <string>
#include <map>

class Material {

	private:
		glm::vec4 ambientCoefs;
		glm::vec4 diffuseCoefs;
		glm::vec4 specularCoefs;
		glm::vec4 emissionCoefs;
		float shininess;

	public:
		Material();
		Material(std::string materialPath);

		float * getAmbientCoefs();
		float * getDiffuseCoefs();
		float * getSpecularCoefs();
		float * getEmissionCoefs();
		float getShininess();

		glm::vec4 getAmbientCoefsVec4();
		glm::vec4 getDiffuseCoefsVec4();
		glm::vec4 getSpecularCoefsVec4();
		glm::vec4 getEmissionCoefsVec4();

		void setAmbientCoefs(glm::vec4 ambientCoefs);
		void setDiffuseCoefs(glm::vec4 diffuseCoefs);
		void setSpecularCoefs(glm::vec4 specularCoefs);
		void setEmissionCoefs(glm::vec4 emissionCoefs);
		void setShininess(float shininess);

		void loadMaterial(std::string materialPath);
		static std::map <std::string, Material> importAll(std::string dirPath);
};

#endif //MATERIAL_H
