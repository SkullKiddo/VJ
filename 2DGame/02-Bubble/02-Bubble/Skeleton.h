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
	bool atacking;
	box hitBox;
	glm::ivec2 posSkeleton;





private:
	//dibuixar
	Texture spritesheet;
	Sprite *sprite;

	//dades
	bool alive;
	int lifes;
	bool vulnerable;

	//atacar
	bool chargingAtack;
	float timeChargingAtack;
	glm::ivec2 hitBoxOffset;

	//tamanys
	glm::ivec2 sizeSkeleton;
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2 colisionOffset;

	TileMap *map;
};

#endif