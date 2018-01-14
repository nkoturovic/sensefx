#include "TexturedObject.h"

#include <algorithm>


TexturedObject::TexturedObject(Object * parent, glm::vec4 color, Model model, Material material, Texture2D texture) :
	DrawableObject(parent, color), model(model), material(material), texture(texture) {}

TexturedObject::TexturedObject() : DrawableObject(NULL) {}

void TexturedObject::drawObject() {

	/* Ujljucivanje osvetljenja */
	glEnable(GL_LIGHTING);

	/* Postavljanje materijala */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.getAmbientCoefs());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.getDiffuseCoefs());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.getSpecularCoefs());
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.getEmissionCoefs());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.getShininess());


	/* Ukljucivanje teksture */
	this->texture.bind();

	/* Iscrtavanje objekta */
	drawTexturedObject();	

	/* Iskljucivanje teksture */
	Texture2D::unbind();

	glEnable(GL_DEPTH_TEST);
}

void TexturedObject::drawTexturedObject() {

	int numOfVertices = model.vertices.size();
	int numOfNormals = model.normals.size();
	int numOfTextureCoordinates = model.textureCoordinates.size(); 

	glBegin(GL_TRIANGLES);

	for (int i=0; i<numOfVertices; i++) {

		if (i < numOfNormals) {
			glNormal3f(model.normals[i].x, model.normals[i].y, model.normals[i].z);
		}

		if (i < numOfTextureCoordinates) {
			glTexCoord2f(model.textureCoordinates[i].s, model.textureCoordinates[i].t);
		}

		glVertex3f(model.vertices[i].x, model.vertices[i].y, model.vertices[i].z);
	}

	glEnd();
}
