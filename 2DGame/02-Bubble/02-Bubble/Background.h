#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "ShaderProgram.h"
#include <string>

class Background
{
public:
	Background(const string image, ShaderProgram &shaderProgram, glm::vec2 pos);

private:
	Texture texturaFons;
	Sprite *spriteFons;
};
