#include "Object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

Object::Object(Object * parent) {
	if (parent != NULL)
		parent->addChild(this);
}

Object::Object() {
}

Object::~Object() { }

void Object::addChild(Object * o) {
	o->parent = this;
	children.push_back(o);
}

glm::mat4 Object::transformationMatrix() {
	Object * tmp_ptr = this;
	glm::mat4 result_matrix(1.0f);

	while (tmp_ptr != NULL) {
		result_matrix = tmp_ptr->getPositioningMatrix() * result_matrix;
		tmp_ptr = tmp_ptr->parent;
	}

	return result_matrix;
}

void Object::setNoParent() {
	setParent(NULL);
}

void Object::setParent(Object * parent) {

	this->parent = parent;

	if (parent != NULL)
		parent->addChild(this);
}

glm::vec3 Object::pointToObjectSys(glm::vec3 worldVec) {
	glm::mat4 transMat = transformationMatrix();
	glm::mat4 inverse = glm::inverse(transMat);
	glm::vec4 worldVec4(worldVec.x, worldVec.y, worldVec.z, 1.0f);
	glm::vec4 resultVec4 = inverse*worldVec4;
	float w = resultVec4.w;

	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 Object::vecToObjectSys(glm::vec3 worldVec) {
	glm::mat4 transMat = transformationMatrix();
	glm::mat4 inverse = glm::inverse(transMat);
	glm::vec4 worldVec4(worldVec.x, worldVec.y, worldVec.z, 1.0f);

	glm::vec4 resultVec4 = inverse*worldVec4;
	float w = resultVec4.w;
	glm::vec3 resultVec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);

	glm::vec4 resultOrigin4 = inverse*glm::vec4(0.0f,0.0f,0.0f,1.0f);
	w = resultOrigin4.w;
	glm::vec3 resultOrigin3(resultOrigin4.x/w, resultOrigin4.y/w, resultOrigin4.z/w);

	return resultVec3 - resultOrigin3;
}

glm::vec3 Object::pointToObjectSys(Object * fromObj, glm::vec3 fromObjVec) {
	glm::vec4 tmpVec4(fromObjVec.x, fromObjVec.y, fromObjVec.z, 1.0f);
	glm::vec4 resultVec4 = fromObj->transformationMatrix(this)*tmpVec4;
	float w = resultVec4.w;
	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 Object::vecToObjectSys(Object * fromObj, glm::vec3 fromObjVec) {
	glm::mat4 tm = fromObj->transformationMatrix(this);
	glm::vec4 tmpVec4(fromObjVec.x, fromObjVec.y, fromObjVec.z, 1.0f);
	glm::vec4 resultVec4 = tm*tmpVec4;
	float w = resultVec4.w;
	glm::vec3 resultVec3 = glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);

	glm::vec4 resultOriginVec4 = tm*glm::vec4(0.0f,0.0f,0.0f,1.0f);
	 w = resultOriginVec4.w;
	glm::vec3 resultOriginVec3 = glm::vec3(resultOriginVec4.x/w, resultOriginVec4.y/w, resultOriginVec4.z/w);

	return  resultVec3 - resultOriginVec3;
}

glm::vec3 Object::pointToWorldSys(glm::vec3 objVec) {
	glm::vec4 worldVec4(objVec.x, objVec.y, objVec.z, 1.0f);
	glm::mat4 transMat = transformationMatrix();
	glm::vec4 resultVec4 = transMat*worldVec4;
	float w = resultVec4.w;

	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 Object::vecToWorldSys(glm::vec3 objVec) {
	glm::vec4 worldVec4(objVec.x, objVec.y, objVec.z, 1.0f);
	glm::mat4 transMat = transformationMatrix();
	glm::vec4 resultVec4 = transMat*worldVec4;
	float w = resultVec4.w;
	glm::vec3 resultVec3 = glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
	glm::vec4 resultOriginVec4 = transMat*w*glm::vec4(0.0f,0.0f,0.0f,1.0f);
	w = resultOriginVec4.w;
	glm::vec3 resultOriginVec3 = glm::vec3(resultOriginVec4.x/w, resultOriginVec4.y/w, resultOriginVec4.z/w);

	return resultVec3 - resultOriginVec3;
}

glm::mat4 Object::transformationMatrix(Object * obj2 ) {

	/* Mnozenje sa ovom daje world */
	glm::mat4 transMat1 = this->transformationMatrix();
	glm::mat4 transMat2 = glm::inverse(obj2->transformationMatrix());

	return transMat2*transMat1;
}

glm::mat4 Object::getPositioningMatrix() {
	return this->positioningMatrix;
}


void Object::setPositioningMatrix(glm::mat4 positioningMatrix) {
	this->positioningMatrix = positioningMatrix;
}
