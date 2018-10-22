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
	void hit();

	void setTileMap(TileMap *tileMap);
	void setPosition();

	glm::ivec2 posSkeleton;


private:
	Texture spritesheet;
	Sprite *sprite;
	bool alive;
	int lifes;
	bool vulnerable;
	TileMap *map;

	glm::ivec2 tileMapDispl;
};

#endif