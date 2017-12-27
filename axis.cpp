#include "object.h"
#include "axis.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
 
