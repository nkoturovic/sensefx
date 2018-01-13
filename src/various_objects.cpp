#include "various_objects.h"
#include "Object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
