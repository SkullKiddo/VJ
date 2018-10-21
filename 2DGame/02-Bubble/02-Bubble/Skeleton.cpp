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
	IDDLE_LEFT, IDDLE_RIGHT, MOVE_LEFT, MOVE_RIGHT,DIE_RIGHT
};

void Skeleton::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/esqueleto.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(43*3, 37*3), glm::vec2(anchframe,altframe), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);
		
		
		sprite->setAnimationSpeed(IDDLE_LEFT, MOVEMENT_SPEED);
		for (int i = 0; i < 18; i++) {
			sprite->addKeyframe(IDDLE_LEFT, glm::vec2(anchframe * i, altframe * 0));
		}

		sprite->setAnimationSpeed(IDDLE_RIGHT, MOVEMENT_SPEED);
		for (int i = 0; i < 18; i++) {
			sprite->addKeyframe(IDDLE_LEFT, glm::vec2(anchframe * 17-i, altframe * 1));
		}

		sprite->setAnimationSpeed(DIE_RIGHT, MOVEMENT_SPEED);
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 0, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 1, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 2, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 3, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 4, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 5, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 6, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 7, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 8, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 9, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 10, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 11, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 12, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 13, altframe * 4));
		sprite->addKeyframe(DIE_RIGHT, glm::vec2(anchframe * 14, altframe * 4));




	sprite->changeAnimation(IDDLE_LEFT);
	//glm::ivec2 tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(20, 20));
}

void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Skeleton::render()
{
	sprite->render();
}
