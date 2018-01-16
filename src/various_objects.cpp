#include "various_objects.h"
#include "Object.h"
#include "DataContainer.h"
#include "ast.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <regex>
#include <experimental/filesystem>

extern DataContainer globalData;

namespace fs = std::experimental::filesystem;

Room::Room() {
	init();
}

void Room::drawObject() {
}

void Room::setDimensions(glm::vec3 lwh) {
	this->setPositioningMatrix(glm::mat4(1.0f));
	this->scale(lwh/2.0f);
	glm::vec3 backToPosition = this->vecToObjectSys(this->vecToWorldSys(glm::vec3(0,1,-1)));
	this->translate(backToPosition);
}

std::vector <Object *> Room::getColisionList() {
	return this->collisionList;
}

void Room::init() {
	frontWall.translate(glm::vec3(0,0,-1*translateCoef));
	frontWall.scale(glm::vec3(1,1,wallThickness));
	frontWall.setColor(color);
	frontWall.model = cube;
	frontWall.material = material;
	frontWall.texture = wallTexture;
	this->addChild(&frontWall);

	backWall.translate(glm::vec3(0,0,1*translateCoef));
	backWall.scale(glm::vec3(1,1,wallThickness));
	backWall.setColor(color);
	backWall.model = cube;
	backWall.material = material;
	backWall.texture = wallTexture;
	this->addChild(&backWall);

	leftWall.translate(glm::vec3(-1*translateCoef,0,0));
	leftWall.rotate(90, glm::vec3(0,1,0));
	leftWall.scale(glm::vec3(1,1,wallThickness));
	leftWall.setColor(color);
	leftWall.model = cube;
	leftWall.material = material;
	leftWall.texture = wallTexture;
	this->addChild(&leftWall);

	rightWall.translate(glm::vec3(1*translateCoef,0,0));
	rightWall.rotate(90, glm::vec3(0,1,0));
	rightWall.scale(glm::vec3(1,1,wallThickness));
	rightWall.setColor(color);
	rightWall.model = cube;
	rightWall.material = material;
	rightWall.texture = wallTexture;
	this->addChild(&rightWall);

	floor.translate(glm::vec3(0,-1*translateCoef,0));
	floor.rotate(90, glm::vec3(1,0,0));
	floor.scale(glm::vec3(1,1,wallThickness));
	floor.setColor(color);
	floor.model = cube;
	floor.material = material;
	floor.texture = floorTexture;
	this->addChild(&floor);

	floorColider.translate(glm::vec3(0,-1*translateCoef,0));
	floorColider.scale(glm::vec3(1,wallThickness,1));
	this->addChild(&floorColider);

	ceiling.translate(glm::vec3(0,1*translateCoef,0));
	ceiling.rotate(90, glm::vec3(1,0,0));
	ceiling.scale(glm::vec3(1,1,wallThickness));
	ceiling.setColor(color);
	ceiling.model = cube;
	ceiling.material = material;
	ceiling.texture = ceilingTexture;
	this->addChild(&ceiling);

	ceilingColider.translate(glm::vec3(0,1*translateCoef,0));
	ceilingColider.scale(glm::vec3(1,wallThickness,1));
	this->addChild(&ceilingColider);


}

