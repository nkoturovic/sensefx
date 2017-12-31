#include "various_objects.h"
#include "object.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* ######### CHESS FLOOR ######### */
/* Upozorenje kod ispod je IZNABADAN GRUBOM SILOM!!!! */
/* Kod za ovo je prilicno haotican, sve ovo
 * se moze po potrebi ponovo implementirati,
 * ali nije neophodne jer se ovi objekti najvise
 * koriste za testiranje i demonstraciju */


chessFloor::chessFloor(object * parent, int numOfUnits) : object(parent), numOfUnits(numOfUnits) { }
chessFloor::chessFloor(object * parent) : chessFloor(parent, 10){ }
chessFloor::chessFloor(int numOfUnits) : chessFloor(NULL, numOfUnits) { }
chessFloor::chessFloor() : chessFloor(NULL, 10){ }

void chessFloor::drawObject() {

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

void chessFloor::drawChessSingleTexture() {
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


