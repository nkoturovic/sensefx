#include "Object.h"
#include "math_objects.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


/* ######### GRID ######### */
Grid::Grid(Object * parent, int numOfUnits) : Object(parent), numOfUnits(numOfUnits) {}
Grid::Grid(int numOfUnits) : Grid(NULL, numOfUnits) {}
Grid::Grid() : Grid(NULL, 10) {}

void Grid::drawObject() {

	if (fill) {
		glPushMatrix();
			glTranslatef(0,0.01,0);
			glColor3f(.55, .55, .55);
			glBegin(GL_QUADS);
					glVertex3f(-1, 0, -1);
					glTexCoord2f(0, 0);
					glVertex3f(1, 0, -1);
					glTexCoord2f(0, 1);
					glVertex3f(1, 0, 1);
					glTexCoord2f(1, 1);
					glVertex3f(-1, 0, 1);
					glTexCoord2f(1, 0);
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
 
WireCube::WireCube(Object * parent) : Object(parent) {}
WireCube::WireCube() : WireCube(NULL){}

void WireCube::drawObject() {
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


/* ######### AXIS ######### */
Axis::Axis(Object * parent, int numOfUnits) : Object(parent), numOfUnits(numOfUnits) {}
Axis::Axis(int numOfUnits) : Axis(NULL, numOfUnits) {}
Axis::Axis() : Axis(NULL, 5) {}

void Axis::drawObject() {

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
Plane::Plane(Object * parent) : Object(parent) {}
Plane::Plane() : Plane(NULL) {}

void Plane::drawObject() {
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
