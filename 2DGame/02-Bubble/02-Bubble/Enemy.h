#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Enemy
{
public:
	void render();
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {};
	virtual void update(int deltaTime) {};
	virtual void hit() {};
	box calcHurtBox();
	void setPosition();
	void setTileMap(TileMap *tileMap);


	//attributes
	bool atacking;
	box hitBox;
	glm::ivec2 pos;
	glm::ivec2 size;

protected:
	//dibuixar
	Sprite *sprite;
	Texture spritesheet;

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
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2 colisionOffset;

	TileMap *map;
};