#pragma once

#include "Sprite.h"
#include "TileMap.h"


class Character
{
public:
	//definides
	virtual void render();
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
	bool attacking;
	glm::ivec2 pos;
	glm::ivec2 size, size_hearts;
	Character* target;

protected:

	//dibuixar
	Sprite *sprite;
	Texture spritesheet;
	Sprite *sprite_hearts;
	Texture spritesheet_hearts;
	Sprite *sprite_smoke;
	Texture spritesheet_smoke;

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
	bool attackTarget;
	virtual void killTarget(); //actualitza els bools per moures cap al taget o atacar

	//atacar
	bool chargingAttack;
	float timeChargingAttack;
	glm::ivec2 hitBoxOffset;
	bool hitted = false;

	//tamanys
	glm::ivec2 colisionBox;
	glm::ivec2 tileMapDispl;
	glm::ivec2 colisionOffset;

	TileMap *map;
};