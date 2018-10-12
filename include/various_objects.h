#ifndef SENSEFX_VARIOUS_OBJECTS_H
#define SENSEFX_VARIOUS_OBJECTS_H

#include "Object.h"
#include "DrawableObject.h"
#include "ColidableObject.h"
#include "TransformableObject.h"
#include "TexturedObject.h"
#include "User.h"
#include "DataContainer.h"
#include "AnimatedObject.h"

/* Colidable-Transformable object */
class CTObject : public ColidableObject, public TransformableObject {

};


class CTexObject : public ColidableObject, public TexturedObject {


};

class Room : public TransformableObject , public DrawableObject {

	private:

		CTexObject frontWall, backWall, leftWall, rightWall;
		CTObject floorColider, ceilingColider;
		TexturedObject ceiling, floor;

		std::vector <Object *> collisionList = {&frontWall, &backWall, &leftWall, &rightWall, &ceilingColider, &floorColider};

		/* Implicitno default parametri */
		glm::vec4 color = glm::vec4(1,1,1,1);

		/* Default values */
		Model cube = Model("resources/models/cube.obj");
		Material material;
		Texture2D wallTexture;
		Texture2D floorTexture;
		Texture2D ceilingTexture;


		float wallThickness = 0.05;
		float translateCoef = 1.0;


	public:
		Room();
		void drawObject();
		void init();
		void setDimensions(glm::vec3 lwh);

		std::vector <Object *> getColisionList();
};


class FileObject : public TexturedObject, public ColidableObject, public AnimatedObject {

	protected:
		bool isGlowing=false;

	public:
		std::string path;
		std::string name;

		FileObject(std::string path);
		
		void animate();

		/* Default File action */
		virtual void action();
		void startGlowing();
		void stopGlowing();

		static std::vector <FileObject *> importAll(std::string dirPath, int maxFiles, DataContainer * gd);
};


class DirectoryObject : public FileObject {
	public:
		DirectoryObject(std::string Path);

	/* Directory action, resetuje FXSimulaciju sa drugim direktorijumom */
	void action();

};


class RegularFileObject : public FileObject {
	public:
		std::string extension;
		RegularFileObject(std::string Path);

		void action();
};

#endif // SENSEFX_VARIOUS_OBJECTS_H
