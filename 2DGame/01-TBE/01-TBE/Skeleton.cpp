#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Skeleton.h"

#define MOVEMENT_SPEED 10
#define SIZE 10
#define POSX 10
#define POSY 10

enum PlayerAnims
{
	IDDLE_LEFT, IDDLE_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Skeleton::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/skeletonIddle.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0f / 11.0f, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDDLE_LEFT, MOVEMENT_SPEED);
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	/* RESTA DE FRAMES
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDDLE_LEFT, glm::vec2(0.f, 0.f));
	*/

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
