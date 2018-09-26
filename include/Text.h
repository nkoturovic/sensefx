#ifndef SENSEFX_TEXT_H
#define SENSEFX_TEXT_H

#include "glm/glm.hpp"
#include <string>

class Text {

	private:
		std::string text;
		glm::vec2 position;
		glm::vec3 color;

	public:

		Text();
		Text(glm::vec2 position, glm::vec3 color, std::string text);

		void setText(std::string text);
		void setPosition(glm::vec2 position);
		void setColor(glm::vec3 color);

		std::string getText();
		glm::vec2 getPosition();
		glm::vec3 getColor();

		void print(glm::vec2 screenSize);
};


#endif
