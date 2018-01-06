#include "object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <limits>

object::object(object * parent) {
	if (parent != NULL)
		parent->addChild(this);
}

object::object() {
}

void object::addChild(object * o) {
	o->parent = this;
	children.push_back(o);
}

void object::draw() {
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(matrix));
	/* Iscrtava mrezu (kocku) oko "prostora" objekata */
	drawSurroundingGrid();
	drawObject();
	drawChildren();
	glPopMatrix();
}

void object::drawChildren() {
	for_each (children.begin(), children.end(), [] (object *o) {
		o->draw();
	});
}

void object::rotate(float degrees, glm::vec3 aroundVec) {
	matrix = glm::rotate(matrix, glm::radians(degrees), aroundVec);
}

void object::translate(glm::vec3 translateVec) {
	matrix = glm::translate(matrix, translateVec);
}

void object::scale(glm::vec3 scaleVec) {
	matrix = glm::scale(matrix, scaleVec);
}

glm::mat4 object::transformationMatrix() {
	object * tmp_ptr = this;
	glm::mat4 result_matrix(1.0f);

	while (tmp_ptr != NULL) {
		result_matrix = tmp_ptr->matrix * result_matrix;
		tmp_ptr = tmp_ptr->parent;
	}

	return result_matrix;
}

void object::setNoParent() {
	setParent(NULL);
}

void object::setParent(object * parent) {

	this->parent = parent;

	if (parent != NULL)
		parent->addChild(this);
}

void object::processKeyboardInput(bool pressedKeys[256], int x, int y) {

	/* Kretanje napred,nazad,levo,desno (komb.) */
	moveKeys(pressedKeys);

	/* Rotitanje objekta ijkl */
	rotateUpKeys(pressedKeys);
	rotateLeftKeys(pressedKeys);

}

void object::moveKeys(bool pressedKeys[256]) {

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

void object::move(glm::vec3 moveVector) {

	/* Provera da li objekat dolazi u koliziju sa bilo kojim
	 * od objekata koje smo mu prosledili u checkColisionList */
	// Potrebno ukljuciti dole na kraju funkcije //
	int numOfColidingY = 0;

	for_each (checkColisionList.begin(), checkColisionList.end(), [&numOfColidingY,this, &moveVector] (object * o) {

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
			if (fabs(normalInO.y) > 0.98f && fabs(normalInThis.y) > 0.98 && fabs(moveVector.y) > 0.0f && fabs(moveVector.x) < 0.01f && fabs(moveVector.z) < 0.01f) {
				addToVelocity.y = -moveVector.y;
			}

			/* Slucaj KRECE se po kutiji ==  */
			 else if (fabs(normalInO.y) > 0.98f && fabs(moveVector.y) < 0.01f &&  fabs(normalInThis.y) > 0.98f && (fabs(moveVector.x) > 0.0f || (fabs(moveVector.z) > 0.0f ))) {
				addToVelocity.x = normalInThisNormalized.x*projIntensity;
				addToVelocity.z = normalInThisNormalized.z*projIntensity;

				/* Svugde gde ima velocity = 0 to znaci da je 
				 * objekat udario u podlogu i da brzinu treba resetovati */
				velocity.y = 0;
			}
			 /* SLUCAJ ZID */
			 else if (fabs(normalInO.y) < 0.01f && fabs(moveVector.y) < 0.01f  && (fabs(moveVector.x) > 0.0f || fabs(moveVector.z) > 0.0f )) {
				addToVelocity.x = normalInThisNormalized.x*projIntensity;
				addToVelocity.z = normalInThisNormalized.z*projIntensity;
			}

			/* Stoji na KRIVOM PODU */
			else if (fabs(normalInO.y) > 0.0f && fabs(normalInThis.y) < 0.98 && fabs(moveVector.y) > 0.0f && fabs(moveVector.x) < 0.01f && fabs(moveVector.z) < 0.01f) {
				addToVelocity.y = -moveVector.y;
				velocity.y = 0;
			}

			/* Ima y brzinu po y-lonu normali ali je nije hteo (Slucaj Kretanje po krivom podu) */ 
		       else if ((fabs(moveVector.x) > 0.0f || fabs(moveVector.z) >0.0f) && fabs(moveVector.y) < 0.01f  && fabs(normalInThis.y) > 0.0f && fabs(normalInO.y) > 0.01 && fabs(normalInThis.y) < 0.98f) {
			       /*Nizbrdo */
			if (projIntensitySigned > 0) {
				/* Za kretanje uzbrdo sa klizanjem otkomentarisi
				 * sve linije za prefiksom #, a zakomentarisi sa 
				 * postfiom @ i suprotno za stepenasto kretanje */

			      	// ZASTO 0.14 NEMAM POOOOOOOOJJJJJJMAAAAAAAAAA 0.134
			     	// Ali it werks, skoro savrseno :)!

				/* x i z ce da dodaju na uspon usporenje (uncomment) */
			      	addToVelocity.x = normalInThisNormalized.x*projIntensity;
			      	addToVelocity.y = normalInThisNormalized.y*(projIntensitySigned);
			      	//addToVelocity.y = normalInThisNormalized.y*(projIntensity); //@
				addToVelocity.z = normalInThisNormalized.z*projIntensity;
		       } else {
				/*UZBRDO */
			      	// ZASTO 0.14 NEMAM POOOOOOOOJJJJJJMAAAAAAAAAA 0.134
				projIntensitySigned*= -0.134;
				
				/* x i z ce da dodaju na nizbrdicu ubrzanje (uncomment) */
			      	addToVelocity.x = normalInThisNormalized.x*projIntensity;
			      	addToVelocity.y = normalInThisNormalized.y*(projIntensitySigned); 
			      	//addToVelocity.y = normalInThisNormalized.y*(projIntensity); //@
				addToVelocity.z = normalInThisNormalized.z*projIntensity;

		       }
				velocity.y = 0;

				/* ZA SADA NE DIRAJ RADI, sem ako imas neku super ideju */
		        } else {
				/* Ne treba nista - ovo ostali slucajevi */
			/* Slobodan pad */
			}
			moveVector += addToVelocity;
			}
	});

	/* Kada PADAS ne kreci se, za to je ovo
	 * ispod potrebno ukljuviti (kod slobodnog pada */
	if(numOfColidingY == 0) {
			//moveVector.x = 0.0f;
			//moveVector.z = 0.0f;
	}

	this->translate(moveVector);
}

