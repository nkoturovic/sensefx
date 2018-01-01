#include "object.h"
#include "math_objects.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


/* ######### GRID ######### */
grid::grid(object * parent, int numOfUnits) : object(parent), numOfUnits(numOfUnits) {}
grid::grid(int numOfUnits) : grid(NULL, numOfUnits) {}

void grid::drawObject() {

	if (fill) {
		glPushMatrix();
			glTranslatef(0,-0.01,0);
			glColor3f(.55, .55, .55);
			glBegin(GL_QUADS);
					glVertex3f(-1, 0, -1);
					glVertex3f(1, 0, -1);
					glVertex3f(1, 0, 1);
					glVertex3f(-1, 0, 1);
			glEnd();
		glPopMatrix();
	}
		float coef = 1.0/numOfUnits; 
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_LINES);
		for (float i=-1; i <= 1; i+= coef) {

				glVertex3f(i, 0, -1);
				glVertex3f(i, 0, 1);

				glVertex3f(-1, 0, i);
				glVertex3f(1, 0, i);
		}
		glEnd();
}
 

/* ######### AXIS ######### */
axis::axis(object * parent, int numOfUnits) : object(parent), numOfUnits(numOfUnits) {}
axis::axis(int numOfUnits) : axis(NULL, numOfUnits) {}

void axis::drawObject() {

	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(-1, 0, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 1, 0);
		glVertex3f(0, -1, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0,  1);
		glVertex3f(0, 0, -1);

		float coef = 1.0/numOfUnits; 
		for (float i=-1; i < 1; i+= coef) {
			glColor3f(1, 0, 0);
			glVertex3f(i, -0.1*coef, 0);
			glVertex3f(i, +0.1*coef, 0);

			glColor3f(0, 0, 1);
			glVertex3f(-0.1*coef, i, 0);
			glVertex3f(+0.1*coef, i, 0);

			glColor3f(0, 1, 0);
			glVertex3f(-0.1*coef, 0, i);
			glVertex3f(+0.1*coef, 0, i);
		}

	glEnd();
}
 
/* ######### PLANE ######### */
plane::plane(object * parent) : object(parent) {}
plane::plane() : plane(NULL) {}

void plane::drawObject() {
	glBegin(GL_QUADS);
		glColor3f(.55, .55, .55);
		glBegin(GL_QUADS);
				glNormal3f(0,1,0);
				glVertex3f(-1, 0, -1);
				glVertex3f(1, 0, -1);
				glVertex3f(1, 0, 1);
				glVertex3f(-1, 0, 1);
		glEnd();
	
	glEnd();
}
