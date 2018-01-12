#include "Texture.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <experimental/filesystem>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace fs = std::experimental::filesystem;

void Texture2D::init(Image * image, bool repeatS, bool repeatT) {

	this->repeatS = repeatS;
	this->repeatT = repeatS;

	glGenTextures(1, &textureName);

	glBindTexture(GL_TEXTURE_2D, textureName);

	if (repeatS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);


	if (repeatT) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 else 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			 image->width, image->height, 0,
			 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	image_done(image);
}

Texture2D::Texture2D(Image * image, bool repeatS, bool repeatT) {
	this->init(image, repeatS, repeatT);
}

Texture2D::Texture2D(Image * image) : Texture2D(image, true, true) {}

Texture2D::Texture2D(std::string path, bool repeatS, bool repeatT) {
	Image * image = image_init(0, 0);
	image_read(image, (char *) path.c_str());
	this->init(image, repeatS, repeatT);
}

Texture2D::Texture2D(std::string path) : Texture2D(path, true, true) { }

Texture2D::Texture2D() {
	//Image * image = image_init(0,0);
	//init(image, false, false);
}

GLuint Texture2D::getTextureName() {
	return this->textureName;
}

bool Texture2D::isRepeatingS() {
	return this->repeatS;
}

bool Texture2D::isRepeatingT() {
	return this->repeatT;
}

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, this->textureName);
}

void Texture2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::remove() {
	this->unbind();
	glDeleteTextures(1, &textureName);
}

std::map<std::string, Texture2D> Texture2D::importAll(std::string dirPath) {
    std::map<std::string, Texture2D> textureMap;

    const std::regex regConf(".*?([^\\/]+)\\.bmp", std::regex_constants::icase);
    std::smatch match;
    std::string textureName;

    fs::path dirPathp = fs::path(dirPath);

    for (auto &p : fs::directory_iterator(dirPathp)) {
        fs::path filePathp = fs::u8path(p);
        std::string filePath = filePathp.u8string();

        if (std::regex_search(filePath, match, regConf)) {
            textureName = match[1];

            Texture2D tex = Texture2D(filePath);
            textureMap[textureName] = tex;
        }
    }

    return textureMap;
}
