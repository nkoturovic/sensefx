#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Object {

	private:
		/* Matrica pozicije u odnosu na roditelja
		 * (relativna pozicija u odnosu na roditelja) */
		glm::mat4 positioningMatrix;

	protected:
		std::vector <Object*> children;

	public:
		Object * parent = NULL;

		Object();
		Object(Object * parent);
		virtual ~Object();

		glm::mat4 getPositioningMatrix();
		void setPositioningMatrix(glm::mat4 matrix);

		void addChild(Object *o);
		void setParent(Object * parent);
		void setNoParent();

		/* Matrica koja transformise iz obj2 CS u this objects CS */
		glm::mat4 transformationMatrix(Object * obj2);
		/* Matrica koja transformise iz WCS u this objects CS */
		glm::mat4 transformationMatrix();

		/* Metodi point samo tacku vracaju, bez da od nje se oduzme
		 * polozaj koordinatnog pocetka starog repera (time bi se dobio
		 * identican vektor u nasem sistemu) - a ovde se
		 * dobija polozaj "udajlene tacke" u (this) reperu */
		glm::vec3 pointToWorldSys(glm::vec3 objVec);
		glm::vec3 pointToObjectSys(glm::vec3 worldVec);
		glm::vec3 pointToObjectSys(Object * fromObj, glm::vec3 fromObjVec);

		/* Funkcije "analogno kopiraju vektor!!"
		 * pogledaj u isto mesto u koje ja gledam ume-
		 * sto da gledas u mene (moju tacku) !!!
		 * (uzima se u obzir koordinatni pocetak starog
		 * repera - oduzima se od pozicije tacke) */
		glm::vec3 vecToWorldSys(glm::vec3 objVec);
		glm::vec3 vecToObjectSys(glm::vec3 worldVec);
		glm::vec3 vecToObjectSys(Object * fromObj, glm::vec3 fromObjVec);
};

#endif
