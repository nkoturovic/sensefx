#include "Light.h"
#include "glm/gtc/type_ptr.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <regex>
#include <map>

namespace fs = std::experimental::filesystem;

void Light::initLightConstants() {
	Light::lightConstants[0] = GL_LIGHT0;
	Light::lightConstants[1] = GL_LIGHT1;
	Light::lightConstants[2] = GL_LIGHT2;
	Light::lightConstants[3] = GL_LIGHT3;
	Light::lightConstants[4] = GL_LIGHT4;
	Light::lightConstants[5] = GL_LIGHT5;
	Light::lightConstants[6] = GL_LIGHT6;
	Light::lightConstants[7] = GL_LIGHT7;
}

void Light::init() {
	this->lightID = Light::lightCounter;

	if (lightCounter == 0) {
		initLightConstants();
	}

	if (lightID < 7) {
		Light::lightCounter++;
	} else if (lightID >= 7) {
		//std::cout << "Maximum lights reached" << std::endl;
	} else {
		std::cout << "Unknown behavior LIGHT constructor" << std::endl;
	}
}

Light::Light() {
	//this->init(); // ? Da li
}

Light::Light(std::string lightPath) {
	this->init();
	this->loadLight(lightPath);
}

float * Light::getAmbient() {
	return glm::value_ptr(this->ambient);
}

float * Light::getDiffuse() {
	return glm::value_ptr(this->diffuse);

}

float * Light::getSpecular() {
	return glm::value_ptr(this->specular);
}

float * Light::getPosition() {
	return glm::value_ptr(this->position);
}

void Light::loadLight(std::string lightPath) {

    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 ambient;
    glm::vec4 position;

    std::ifstream f(lightPath);

    for (std::string line; getline(f, line);) {
	    if (line[0] == 'd') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &diffuse.x, &diffuse.y, &diffuse.z, &diffuse.w);
	    } else if (line[0] == 'a') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &ambient.x, &ambient.y, &ambient.z, &ambient.w);
	    } else if (line[0] == 'p') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &position.x, &position.y, &position.z, &position.w);
	    } else if (line[0] == 's') {
		    const char * toscan = &line.c_str()[1];
		    sscanf(toscan ,"%f%f%f%f", &specular.x, &specular.y, &specular.z, &specular.w);
	    }
    }
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->position = position;
	this->specular = specular;
}


std::map<std::string, Light> Light::importAll(std::string dirPath) {
	std::map<std::string, Light> lightMap;

	const std::regex regLight(".*?([^\\/]+)\\.lgh", std::regex_constants::icase);
	std::smatch match;
	std::string lightName;

	fs::path dirPathp = fs::path(dirPath);

	for (auto &p : fs::directory_iterator(dirPathp)) {
		fs::path filePathp = fs::u8path(p);
		std::string filePath = filePathp.u8string();

		if (std::regex_search(filePath, match, regLight)) {
		    lightName = match[1];
		    Light lig(filePath);
		    lightMap[lightName] = lig;
		}
	}

	return lightMap;
}

void Light::enable() {
	int lightConstant = Light::lightConstants[lightID];
	glEnable(lightConstant);
	glLightfv(lightConstant, GL_POSITION, this->getPosition());
	glLightfv(lightConstant, GL_AMBIENT, this->getAmbient());
	glLightfv(lightConstant, GL_DIFFUSE, this->getDiffuse());
	glLightfv(lightConstant, GL_SPECULAR, this->getSpecular());
}


void Light::disable() {
	glDisable(lightConstants[lightID]);
}

void Light::setPosition(glm::vec3 position) {
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}
