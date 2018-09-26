#include "Material.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <regex>
#include <map>

namespace fs = std::experimental::filesystem;

Material::Material() {}

Material::Material(std::string materialPath) {
	this->loadMaterial(materialPath);
}


float * Material::getAmbientCoefs() {
	return glm::value_ptr(ambientCoefs);
}

float * Material::getDiffuseCoefs() {
	return glm::value_ptr(diffuseCoefs);
}

float * Material::getSpecularCoefs() {
	return glm::value_ptr(specularCoefs);
}

float * Material::getEmissionCoefs() {
	return glm::value_ptr(emissionCoefs);
}

float Material::getShininess() {
	return shininess;
}

void Material::loadMaterial(std::string materialPath) {

    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 ambient;
    glm::vec4 emission;
    float shininess;

    std::ifstream f(materialPath);

    for (std::string line; getline(f, line);) {
	    if (line[0] == 'd') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &diffuse.x, &diffuse.y, &diffuse.z, &diffuse.w);
	    } else if (line[0] == 'a') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &ambient.x, &ambient.y, &ambient.z, &ambient.w);
	    } else if (line[0] == 'e') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &emission.x, &emission.y, &emission.z, &emission.w);
	    } else if (line[0] == 's') {

		    if (line[1] == 'h') {
			    const char * toscan = &line.c_str()[2];
			    sscanf(toscan, "%f", &shininess);
		    } else {
			    const char * toscan = &line.c_str()[1];
			    sscanf(toscan ,"%f%f%f%f", &specular.x, &specular.y, &specular.z, &specular.w);
		    }
	    }
    }
	this->diffuseCoefs = diffuse;
	this->ambientCoefs = ambient;
	this->emissionCoefs = emission;
	this->specularCoefs = specular;
	this->shininess = shininess;
}

std::map<std::string, Material> Material::importAll(std::string dirPath) {
    std::map<std::string, Material> matMap;

    const std::regex regMat(".*?([^\\/]+)\\.mat", std::regex_constants::icase);
    std::smatch match;
    std::string matName;

    fs::path dirPathp = fs::path(dirPath);

    for (auto &p : fs::directory_iterator(dirPathp)) {
        fs::path filePathp = fs::u8path(p);
        std::string filePath = filePathp.u8string();

        if (std::regex_search(filePath, match, regMat)) {
            matName = match[1];
            Material mat(filePath);
            matMap[matName] = mat;
        }
    }

    return matMap;
}

glm::vec4 Material::getAmbientCoefsVec4() {
	return this->ambientCoefs;
}

glm::vec4 Material::getDiffuseCoefsVec4() {
	return this->diffuseCoefs;
}

glm::vec4 Material::getSpecularCoefsVec4() {
	return this->specularCoefs;
}

glm::vec4 Material::getEmissionCoefsVec4() {
	return this->emissionCoefs;
}

void Material::setAmbientCoefs(glm::vec4 ambientCoefs) {
	this->ambientCoefs = ambientCoefs;
}

void Material::setDiffuseCoefs(glm::vec4 diffuseCoefs) {
	this->diffuseCoefs = diffuseCoefs;
}

void Material::setSpecularCoefs(glm::vec4 specularCoefs) {
	this->specularCoefs = specularCoefs;
}

void Material::setEmissionCoefs(glm::vec4 emissionCoefs) {
	this->emissionCoefs = emissionCoefs;
}

void Material::setShininess(float shininess) {
	this->shininess = shininess;
}