std::vector <FileObject *> FileObject::importAll(std::string dirPath, int maxFiles, DataContainer * gd) {

	std::vector <FileObject *> files;

	ast(fs::is_directory(dirPath), "Invalid directory path");

	fs::path dirPathp = fs::path(dirPath);
	int counter = 0;

	for (auto &p : fs::directory_iterator(dirPathp)) {

		if (++counter > maxFiles) {
			break;
		}

	       	fs::path filePathp = fs::u8path(p);
	       	std::string filePath = filePathp.u8string();

	       	if (fs::is_directory(p)) {

	       		DirectoryObject & dir = *new DirectoryObject(filePath);

	       		/* Ako postoje parametri za ime foldera dir_xxx u mapi, postavi ih
	       		* a ako ne - postavi default Model/Materijal/Teksturu/ za dir */
	       		/* Model */
	       		if (gd->models.count("dir_" + dir.name) && dir.name.length() > 0) {
	       			dir.model = gd->models[dir.name];
	       		} else {
	       			dir.model = gd->models["dir_default"];
	       		}

	       		/* Materijal */
	       		if (gd->materials.count("dir_" + dir.name) && dir.name.length() > 0) {
	       			dir.material = gd->materials[dir.name];
	       		} else {
	       			dir.material = gd->materials["dir_default"];
	       		}

	       		/* Tekstura */
	       		if (gd->textures.count("dir_" + dir.name) && dir.name.length() > 0) {
	       			dir.texture = gd->textures[dir.name];
	       		} else {
	       			dir.texture = gd->textures["dir_default"];
	       		}

	       		files.push_back(&dir);

	       	} else {
	       		/* Ako nije direktorijum vec regularan fajl */

	       		RegularFileObject & file = * new RegularFileObject(filePath);

	       	       /* Ako postoje parametri za ekstenziju formata file_xxx, postavi ih
	       		* a ako ne postavi default Model/Materijal/Teksturu/ za file */
	       		/* Model */
	       		if (gd->models.count("regfile_" + file.extension) && file.extension.length() > 0) {
	       			file.model = gd->models["regfile_" + file.extension];
	       		} else {
	       			file.model = gd->models["regfile_default"];
	       		}

	       		/* Materijal */
	       		if (gd->materials.count("regfile_" + file.extension) && file.extension.length() > 0) {
	       			file.material = gd->materials["regfile_" + file.extension];
	       		} else {
	       			file.material = gd->materials["regfile_default"];
	       		}

	       		/* Tekstura */
	       		if (gd->textures.count("regfile_" + file.extension) && file.extension.length() > 0) {
	       			file.texture = gd->textures["regfile_" + file.extension];
	       		} else {
	       			file.texture = gd->textures["regfile_default"];
	       		}

	       		files.push_back(&file);
	       	}
	  }

	/* Parent dir jos razresimo i dodamo na kraj vektora */
	DirectoryObject & dir = *new DirectoryObject(dirPathp.parent_path());

	if (gd->models.count("dir_" + dir.name) && dir.name.length() > 0) {
		dir.model = gd->models[dir.name];
	} else {
		dir.model = gd->models["dir_default"];
	}

	/* Materijal */
	if (gd->materials.count("dir_" + dir.name) && dir.name.length() > 0) {
		dir.material = gd->materials[dir.name];
	} else {
		dir.material = gd->materials["dir_default"];
	}

	/* Tekstura */
	if (gd->textures.count("dir_" + dir.name) && dir.name.length() > 0) {
		dir.texture = gd->textures[dir.name];
	} else {
		dir.texture = gd->textures["dir_default"];
	}

	files.push_back(&dir);

	return files;
}

FileObject::FileObject(std::string path) : path(path)  {

	const std::regex regName(".*?([^\\/]+)?(\\.*)$", std::regex_constants::icase);

	std::smatch match;

	if (std::regex_search(this->path, match, regName)) {
		this->name = match[1];
	}
}

void FileObject::animate() {

	this->rotate(0.22, glm::vec3(0,1,0));

	if (this->animationParameter >= 2*M_PI) {
		this->animationParameter = 0.0f;
	}

	this->animationParameter += 0.02;

	this->translate(this->vecToObjectSys(glm::vec3(0, 0.0005*sin(this->animationParameter) ,0)));
}

void FileObject::startGlowing() {
	//if (!isGlowing) {
	//	globalData.lights["file_glowing"].setPosition(globalData.activeUser->pointToWorldSys(glm::vec3(0,0,0)));
	//	globalData.lights["file_glowing"].enable();
	//	this->isGlowing = true;
	//}
}

void FileObject::stopGlowing() {
	//if (isGlowing) {
	//	globalData.lights["file_glowing"].disable();
	//	this->isGlowing = false;
	//}
}

void FileObject::action() {

}

RegularFileObject::RegularFileObject(std::string filePath) : FileObject(filePath) {
	/* Odredjivanje extenzije na osnovu imena */
	const std::regex extInName(".*?\\.(.+)", std::regex_constants::icase);
	std::smatch matchExt;
	if (std::regex_search(this->name, matchExt, extInName)) {
		this->extension = matchExt[1];
	}
}

DirectoryObject::DirectoryObject(std::string filePath) : FileObject(filePath) {}

void RegularFileObject::action() {}
void DirectoryObject::action() {
	if (this->path.length() > 0)
		globalData.fxCurrentDir = this->path;
	else 
		globalData.fxCurrentDir = "/";
}
