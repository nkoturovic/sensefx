#ifndef SENSEFX_VARIOUS_OBJECTS_H
#define SENSEFX_VARIOUS_OBJECTS_H

#include "Object.h"
#include "DrawableObject.h"
#include "ColidableObject.h"
#include "TransformableObject.h"
#include "TexturedObject.h"
#include "User.h"

/* Colidable-Transformable object */
class CTObject : public ColidableObject, public TransformableObject {

};


class CTexObject : public ColidableObject, public TexturedObject {


};

class Room : public TransformableObject , public DrawableObject {

	private:
		std::vector <Object *> collisionList = {&frontWall, &backWall, &leftWall, &rightWall, &ceilingColider, &floorColider};

		CTObject floorColider, ceilingColider;
		CTexObject frontWall, backWall, leftWall, rightWall;
		TexturedObject ceiling, floor;

		/* Implicitno default parametri */
		glm::vec4 color = glm::vec4(1,1,1,1);
		Model cube = Model("resources/models/cube.obj");
		Material material = Material("resources/materials/normal.mat");
		Texture2D wallTexture = Texture2D("resources/textures/wall.bmp");
		Texture2D floorTexture = Texture2D("resources/textures/floor.bmp");
		Texture2D ceilingTexture = Texture2D("resources/textures/ceiling.bmp");


		float wallThickness = 0.05;
		float translateCoef = 1.0;


	public:
		Room();
		void drawObject();
		void init();
		void setDimensions(glm::vec3 lwh);

		std::vector <Object *> getColisionList();
};

#endif // SENSEFX_VARIOUS_OBJECTS_H
