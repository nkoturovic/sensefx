#ifndef SENSEFX_CAMERA_H
#define SENSEFX_CAMERA_H

#include "Object.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {

	public:
		glm::mat4 matrix;
		Camera();
		Camera(Object * attachTo);

		Object * attachedTo = NULL;

		glm::mat4 viewMatrix();
};

#endif
