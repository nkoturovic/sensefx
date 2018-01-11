#ifndef SENSEFX_TEXTURE_H
#define SENSEFX_TEXTURE_H

#include "image.h"
#include <string>
#include <map>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Texture2D {

	private:
		GLuint textureName;
		bool repeatS = true;
		bool repeatT = true;
	
	public:
		static std::map<std::string, Texture2D> importAll(std::string dirPath);

		void init(Image * image, bool repeatS, bool repeatT);
		Texture2D(Image * image, bool repeatS, bool repeatT);
		Texture2D(Image * image);
		Texture2D(std::string path, bool repeatS, bool repeatT);
		Texture2D(std::string path);
		Texture2D();

		GLuint getTextureName();
		bool isRepeatingS();
		bool isRepeatingT();

		void bind();
		void static unbind();
		void remove();

};

#endif // SENSEFX_TEXTURE_H

