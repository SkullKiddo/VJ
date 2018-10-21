#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Skeleton
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:
	Texture spritesheet;
	Sprite *sprite;
};

#endif