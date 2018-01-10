#include "MovableObject.h"

#include <algorithm>

MovableObject::MovableObject(float speed, float jump) : speed(speed), jump(jump) { }

MovableObject::MovableObject() : MovableObject(0.1, 0.15) {};

void MovableObject::moveKeys(bool pressedKeys[256]) {

	/* Kretanje napred,nazad,levo,desno (komb.) */

	float moveForward = 0.0f, moveLeft = 0.0f;

	if (pressedKeys[keybindings.moveForwardKey])
		moveForward-=1.0f;

	if (pressedKeys[keybindings.moveBackKey])
		moveForward+=1.0f;

	if (pressedKeys[keybindings.moveLeftKey])
		moveLeft-=1.0f;

	if (pressedKeys[keybindings.moveRightKey])
		moveLeft+=1.0f;

	if (moveLeft || moveForward)
		this->move(glm::normalize(glm::vec3(moveLeft, 0, moveForward))*speed);

}

/* UPOZORENJE!! - Funkcija je napisana prilicno haoticno, i treba da
 * se udesi, bar napravi citljivijom i otklone pojedini bug-ovi
 * za sada radi i razresava kolizije OK za kockaste objekte */
void MovableObject::move(glm::vec3 moveVector) {

	/* Provera da li objekat dolazi u koliziju sa bilo kojim
	 * od objekata koje smo mu prosledili u checkColisionList */
	// Potrebno ukljuciti dole na kraju funkcije //
	int numOfColidingY = 0;

	for_each (checkColisionList.begin(), checkColisionList.end(), [&numOfColidingY,this, &moveVector] (Object * o) {

		if (this->isColiding(o)) {

			/* Centar objekta this u koordinatnom sistemu objekta o */
			glm::vec3 thisCenterInO = o->pointToObjectSys(this, glm::vec3(0,0,0));

			/* Trazimo normalu stranice kvadra */
			if (fabs(thisCenterInO.x) > fabs(thisCenterInO.y)) {
				thisCenterInO.y = 0;
				if (fabs(thisCenterInO.x) > fabs(thisCenterInO.z))
						thisCenterInO.z = 0;
				else 
						thisCenterInO.x = 0;
			} else {
				thisCenterInO.x = 0;
				if (fabs(thisCenterInO.y) > fabs(thisCenterInO.z))
						thisCenterInO.z = 0;
				else 
						thisCenterInO.y = 0;
			}

			/* Normala u koordinatnom sistemu objekta O */
			glm::vec3 normalInO = glm::normalize(thisCenterInO);

			/* Osiguravamo da ima podloga */
			if (abs(normalInO.y) > 0.0f)
				numOfColidingY++;

			//glm::vec3 moveVecInThisNormalized = glm::normalize(moveVector);

			/* Normala u koordinatnom sistemu objekta This */
			glm::vec3 normalInThis = this->vecToObjectSys(o, normalInO);
			glm::vec3 normalInThisNormalized = glm::normalize(normalInThis);

			/* Intenzitet vektora brzine u smetu ove normale */
			float projIntensity = glm::dot((glm::vec3) moveVector, (glm::vec3) normalInThisNormalized);
			float projIntensitySigned = projIntensity;
			projIntensity = fabs(projIntensity);
			//float projIntensityNormal = projIntensity/glm::length(moveVector);

			//float cosAngle = angle(glm::normalize(moveVector),normalInThisNormalized)/M_PI;
			//float cosAngle = projIntensity/glm::length(moveVector);
			//float sinAngle = sqrt(1-cosAngle*cosAngle);

			glm::vec3 addToVelocity(0,0,0);

			/* Ima brzinu po normali i hteo je samo po y-lonu */
			/* Slucaj STOJI na kutiji */
			if (normalInO.y > 0.98f && fabs(normalInThisNormalized.y) > 0.98 && fabs(moveVector.y) > 0.0f && fabs(moveVector.x) < 0.01f && fabs(moveVector.z) < 0.01f) {
				addToVelocity.y = -moveVector.y;

				/* Svugde gde ima isOnGround = true to znaci da je 
				 * objekat udario u podlogu (mora da pada kada udari 
				 * zato ovo manje od 0*/
				if (this->velocity.y <= 0.01f) {
					this->velocity.y = 0; 
					this->isOnGround = true;
				} else {
					addToVelocity.y = moveVector.y;
				}
			}

			/* Slucaj udara u kutiju odozdo ==  mozda nije potrebno */
			else if (normalInO.y < 0.98f && fabs(normalInThisNormalized.y) > 0.98 && fabs(moveVector.y) > 0.0f
					&& fabs(moveVector.x) < 0.01f && fabs(moveVector.z) < 0.01f) {
				addToVelocity.y = 0.0f;
			}

			/* Slucaj KRECE se po kutiji ==  */
			 else if (fabs(normalInO.y) > 0.98f && fabs(moveVector.y) < 0.01f &&  fabs(normalInThisNormalized.y) > 0.98f && 
					 (fabs(moveVector.x) > 0.0f || (fabs(moveVector.z) > 0.0f ))) {
				addToVelocity.x = normalInThisNormalized.x*projIntensity;
				addToVelocity.z = normalInThisNormalized.z*projIntensity;
			}
			 /* SLUCAJ ZID */
			 else if (fabs(normalInO.y) < 0.01f && fabs(moveVector.y) < 0.01f  && (fabs(moveVector.x) > 0.0f || fabs(moveVector.z) > 0.0f )) {
				if (fabs(normalInThisNormalized.y) > 0.0f) {
					/* Kriv zid */
					addToVelocity.x = normalInThisNormalized.x * 2;
					addToVelocity.z = normalInThisNormalized.z * 2;
					addToVelocity.y = -7; // Propadni bolje nego da udjes u zid
				}else {
					/* prav zid */
					addToVelocity.x = normalInThisNormalized.x*projIntensity;
					addToVelocity.z = normalInThisNormalized.z*projIntensity;
				}
				
			}

			/* Stoji na KRIVOM PODU */
			else if (fabs(normalInO.y) > 0.0f && fabs(normalInThisNormalized.y) < 0.98 && fabs(moveVector.y) > 0.0f && 
					fabs(moveVector.x) < 0.01f && fabs(moveVector.z) < 0.01f) {
				addToVelocity.y = -moveVector.y;

				if (this->velocity.y <= 0.01f) {
					this->velocity.y = 0;
					this->isOnGround = true;
				} else {
					addToVelocity.y = moveVector.y;
				}
			}

			/* Ima y brzinu po y-lonu normali ali je nije hteo (Slucaj Kretanje po krivom podu) */ 
		       else if ((fabs(moveVector.x) > 0.0f || fabs(moveVector.z) >0.0f) && fabs(moveVector.y) < 0.01f  && 
				       fabs(normalInThisNormalized.y) > 0.0f && fabs(normalInO.y) > 0.01 && fabs(normalInThisNormalized.y) < 0.98f) {
			       /*Nizbrdo */
			if (projIntensitySigned > 0) {
				projIntensitySigned*= 0.134;
				/* Za kretanje uzbrdo sa klizanjem otkomentarisi
				 * sve linije za prefiksom #, a zakomentarisi sa 
				 * postfiom @ i suprotno za stepenasto kretanje */

			      	// ZASTO 0.14 NEMAM POOOOOOOOJJJJJJMAAAAAAAAAA 0.134
			     	// Ali it werks, skoro savrseno :)!

				/* x i z ce da dodaju na uspon usporenje (uncomment) */
			      	addToVelocity.x = normalInThisNormalized.x*projIntensity;
			      	//addToVelocity.y = normalInThisNormalized.y*(projIntensitySigned);
			      	addToVelocity.y = normalInThisNormalized.y*(projIntensitySigned); //@
				addToVelocity.z = normalInThisNormalized.z*projIntensity;
		       } else {
				/*UZBRDO */
			      	// ZASTO 0.14 NEMAM POOOOOOOOJJJJJJMAAAAAAAAAA 0.134
				projIntensitySigned*= -0.134;
				//projIntensitySigned = projIntensity;
				
				/* x i z ce da dodaju na nizbrdicu ubrzanje (uncomment) */
			      	addToVelocity.x = normalInThisNormalized.x*projIntensity;
			      	addToVelocity.y = normalInThisNormalized.y*(projIntensitySigned);
			      	//addToVelocity.y = normalInThisNormalized.y*(projIntensity); //@
				addToVelocity.z = normalInThisNormalized.z*projIntensity;
		       }

				/* ZA SADA NE DIRAJ RADI, sem ako imas neku super ideju */
		        } else {
				/* Ne treba nista - ovo ostali slucajevi */
			/* Slobodan pad */
			}
			moveVector += addToVelocity;
			}
	});

	/* Kada PADAS ne kreci se, za to je ovo
	 * ispod potrebno ukljuviti (kod slobodnog pada 
	 * I PLUS DA ZNA KAD NEMA PODLOGU ZA SKOK*/

	if(numOfColidingY == 0) {
		this->isOnGround = false;
			//moveVector.x = 0.0f;
			//moveVector.z = 0.0f;
	}

	this->translate(moveVector);
}



