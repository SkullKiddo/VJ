#ifndef _HBANDIT_INCLUDE
#define _HBANDIT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class HeavyBandit
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void hit();

	void setTileMap(TileMap *tileMap);
	void setPosition();
	glm::ivec2 pos;
	box hitBox;
	box calcHurtBox();
	bool atacking;





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
	bool dreta;

	//atacar
	bool chargingAtack;
	float timeChargingAtack;
	glm::ivec2 hitBoxOffset;

};

#endif