#ifndef SENSEFX_TRANSFORMABLE_H
#define SENSEFX_TRANSFORMABLE_H

#include "object.h"

class transformable : public virtual object {

	public:
		void translate(glm::vec3 translateVec);
		void rotate(float degrees, glm::vec3 aroundVec);
		void scale(glm::vec3 scaleVec);
};

#endif //DRAWABLE
