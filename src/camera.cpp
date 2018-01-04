#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

camera::camera(object * attachTo) : matrix(1.0f), attachedTo(attachTo) {}
camera::camera() : camera(NULL) {}

glm::mat4 camera::viewMatrix() {
	if (this->attachedTo == NULL) {
		return glm::inverse(this->matrix);
	}

	return glm::lookAt(attachedTo->pointToWorldSys(glm::vec3(0,0,0)), attachedTo->pointToWorldSys(glm::vec3(0,0,-1)), glm::vec3(0,1,0));
}
