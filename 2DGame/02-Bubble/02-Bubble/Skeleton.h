#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Skeleton
{
public:
	//functions
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void hit();
	void setTileMap(TileMap *tileMap);
	void setPosition();
	box calcHurtBox();

	//attributes
	bool atacking;
	box hitBox;
	glm::ivec2 pos;





private:
	//dibuixar
	Texture spritesheet;
	Sprite *sprite;

	//dades
	bool alive;
	int lifes;
	bool vulnerable;
	bool dreta;

	//atacar
	bool chargingAtack;
	float timeChargingAtack;
	glm::ivec2 hitBoxOffset;

	//tamanys
	glm::ivec2 size;
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2 colisionOffset;

	TileMap *map;
};

#endif