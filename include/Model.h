#ifndef SENSEFX_MODEL_H
#define SENSEFX_MODEL_H

#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <map>

class Model {

	public:
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec3> normals;
		std::vector <glm::vec2> textureCoordinates;

		Model(std::string modelPath);
		Model();

		void loadModel(std::string path);
		static std::map<std::string, Model> importAll(std::string dirPath);
		
};

#endif //Model
