#ifndef _BACKGROUND_INCLUDE
#define _BACKGROUND_INCLUDE

#include "Background.h"

Background::Background(const string image, ShaderProgram &shaderProgram, glm::vec2 pos) {
	texturaFons.loadFromFile(image, TEXTURE_PIXEL_FORMAT_RGBA);
	//spriteFons = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &texturaFons, &shaderProgram);
	//spriteFons->setPosition(pos);
}

#endif // _BACKGROUND_INCLUDE