#include "Text.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Text::Text() { }

Text::Text(glm::vec2 position, glm::vec3 color, std::string text) : text(text), position(position), color(color) {}

void Text::setText(std::string text) {
	this->text = text;
}

void Text::setPosition(glm::vec2 position) {
	this->position = position;
}

void Text::setColor(glm::vec3 color) {
	this->color = color;
}

std::string Text::getText() {
	return this->text;
}

glm::vec2 Text::getPosition() {
	return this->position;
}

glm::vec3 Text::getColor() {
	return this->color;
}

void Text::print(glm::vec2 screenSize) {

	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(color.r, color.g, color.b, 1.0 );
	glOrtho(0, screenSize.x, 0, screenSize.y, -5, 5);

	glRasterPos2f(position.x, position.y);

	int len=text.length();
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}