void MovableObject::rotateUpKeys(bool pressedKeys[256]) {

	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	float rotateUp = 0.0f;

	if (pressedKeys[keybindings.rotateUpKey])
		rotateUp += keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateDownKey])
		rotateUp -= keyRotationSensitivity;

	if (rotateUp)
		this->rotate(rotateUp, glm::vec3(1,0,0));
}

void MovableObject::rotateLeftKeys(bool pressedKeys[256]) {

	float rotateLeft=0.0f;
	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateLeftKey])
		rotateLeft += keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateRightKey])
		rotateLeft -= keyRotationSensitivity;

	if (rotateLeft)
		this->rotate(rotateLeft, glm::vec3(0,1,0));
}

void MovableObject::jumpKeys(bool pressedKeys[256]) {

	if (pressedKeys[keybindings.jumpKey]) {
		if (this->isOnGround) {
			this->translate(glm::vec3(0,0.1,0));
			this->velocity.y = jump;
		}
	}
}

void MovableObject::rotateMouse(glm::vec2 delta) {
	float mouseRotationSensitivity = mouseObj.sensitivity;
	this->rotate(delta.x*mouseRotationSensitivity, glm::vec3(0,1,0));
	this->rotate(delta.y*mouseRotationSensitivity, glm::vec3(1,0,0));
}

void MovableObject::addToVelocity(glm::vec3 addVec) {
	this->velocity += addVec;
}
void MovableObject::setVelocity(glm::vec3 velocity) {
	this->velocity = velocity;
}
glm::vec3 MovableObject::getVelocity() {
	return this->velocity;
}

