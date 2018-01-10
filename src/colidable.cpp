#include "colidable.h"

#include <algorithm>

bool colidable::isColiding(object * obj) {
	
	/* Ideja je da pogledamo da li se ivice prve kocke nalaze
	 * u drugoj kocki i obtatno ivice druge u prvoj, prva/druga kocka 
	 * je ustvari ceo prostor prvog odnosno drugog objekta */ 

	/* Ivice kocke */
	glm::vec4 vertices[] = {glm::vec4(-1.0f,-1.0f,-1.0f,1.0f), glm::vec4(-1.0f,-1.0f,1.0f,1.0f), glm::vec4(-1.0f,1.0f,-1.0f,1.0f), glm::vec4(-1.0f,1.0f,1.0f,1.0f),
 				glm::vec4(1.0f,-1.0f,-1.0f,1.0f), glm::vec4(1.0f,-1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,-1.0f,1.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f)};

	float maxFloat = std::numeric_limits<float>::max();

	/* Projekcija 3 najblize tacke po x,y,z u jednu (minx, miny, minz) */
	/* Inicijalno stavljamo na max vrednost float-a */
	glm::vec3 min(maxFloat,maxFloat,maxFloat);

	/* Matrica koja transformise iz obj u this */
	glm::mat4 transMat = this->transformationMatrix(obj);

	glm::vec4 tmp4;
	glm::vec3 tmp3;
	float tmpf;
	float w;
	
	/* Prolazimo kroz sve ivice (kocke) drugog prosora izrazene u prvom
	 * prostoru i trazimo najblizu uvicu po x,y,z */
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

	/* Na kojoj udaljenosti se detektuje kolizija */
	float detectNearness = 1.01f;

	/* Ako se sva 3 minimuma (minx,miny,minz) nalaze unutar regije
	 * odredjene sa detectNearness, onda kazemo da je doslo do kolizije */
	if (min.x <= detectNearness && min.y <= detectNearness && min.z <= detectNearness)
		return true;

	/* Ukoliko postoji bar jedan minimum koji je van detectNearness regije,
	 * nastavljamo istu proveru za this objekat u obj objektu */

	/* Matrica koja transformise iz this u obj */
	transMat = obj->transformationMatrix(this);

	/* Inicijalno stavljamo na max vrednost float-a */
	min.x = maxFloat;
	min.y = maxFloat;
	min.z = maxFloat;

	/* Prolazimo kroz sve ivice (kocke) prvog prosora izrazene u drugom
	 * prostoru i trazimo najblizu uvicu po x,y,z */
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

	/* Ako se sva 3 minimuma (minx,miny,minz) nalaze unutar regije
	 * odredjene sa detectNearness, onda kazemo da je doslo do kolizije */
	if (min.x <= detectNearness && min.y <= detectNearness && min.z <= detectNearness)
		return true;

	/* Nije doslo do kolizije */
	return false;
}

void colidable::addToCheckColisionList(object * o) {
	checkColisionList.push_back(o);
}

void colidable::removeFromCheckColisionList(object * o) {
	checkColisionList.erase(
		std::remove(checkColisionList.begin(), checkColisionList.end(), o), checkColisionList.end());
}
