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

void Skeleton::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	
	vulnerable = false;
	alive = true;
	lifes = 3;

	spritesheet.loadFromFile("images/esqueleto.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(43*3, 37*3), glm::vec2(anchframe,altframe), &spritesheet, &shaderProgram);
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
	//glm::ivec2 tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(20, 20));
}

void Skeleton::update(int deltaTime)
{

	sprite->update(deltaTime);
	int anim = sprite->animation();
	if (sprite->finished()) vulnerable = true;
	if(alive) {
		if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != ATACK_LEFT && anim != ATACK_RIGHT)) {

			if (Game::instance().getKey('a')) {
				if (anim % 2 == 0) sprite->changeAnimation(ATACK_RIGHT);
				else sprite->changeAnimation(ATACK_LEFT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				if (anim != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if (anim != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			else
			{
				if (anim%2 == 0 && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_RIGHT);
				else if (anim != IDDLE_LEFT && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_LEFT);
			}
		}
	}
	else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (anim % 2 == 0) sprite->changeAnimation(DIEDEDED_RIGHT);
			else sprite->changeAnimation(DIEDEDED_LEFT);
		}
	}
}

void Skeleton::render()
{
	sprite->render();
}
