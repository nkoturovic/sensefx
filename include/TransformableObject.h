#ifndef SENSEFX_TRANSFORMABLE_OBJECT_H
#define SENSEFX_TRANSFORMABLE_OBJECT_H

#include "Object.h"

class TransformableObject : public virtual Object {

	public:
		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
		void scale(glm::vec3 scaleVec);
};

#endif // TRANSFORMABLE_OBJECT