void object::addToCheckColisionList(object * o) {
	checkColisionList.push_back(o);
}

void object::removeFromCheckColisionList(object * o) {
	checkColisionList.erase(
		std::remove(checkColisionList.begin(), checkColisionList.end(), o), checkColisionList.end());
}

void object::rotateUpKeys(bool pressedKeys[256]) {

	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	float rotateUp = 0.0f;

	if (pressedKeys[keybindings.rotateUpKey])
		rotateUp += keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateDownKey])
		rotateUp -= keyRotationSensitivity;

	if (rotateUp)
		this->rotate(rotateUp, glm::vec3(1,0,0));
}

void object::rotateLeftKeys(bool pressedKeys[256]) {

	float rotateLeft=0.0f;
	float keyRotationSensitivity = keyboardObj.keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateLeftKey])
		rotateLeft += keyRotationSensitivity;

	if (pressedKeys[keybindings.rotateRightKey])
		rotateLeft -= keyRotationSensitivity;

	if (rotateLeft)
		this->rotate(rotateLeft, glm::vec3(0,1,0));
}

void object::processMouseMove(glm::vec2 delta) {
	rotateMouse(delta);
}

void object::rotateMouse(glm::vec2 delta) {
	float mouseRotationSensitivity = mouseObj.sensitivity;
	this->rotate(delta.x*mouseRotationSensitivity, glm::vec3(0,1,0));
	this->rotate(delta.y*mouseRotationSensitivity, glm::vec3(1,0,0));
}

