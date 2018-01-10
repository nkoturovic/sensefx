#include "various_objects.h"
#include "Object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* ######### CHESS FLOOR ######### */
/* Upozorenje kod ispod je IZNABADAN GRUBOM SILOM!!!! */
/* Kod za ovo je prilicno haotican, sve ovo
 * se moze po potrebi ponovo implementirati,
 * ali nije neophodne jer se ovi objekti najvise
 * koriste za testiranje i demonstraciju */


ChessFloor::ChessFloor(Object * parent, int numOfUnits) : Object(parent), numOfUnits(numOfUnits) { }
ChessFloor::ChessFloor(Object * parent) : ChessFloor(parent, 10){ }
ChessFloor::ChessFloor(int numOfUnits) : ChessFloor(NULL, numOfUnits) { }
ChessFloor::ChessFloor() : ChessFloor(NULL, 10){ }

void ChessFloor::drawObject() {

	float coef = 1.0f/numOfUnits;
	glTranslatef(-coef,0,-coef);
	glTranslatef(-1,0,-1);
	glScalef(coef,coef,coef);
	glTranslatef(-4,0,-4);

	for (int i=0; i<numOfUnits/2+1; i++) {
		glTranslatef(4.0f,0.0f,0.0f);
		glPushMatrix();
		for (int j=0; j<numOfUnits/2+1; j++) {
			glTranslatef(0.0f,0.0f,4.0f);
			drawChessSingleTexture();
		}
		glPopMatrix();
	}

}

void ChessFloor::drawChessSingleTexture() {

	glPushMatrix();
		glColor3f(0.1,0.1,0.1);
		glBegin(GL_QUADS);
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(1.0, 0.0, -1.0);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 1.0);

		glEnd();

		glTranslatef(2,0, 0);

		glColor3f(0.9,0.9,0.9);
		glBegin(GL_QUADS);
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(1.0, 0.0, -1.0);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 1.0);
		glEnd();

		glTranslatef(0 ,0, 2);
		glColor3f(0.1,0.1,0.1);

		glBegin(GL_QUADS);
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(1.0, 0.0, -1.0);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 1.0);
		glEnd();

		glTranslatef(-2 ,0, 0);
		glColor3f(0.9,0.9,0.9);

		glBegin(GL_QUADS);
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(1.0, 0.0, -1.0);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 1.0);
		glEnd();

		glPopMatrix();
}

TriangleFloor::TriangleFloor(Object * parent, int numOfUnits) : Object(parent), numOfUnits(numOfUnits) { }
TriangleFloor::TriangleFloor(Object * parent) : TriangleFloor(parent, 10){ }
TriangleFloor::TriangleFloor(int numOfUnits) : TriangleFloor(NULL, numOfUnits) { }
TriangleFloor::TriangleFloor() : TriangleFloor(NULL, 10){ }

void TriangleFloor::drawObject() {
	float coef = 1.0f/numOfUnits;
	glTranslatef(-coef,0,-coef);
	glTranslatef(-1,0,-1);
	glScalef(coef,1.0f,coef);

	for (int i=0; i<numOfUnits; i++) {
		glTranslatef(2.0f,0.0f,0.0f);
		glPushMatrix();
		for (int j=0; j<numOfUnits; j++) {
			glTranslatef(0.0f,0.0f,2.0f);
			drawtriangleSingleTexture();
		}
		glPopMatrix();
	}
}

void TriangleFloor::drawtriangleSingleTexture() {

	glPushMatrix();

		glTranslatef(0.0,-0.001,0.0);
		glColor3f(0.9,0.9,0.9);
		glBegin(GL_QUADS);
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(1.0, 0.0, -1.0);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 1.0);

		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,0.01,0);

		glColor3f(0.1,0.1,0.1);
		glBegin(GL_TRIANGLES);
			glVertex3f(-1.0, 0.0,-1.0);
			glVertex3f(1.0, 0.0,-1.0);
			glVertex3f(0.0, 0.0, 0.0);

			glVertex3f(1.0, 0.0, 0.0);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(0.0, 0.0, 1.0);

			glVertex3f(0.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 1.0);
			glVertex3f(-1.0, 0.0, 0.0);


		glEnd();
	glPopMatrix();
}

