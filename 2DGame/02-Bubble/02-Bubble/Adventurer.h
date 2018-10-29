#ifndef _ADVENTURER_INCLUDE
#define _ADVENTURER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Adventurer
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void hit();

	void setTileMap(TileMap *tileMap);
	void setPosition();

private:
	int animSize[13] = {4,4,6,8,5,4,7,4,6,10,4,8,8};
	Texture spritesheet;
	Sprite *sprite;
	bool alive;
	int lifes;
	bool vulnerable;
	bool unshealthed;
	int timeBetweenAttacks;
	TileMap *map;
	glm::ivec2 posAdventurer;
	glm::ivec2 sizeAdventurer;
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2 colisionOffset;
};

#endif