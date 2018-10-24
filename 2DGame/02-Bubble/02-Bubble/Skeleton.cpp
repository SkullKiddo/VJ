#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skeleton.h"
#include "Game.h"

#define MOVEMENT_SPEED 8
#define SIZE 10
#define POSX 10
#define POSY 10
#define altframe 1.f/12.f
#define anchframe 1.f/18.f

enum PlayerAnims
{
	ATACK_RIGHT, ATACK_LEFT, REACT_RIGHT, REACT_LEFT, DIE_RIGHT, DIE_LEFT, IDDLE_RIGHT, IDDLE_LEFT, HIT_RIGHT, HIT_LEFT, MOVE_RIGHT, MOVE_LEFT, DIEDEDED_RIGHT, DIEDEDED_LEFT
};

int animSize[] = {18,4,15,11,8,13};

void Skeleton::hit() {
	if (vulnerable && alive) {
		vulnerable = false;
		if (lifes > 1) {
			lifes--;
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(HIT_RIGHT);
			else sprite->changeAnimation(HIT_LEFT);
		}
		else {
			lifes = 0;
			alive = false;
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);

		}
	}
}

void Skeleton::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
	//posSkeleton = tileMapPos;
	vulnerable = false;
	alive = true;
	lifes = 3;
	sizeSkeleton = glm::ivec2(43 * 3, 37 * 3);
	colisionBox.x = (sizeSkeleton.x *22.0f) / 68.0f;		//22 es els pixels quefa d'ample el personatge, 68 el tamany total del sprite
	colisionBox.y = (sizeSkeleton.y) / 37.0f;				//37 perque te 37 pixels i vull que sigui nomes un pixel de ample
	colisionOffset.x = (sizeSkeleton.x *21.0f) / 68.0f;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (sizeSkeleton.y) - colisionBox.y;	//aixo esta bé mentre es recolzi al terra per la part mes baixa (que en principi sera aixi amb tot personatge)
	posSkeleton = posInicial;
	spritesheet.loadFromFile("images/esqueleto.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(sizeSkeleton, glm::vec2(anchframe,altframe), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);

	for (int i = 0; i < 12; i+=2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i/2]; j++) 
			sprite->addKeyframe(i, glm::vec2(anchframe * j, altframe * i));
	}

	for (int i = 1; i < 12; i+=2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i/2]; j++)
			sprite->addKeyframe(i, glm::vec2(anchframe * (animSize[i/2]-1 - j), altframe * i));
	}

	sprite->setAnimationSpeed(DIEDEDED_RIGHT, MOVEMENT_SPEED);
	sprite->addKeyframe(DIEDEDED_RIGHT, glm::vec2(anchframe * 14, altframe * 4));

	sprite->setAnimationSpeed(DIEDEDED_LEFT, MOVEMENT_SPEED);
	sprite->addKeyframe(DIEDEDED_LEFT, glm::vec2(anchframe * 0, altframe * 5));
	

	sprite->changeAnimation(DIEDEDED_LEFT);
	//tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(posSkeleton.x, posSkeleton.y));
	//setPosition();
}

void Skeleton::update(int deltaTime)
{

	sprite->update(deltaTime);
	int anim = sprite->animation();
	bool dreta = anim % 2 == 0;
	if (sprite->finished()) vulnerable = true;
	if (alive) {
		if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != ATACK_LEFT && anim != ATACK_RIGHT)) {
			auto initialPos = posSkeleton;

			if (Game::instance().getKey('a')) {
				if (dreta) sprite->changeAnimation(ATACK_RIGHT);
				else sprite->changeAnimation(ATACK_LEFT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if (anim != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				posSkeleton.x += 1;
				if (map->collisionMoveRight(posSkeleton, colisionBox,colisionOffset))
				{
					posSkeleton.x -= 1;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				if (anim != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
				posSkeleton.x -= 1;
				if (map->collisionMoveLeft(posSkeleton, colisionBox, colisionOffset))
				{
					posSkeleton.x += 1;
				}
			}
			else if (!Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				if (dreta && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_RIGHT);
				else if (anim != IDDLE_LEFT && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_LEFT);
			}

			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				posSkeleton.y += 1;
				if (anim != MOVE_RIGHT && anim != MOVE_LEFT) {
					if (!dreta) sprite->changeAnimation(MOVE_LEFT);
					else sprite->changeAnimation(MOVE_RIGHT);
				}
				if (map->collisionMoveDown(posSkeleton, colisionBox, colisionOffset))
				{
					posSkeleton.y -= 1;
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				posSkeleton.y -= 1;
				if (anim != MOVE_RIGHT && anim != MOVE_LEFT) {
					if (!dreta) sprite->changeAnimation(MOVE_LEFT);
					else sprite->changeAnimation(MOVE_RIGHT);
				}
				if (map->collisionMoveUp(posSkeleton, colisionBox, colisionOffset))
				{
					posSkeleton.y += 1;
				}
			}
			else if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if (dreta && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_RIGHT);
				else if (anim != IDDLE_LEFT && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_LEFT);
			}
			if (initialPos == posSkeleton) {
				if (dreta) sprite->changeAnimation(IDDLE_RIGHT);
				else sprite->changeAnimation(IDDLE_LEFT);
			}
		}
	}
	else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (dreta) sprite->changeAnimation(DIEDEDED_RIGHT);
			else sprite->changeAnimation(DIEDEDED_LEFT);
		}
	}

	setPosition();
}

void Skeleton::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Skeleton::setPosition()
{
	sprite->setPosition(glm::vec2(float( posSkeleton.x), float(posSkeleton.y)));
}

void Skeleton::render()
{
	sprite->render();
}
