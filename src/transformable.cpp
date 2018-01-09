#include "transformable.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void transformable::rotate(float degrees, glm::vec3 aroundVec) {
	this->setPositioningMatrix(glm::rotate(this->getPositioningMatrix(), glm::radians(degrees), aroundVec));
}

void transformable::translate(glm::vec3 translateVec) {
	this->setPositioningMatrix(glm::translate(this->getPositioningMatrix(), translateVec));
}

void transformable::scale(glm::vec3 scaleVec) {
	this->setPositioningMatrix(glm::scale(this->getPositioningMatrix(), scaleVec));
}
