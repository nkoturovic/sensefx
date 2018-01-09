#include "drawable.h"

#include <algorithm>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void drawable::draw() {
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(this->getPositioningMatrix()));
	/* Iscrtava mrezu (kocku) oko "prostora" objekata */
	//drawSurroundingGrid();
	drawObject();
	drawChildren();
	glPopMatrix();
}

void drawable::drawChildren() {
	for_each (children.begin(), children.end(), [] (object *o) {
		if(drawable* d_o = dynamic_cast<drawable*>(o))
			d_o->draw();
	});
}

void drawable::drawSurroundingGrid() {
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