glm::vec3 object::pointToObjectSys(glm::vec3 worldVec) {
	glm::mat4 transMat = transformationMatrix();
	glm::mat4 inverse = glm::inverse(transMat);
	glm::vec4 worldVec4(worldVec.x, worldVec.y, worldVec.z, 1.0f);
	glm::vec4 resultVec4 = inverse*worldVec4;
	float w = resultVec4.w;

	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 object::vecToObjectSys(glm::vec3 worldVec) {
	glm::mat4 transMat = transformationMatrix();
	glm::mat4 inverse = glm::inverse(transMat);
	glm::vec4 worldVec4(worldVec.x, worldVec.y, worldVec.z, 1.0f);

	glm::vec4 resultVec4 = inverse*worldVec4;
	float w = resultVec4.w;
	glm::vec3 resultVec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);

	glm::vec4 resultOrigin4 = inverse*glm::vec4(0.0f,0.0f,0.0f,1.0f);
	w = resultOrigin4.w;
	glm::vec3 resultOrigin3(resultOrigin4.x/w, resultOrigin4.y/w, resultOrigin4.z/w);

	return resultVec3 - resultOrigin3;
}

glm::vec3 object::pointToObjectSys(object * fromObj, glm::vec3 fromObjVec) {
	glm::vec4 tmpVec4(fromObjVec.x, fromObjVec.y, fromObjVec.z, 1.0f);
	glm::vec4 resultVec4 = fromObj->transformationMatrix(this)*tmpVec4;
	float w = resultVec4.w;
	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 object::vecToObjectSys(object * fromObj, glm::vec3 fromObjVec) {
	glm::mat4 tm = fromObj->transformationMatrix(this);
	glm::vec4 tmpVec4(fromObjVec.x, fromObjVec.y, fromObjVec.z, 1.0f);
	glm::vec4 resultVec4 = tm*tmpVec4;
	float w = resultVec4.w;
	glm::vec3 resultVec3 = glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);

	glm::vec4 resultOriginVec4 = tm*glm::vec4(0.0f,0.0f,0.0f,1.0f);
	 w = resultOriginVec4.w;
	glm::vec3 resultOriginVec3 = glm::vec3(resultOriginVec4.x/w, resultOriginVec4.y/w, resultOriginVec4.z/w);

	return  resultVec3 - resultOriginVec3;
}

glm::vec3 object::pointToWorldSys(glm::vec3 objVec) {
	glm::vec4 worldVec4(objVec.x, objVec.y, objVec.z, 1.0f);
	glm::mat4 transMat = transformationMatrix();
	glm::vec4 resultVec4 = transMat*worldVec4;
	float w = resultVec4.w;

	return glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
}

glm::vec3 object::vecToWorldSys(glm::vec3 objVec) {
	glm::vec4 worldVec4(objVec.x, objVec.y, objVec.z, 1.0f);
	glm::mat4 transMat = transformationMatrix();
	glm::vec4 resultVec4 = transMat*worldVec4;
	float w = resultVec4.w;
	glm::vec3 resultVec3 = glm::vec3(resultVec4.x/w, resultVec4.y/w, resultVec4.z/w);
	glm::vec4 resultOriginVec4 = transMat*w*glm::vec4(0.0f,0.0f,0.0f,1.0f);
	w = resultOriginVec4.w;
	glm::vec3 resultOriginVec3 = glm::vec3(resultOriginVec4.x/w, resultOriginVec4.y/w, resultOriginVec4.z/w);

	return resultVec3 - resultOriginVec3;
}

glm::mat4 object::transformationMatrix(object * obj2 ) {

	/* Mnozenje sa ovom daje world */
	glm::mat4 transMat1 = this->transformationMatrix();
	glm::mat4 transMat2 = glm::inverse(obj2->transformationMatrix());

	return transMat2*transMat1;
}

bool object::isColiding(object * obj) {

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

void object::drawSurroundingGrid() {
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,-1,1);
		glVertex3f(1,-1,1);
		glVertex3f(1,-1,-1);
		glVertex3f(-1,-1,-1);

		glVertex3f(-1,1,-1);
		glVertex3f(-1,1,1);
		glVertex3f(1,1,1);
		glVertex3f(1,1,-1);
		glVertex3f(-1,1,-1);
	glEnd();

	glBegin(GL_LINES);

		glVertex3f(1,-1,-1);
		glVertex3f(1,1,-1);
		glVertex3f(-1,-1,1);
		glVertex3f(-1,1,1);
		glVertex3f(1,-1,1);
		glVertex3f(1,1,1);

	glEnd();
}
