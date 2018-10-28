#ifndef _LBANDIT_INCLUDE
#define _LBANDIT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class LightBandit
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void hit();

	void setTileMap(TileMap *tileMap);
	void setPosition();
	glm::ivec2 pos;





private:
	Texture spritesheet;
	Sprite *sprite;
	bool alive;
	int lifes;
	bool vulnerable;
	TileMap *map;
	glm::ivec2 size;
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2  colisionOffset;
};

#endif