#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

camera::camera(object * attachTo) : matrix(1.0f), attachedTo(attachTo) {}
camera::camera() : camera(NULL) {}

glm::mat4 camera::viewMatrix() {
	if (this->attachedTo == NULL) 
		return glm::inverse(this->matrix);

	return glm::inverse(this->attachedTo->transformationMatrix() * this->matrix);
}
