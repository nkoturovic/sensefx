#include "TransformableObject.h"
#include "TexturedObject.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>

void TransformableObject::rotate(float degrees, glm::vec3 aroundVec) {
	this->setPositioningMatrix(glm::rotate(this->getPositioningMatrix(), glm::radians(degrees), aroundVec));
}

void TransformableObject::translate(glm::vec3 translateVec) {
	this->setPositioningMatrix(glm::translate(this->getPositioningMatrix(), translateVec));
}

void TransformableObject::scale(glm::vec3 scaleVec) {
	this->setPositioningMatrix(glm::scale(this->getPositioningMatrix(), scaleVec));
}
