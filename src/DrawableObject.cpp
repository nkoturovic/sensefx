#include "DrawableObject.h"
#include "TexturedObject.h"

#include <algorithm>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

/* Konstruktori */
DrawableObject::DrawableObject(Object * parent, glm::vec4 color) : Object(parent), color(color) {}
DrawableObject::DrawableObject(Object * parent) : DrawableObject(parent, glm::vec4(1,1,1,1)) {}

DrawableObject::DrawableObject(glm::vec4 color) : Object(NULL), color(color) {}
DrawableObject::DrawableObject() : DrawableObject(glm::vec4(1,1,1,1)) {}


void DrawableObject::draw() {
	glPushMatrix();

		/* Iscrtava mrezu (kocku) oko "prostora" objekata - uncomment */
		//drawSurroundingGrid();
		
		/* Gde ima materijal osvetljenje ce se automatski ukljuciti, inicijalno je iskljuceno */
		glDisable(GL_LIGHTING); 
		/* Postavljanje boje objekta */
		glColor4f(this->color.r, this->color.g, this->color.b, this->color.a);


		glm::mat4 transformation = this->getPositioningMatrix(); // your transformation matrix.
		const float * positioningMatrix = glm::value_ptr(transformation);

		/* TODO: BUGFIX TEKSTURE -- Ovo definitivno treba razmisliti i srediti */
		/* Skaliramo teksture za repeatable (bio je BUG) razvlacenja */
		if(TexturedObject* t_o = dynamic_cast<TexturedObject*>(this)) {
			if (t_o->texture.isRepeatingS() || t_o->texture.isRepeatingT()) {
				/* Uzimamo scale iz matrice transformacija */
				glm::vec3 scale, translation, skew; glm::quat rotation; glm::vec4 perspective;
				glm::decompose(this->transformationMatrix(), scale, rotation, translation, skew, perspective);

				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glScalef(scale.x, scale.y, scale.z);
			}
		}

		/* Mnozenje tekuce ModelView matrice (transformise se prostor) */
		glMatrixMode(GL_MODELVIEW);
		glMultMatrixf(positioningMatrix);

		/* Iscrtavanje datog objekta */
		drawObject();
		/* Iscrtavanje dece */
		drawChildren();
		
	glPopMatrix();
}

void DrawableObject::drawChildren() {
	for_each (children.begin(), children.end(), [] (Object *o) {
		if(DrawableObject* d_o = dynamic_cast<DrawableObject*>(o))
			d_o->draw();
	});
}

void DrawableObject::drawSurroundingGrid() {
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

glm::vec4 DrawableObject::getColor() {
	return this->color;
}

void DrawableObject::setColor(glm::vec4 color) {
	this->color = color;
}
