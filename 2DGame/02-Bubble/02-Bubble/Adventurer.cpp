#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Adventurer.h"
#include "Game.h"

#define MOVEMENT_SPEED 8
#define SIZE 10
#define POSX 10
#define POSY 10
#define altframe 1.f/26.f
#define anchframe 1.f/10.f

#define MAXTIMEBETWEENATTACKS 2000

enum PlayerAnims
{
	IDLE_RIGHT, IDLE_LEFT, CROUCH_RIGHT, CROUCH_LEFT, RUN_RIGHT, RUN_LEFT, JUMP_RIGHT, JUMP_LEFT,
	SLIDE_RIGHT, SLIDE_LEFT, UNSHEATHED_RIGHT, UNSHEATHED_LEFT, ATTACK_1_RIGHT, ATTACK_1_LEFT,
	ATTACK_2_RIGHT, ATTACK_2_LEFT, ATTACK_3_RIGHT, ATTACK_3_LEFT, DIE_RIGHT, DIE_LEFT, 
	DRAW_RIGHT, DRAW_LEFT, CAST_RIGHT, CAST_LEFT, JUMP_ATTACK_RIGHT, JUMP_ATTACK_LEFT
};

void Adventurer::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
	vulnerable = false;
	alive = true;
	unshealthed = false;
	lifes = 3;
	sizeAdventurer = glm::ivec2(50*2, 37*2);
	posAdventurer = posInicial;
	spritesheet.loadFromFile("images/adventurer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(sizeAdventurer, glm::vec2(anchframe, altframe), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(26);

	for (int i = 0; i < 26; i += 2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i / 2]; ++j)
			sprite->addKeyframe(i, glm::vec2(anchframe * j, altframe * i));
	}

	for (int i = 1; i < 26; i += 2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i / 2]; ++j)
			sprite->addKeyframe(i, glm::vec2(anchframe * (animSize[i / 2] - 1 - j), altframe * i));
	}

	sprite->setPosition(glm::vec2(posAdventurer.x, posAdventurer.y));
}

void Adventurer::update(int deltaTime) {
	timeBetweenAttacks += deltaTime;
	sprite->update(deltaTime);
	int anim = sprite->animation();
	bool right = anim % 2 == 0;
	if (sprite->finished()) vulnerable = true;
	if (alive) {
		if (sprite->finished() || (anim != ATTACK_1_LEFT && anim != ATTACK_1_RIGHT && anim != ATTACK_2_LEFT
			&& anim != ATTACK_2_RIGHT && anim != ATTACK_3_LEFT && anim != ATTACK_3_RIGHT
			&& anim != DRAW_LEFT && anim != DRAW_RIGHT && anim != JUMP_ATTACK_LEFT && anim != JUMP_ATTACK_RIGHT))
		{
			auto initialPos = posAdventurer;

			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				posAdventurer.x += 1;
				if (unshealthed) {
					if (right) sprite->changeAnimation(DRAW_RIGHT);
					else sprite->changeAnimation(DRAW_LEFT);
					unshealthed = false;
				}
				else {
					if (anim != RUN_RIGHT)
						sprite->changeAnimation(RUN_RIGHT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				posAdventurer.x -= 1;
				if (unshealthed) {
					if (right) sprite->changeAnimation(DRAW_RIGHT);
					else sprite->changeAnimation(DRAW_LEFT);
					unshealthed = false;
				}
				else {
					if (anim != RUN_LEFT)
						sprite->changeAnimation(RUN_LEFT);
				}
			}

			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				posAdventurer.y += 1;
				if (unshealthed) {
					if (right) sprite->changeAnimation(DRAW_RIGHT);
					else sprite->changeAnimation(DRAW_LEFT);
					unshealthed = false;
				}
				else {
					if (anim != RUN_RIGHT && anim != RUN_LEFT) {
						if (right) sprite->changeAnimation(RUN_RIGHT);
						else sprite->changeAnimation(RUN_LEFT);
					}
				}
			}

			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				posAdventurer.y -= 1;
				if (unshealthed) {
					if (right) sprite->changeAnimation(DRAW_RIGHT);
					else sprite->changeAnimation(DRAW_LEFT);
					unshealthed = false;
				}
				else {
					if (anim != RUN_RIGHT && anim != RUN_LEFT) {
						if (right) sprite->changeAnimation(RUN_RIGHT);
						else sprite->changeAnimation(RUN_LEFT);
					}
				}
			}

			if (initialPos == posAdventurer && unshealthed && anim != UNSHEATHED_LEFT && anim != UNSHEATHED_RIGHT) {
				if (right) sprite->changeAnimation(UNSHEATHED_RIGHT);
				else sprite->changeAnimation(UNSHEATHED_LEFT);
			}

			else if (initialPos == posAdventurer && !unshealthed && anim != IDLE_LEFT && anim != IDLE_RIGHT) {
				if (right) sprite->changeAnimation(IDLE_RIGHT);
				else sprite->changeAnimation(IDLE_LEFT);
			}

			if (Game::instance().getKey('a')) {

				unshealthed = true;
				if (timeBetweenAttacks > MAXTIMEBETWEENATTACKS) {
					if (right) sprite->changeAnimation(ATTACK_1_RIGHT);
					else sprite->changeAnimation(ATTACK_1_LEFT);
					timeBetweenAttacks = 0;
				}
				else {
					if (sprite->previousAnimation() == ATTACK_1_RIGHT || sprite->previousAnimation() == ATTACK_1_LEFT) {
						if (right) sprite->changeAnimation(ATTACK_2_RIGHT);
						else sprite->changeAnimation(ATTACK_2_LEFT);
					}
					else if (sprite->previousAnimation() == ATTACK_2_RIGHT || sprite->previousAnimation() == ATTACK_2_LEFT) {
						if (right) sprite->changeAnimation(ATTACK_3_RIGHT);
						else sprite->changeAnimation(ATTACK_3_LEFT);
					}
					else {
						if (right) sprite->changeAnimation(ATTACK_1_RIGHT);
						else sprite->changeAnimation(ATTACK_1_LEFT);
					}
					timeBetweenAttacks = 0;
				}
			}

			else if (Game::instance().getKey('s')) {
				if (right) sprite->changeAnimation(DRAW_RIGHT);
				else sprite->changeAnimation(DRAW_LEFT);
			}
		}
	}

	else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (!right) sprite->changeAnimation(DIE_LEFT);
			else sprite->changeAnimation(DIE_RIGHT);
		}
	}

	setPosition();
}


void Adventurer::setPosition()
{
	sprite->setPosition(glm::vec2(float(posAdventurer.x), float(posAdventurer.y)));
}

void Adventurer::render() {
	sprite->render();
}

void Adventurer::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}