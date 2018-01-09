#ifndef SENSEFX_OBJECT_H
#define SENSEFX_OBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "keyboard.h"
#include "mouse.h"

class object {

	private:
		/* Matrica pozicije u odnosu na roditelja 
		 * (relativna pozicija u odnosu na roditelja) */
		glm::mat4 positioningMatrix; 

	protected:
		std::vector <object*> children;

	public:

		object * parent = NULL;

		object();
		object(object * parent);
		virtual ~object();

		glm::mat4 getPositioningMatrix();
		void setPositioningMatrix(glm::mat4 matrix);

		void addChild(object *o);
		void setParent(object * parent);
		void setNoParent();

		/* Metodi point samo tacku menjaju, bez da od nje se oduzme 
		 * polozaj koordinatnog pocetka (time bi se dobio
		 * identican vektor u nasem sistemu) - a ovde se 
		 * dobija polozaj "udajlene tacke" umesto bliskog
		 * vektora */
		glm::vec3 pointToWorldSys(glm::vec3 objVec);
		glm::vec3 pointToObjectSys(glm::vec3 worldVec);
		glm::vec3 pointToObjectSys(object * fromObj, glm::vec3 fromObjVec);

		/* Apsolutna matrica pozicioniranja u WCS - kada
		 * se primene sve transformacije roditelja i napokon
		 * datog objekta */
		glm::mat4 transformationMatrix(object * obj2);
		glm::mat4 transformationMatrix();

		/* Funkcije ispod "analogno kopiraju vektor!!"
		 * pogledaj u isto mesto u koje ja gledam ume-
		 * sto da gledas u mene (moju tacku) !!!
		 * (uzima se u obzir koordinatni pocetak) */

		/* Maksima - Pogledajte svi u istom pravcu umesto
		 * pogledajte svi u istu tacku!!!!!! */
		glm::vec3 vecToWorldSys(glm::vec3 objVec);
		glm::vec3 vecToObjectSys(glm::vec3 worldVec);
		glm::vec3 vecToObjectSys(object * fromObj, glm::vec3 fromObjVec);
};

#endif
