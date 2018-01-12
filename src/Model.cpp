#include "Model.h"
#include "objloader.h"

#include <regex>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

Model::Model(std::string modelPath) {
	loadModel(modelPath);
}

Model::Model() {}

void Model::loadModel(std::string path) {
	loadOBJ(path.c_str(), this->vertices, this->textureCoordinates, this->normals);
}

std::map<std::string, Model> Model::importAll(std::string dirPath) {

	std::map<std::string, Model> modelMap;

	const std::regex regConf(".*?([^\\/]+)\\.obj", std::regex_constants::icase);
	std::smatch match;
	std::string modelName;

	fs::path dirPathp = fs::path(dirPath);

	for (auto &p : fs::directory_iterator(dirPathp)) {
	    fs::path filePathp = fs::u8path(p);
	    std::string filePath = filePathp.u8string();

	    if (std::regex_search(filePath, match, regConf)) {
		modelName = match[1];

		Model mdl = Model(filePath);
		modelMap[modelName] = mdl;
	    }
	}

	return modelMap;

}
