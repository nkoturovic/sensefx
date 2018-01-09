#include "colidable.h"

#include <algorithm>

bool colidable::isColiding(object * obj) {

	/* Surrounding cube vertices */
	glm::vec4 vertices[] = {glm::vec4(-1.0f,-1.0f,-1.0f,1.0f), glm::vec4(-1.0f,-1.0f,1.0f,1.0f), glm::vec4(-1.0f,1.0f,-1.0f,1.0f), glm::vec4(-1.0f,1.0f,1.0f,1.0f),
 				glm::vec4(1.0f,-1.0f,-1.0f,1.0f), glm::vec4(1.0f,-1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,-1.0f,1.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f)};

	float maxFloat = std::numeric_limits<float>::max();
	glm::vec3 min(maxFloat,maxFloat,maxFloat);

	glm::mat4 transMat = this->transformationMatrix(obj);
	glm::vec4 tmp4;
	glm::vec3 tmp3;
	float tmpf;
	float w;

	for (int i=0; i<8; i++) {
		tmp4 = transMat*vertices[i];
		w = tmp4.w;
		tmp3.x = tmp4.x/w;
		tmp3.y = tmp4.y/w;
		tmp3.z = tmp4.z/w;

		if ((tmpf = fabs(tmp3.x)) < min.x)
			min.x = tmpf;
		if ((tmpf = fabs(tmp3.y)) < min.y)
			min.y = tmpf;
		if ((tmpf = fabs(tmp3.z)) < min.z)
			min.z = tmpf;
	}

	float detectNearness = 1.01f;
	if (min.x <= detectNearness && min.y <= detectNearness && min.z <= detectNearness)
		return true;

	transMat = obj->transformationMatrix(this);

	min.x = maxFloat;
	min.y = maxFloat;
	min.z = maxFloat;

	for(int i=0; i<8; i++) {
		tmp4 = transMat*vertices[i];
		w = tmp4.w;
		tmp3.x = tmp4.x/w;
		tmp3.y = tmp4.y/w;
		tmp3.z = tmp4.z/w;

		if ((tmpf = fabs(tmp3.x)) < min.x)
			min.x = tmpf;
		if ((tmpf = fabs(tmp3.y)) < min.y)
			min.y = tmpf;
		if ((tmpf = fabs(tmp3.z)) < min.z)
			min.z = tmpf;
	}

	if (min.x <= detectNearness && min.y <= detectNearness && min.z <= detectNearness)
		return true;

	return false;
}

void colidable::addToCheckColisionList(object * o) {
	checkColisionList.push_back(o);
}

void colidable::removeFromCheckColisionList(object * o) {
	checkColisionList.erase(
		std::remove(checkColisionList.begin(), checkColisionList.end(), o), checkColisionList.end());
}
