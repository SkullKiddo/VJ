#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Character
{
public:
	//definides
	void render();
	box hurtBox();
	void setPosition();
	void setTileMap(TileMap *tileMap);
	bool canHit(Character* target);

	//virtuals
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {};
	virtual void update(int deltaTime) {};
	virtual void hit() {};
	virtual box hitBox();

	//attributes
	bool atacking;
	glm::ivec2 pos;
	glm::ivec2 size, size_hearts;
	Character* target;

protected:

	//dibuixar
	Sprite *sprite;
	Texture spritesheet;
	Sprite *sprite_hearts;
	Texture spritesheet_hearts;

	//dades
	bool alive;
	int lifes;
	bool vulnerable;
	int speed;
	bool dreta;

	//IA
	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;
	bool atackTarguet;
	void killTarget(); //actualitza els bools per moures cap al taget o atacar

	//atacar
	bool chargingAtack;
	float timeChargingAtack;
	glm::ivec2 hitBoxOffset;
	bool hitted = false;

	//tamanys
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2 colisionOffset;

	TileMap *map;
};