#ifndef SENSEFX_CAMERA_H
#define SENSEFX_CAMERA_H

#include "object.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera {

	public:
		glm::mat4 matrix;
		camera();
		camera(object * attachTo);

		object * attachedTo = NULL;

		glm::mat4 viewMatrix();
};

#endif
