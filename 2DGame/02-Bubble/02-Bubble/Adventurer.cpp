#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Adventurer.h"
#include "Game.h"
#include <Mmsystem.h>

#define MOVEMENT_SPEED 8
#define SIZE 10
#define POSX 10
#define POSY 10
#define ALTFRAME 1.f/26.f
#define ANCHFRAME 1.f/10.f
#define ANCH_FRAME_PIXELS 50
#define ALT_FRAME_PIXELS 37

#define MAX_TIME_BETWEEN_ATTACKS 2000

#define ATACK_1_3_CHARGING_TIME (1000.f/MOVEMENT_SPEED) *2.f
#define ATACK_2_CHARGING_TIME (1000.f/MOVEMENT_SPEED)

enum PlayerAnims
{
	IDLE_RIGHT, IDLE_LEFT, CROUCH_RIGHT, CROUCH_LEFT, RUN_RIGHT, RUN_LEFT, JUMP_RIGHT, JUMP_LEFT,
	SLIDE_RIGHT, SLIDE_LEFT, UNSHEATHED_RIGHT, UNSHEATHED_LEFT, ATTACK_1_RIGHT, ATTACK_1_LEFT,
	ATTACK_2_RIGHT, ATTACK_2_LEFT, ATTACK_3_RIGHT, ATTACK_3_LEFT, DIE_RIGHT, DIE_LEFT, 
	DRAW_RIGHT, DRAW_LEFT, CAST_RIGHT, CAST_LEFT, JUMP_ATTACK_RIGHT, JUMP_ATTACK_LEFT, HIT_RIGHT, HIT_LEFT, DIEDEDEDEDED_RIGHT, DIEDEDEDEDED_LEFT
};

void Adventurer::hit() {
	if (vulnerable && alive) {
		chargingAttack = false;
		vulnerable = false;
		if (lifes > 1) {
			lifes--;
			if (dreta) sprite->changeAnimation(HIT_RIGHT);
			else sprite->changeAnimation(HIT_LEFT);
		}
		else {
			lifes = 0;
			alive = false;
			if (dreta) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);

		}
	}
}

void Adventurer::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
	vulnerable = false;
	alive = true;
	unshealthed = false;
	lifes = 3;
	size = glm::ivec2(50*2, 37*2);
	colisionBox.x = size.x;
	colisionBox.y = size.y / ALT_FRAME_PIXELS;
	colisionOffset.x = (size.x *16.0f) / ANCH_FRAME_PIXELS;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (size.y) - colisionBox.y;
	pos = posInicial;
	spritesheet.loadFromFile("images/adventurer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(size, glm::vec2(ANCHFRAME, ALTFRAME), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(30);

	for (int i = 0; i < 26; i += 2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i / 2]; ++j)
			sprite->addKeyframe(i, glm::vec2(ANCHFRAME * j, ALTFRAME * i));
	}

	for (int i = 1; i < 26; i += 2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i / 2]; ++j)
			sprite->addKeyframe(i, glm::vec2(ANCHFRAME * (animSize[i / 2] - 1 - j), ALTFRAME * i));
	}

	sprite->setAnimationSpeed(HIT_RIGHT, MOVEMENT_SPEED);
	for (int j = 0; j < 4; ++j) sprite->addKeyframe(HIT_RIGHT, glm::vec2(ANCHFRAME * j, ALTFRAME * DIE_RIGHT));

	sprite->setAnimationSpeed(HIT_LEFT, MOVEMENT_SPEED);
	for (int j = 0; j < 4; ++j) sprite->addKeyframe(HIT_LEFT, glm::vec2(ANCHFRAME * (10 - 1 - j), ALTFRAME * DIE_LEFT));

	sprite->setAnimationSpeed(DIEDEDEDEDED_RIGHT, MOVEMENT_SPEED);
	sprite->addKeyframe(DIEDEDEDEDED_RIGHT, glm::vec2(ANCHFRAME * (9), ALTFRAME * DIE_RIGHT));

	sprite->setAnimationSpeed(DIEDEDEDEDED_LEFT, MOVEMENT_SPEED);
	for (int j = 0; j < 4; ++j) sprite->addKeyframe(DIEDEDEDEDED_LEFT, glm::vec2(0, ALTFRAME * DIE_LEFT));

	sprite->setPosition(glm::vec2(pos.x, pos.y));
}

void Adventurer::update(int deltaTime) {
	attacking = false;
	timeBetweenAttacks += deltaTime;
	sprite->update(deltaTime);
	int anim = sprite->animation();
	dreta = anim % 2 == 0;
	if (sprite->finished()) vulnerable = true;
	if (alive) {
		if (sprite->finished() || (anim != ATTACK_1_LEFT && anim != ATTACK_1_RIGHT && anim != ATTACK_2_LEFT
			&& anim != ATTACK_2_RIGHT && anim != ATTACK_3_LEFT && anim != ATTACK_3_RIGHT
			&& anim != DRAW_LEFT && anim != DRAW_RIGHT && anim != JUMP_ATTACK_LEFT && anim != JUMP_ATTACK_RIGHT && anim != HIT_LEFT
			&& anim != HIT_RIGHT))
		{
			auto initialPos = pos;

			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				pos.x += 1;
				if (map->collisionMoveRight(pos, colisionBox, colisionOffset))
				{
					pos.x -= 1;
				}
				else {
					if (unshealthed) {
						if (dreta) sprite->changeAnimation(DRAW_RIGHT);
						else sprite->changeAnimation(DRAW_LEFT);
						unshealthed = false;
					}
					else {
						if (anim != RUN_RIGHT)
							sprite->changeAnimation(RUN_RIGHT);
					}
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				pos.x -= 1;
				if (map->collisionMoveLeft(pos, colisionBox, colisionOffset))
				{
					pos.x += 1;
				}
				else {
					if (unshealthed) {
						if (dreta) sprite->changeAnimation(DRAW_RIGHT);
						else sprite->changeAnimation(DRAW_LEFT);
						unshealthed = false;
					}
					else {
						if (anim != RUN_LEFT)
							sprite->changeAnimation(RUN_LEFT);
					}
				}
			}

			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				pos.y += 1;
				if (map->collisionMoveDown(pos, colisionBox, colisionOffset))
				{
					pos.y -= 1;
				}
				else {
					if (unshealthed) {
						if (dreta) sprite->changeAnimation(DRAW_RIGHT);
						else sprite->changeAnimation(DRAW_LEFT);
						unshealthed = false;
					}
					else {
						if (anim != RUN_RIGHT && anim != RUN_LEFT) {
							if (dreta) sprite->changeAnimation(RUN_RIGHT);
							else sprite->changeAnimation(RUN_LEFT);
						}
					}
				}
			}

			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				pos.y -= 1;
				if (map->collisionMoveUp(pos, colisionBox, colisionOffset))
				{
					pos.y += 1;
				}
				else {
					if (unshealthed) {
						if (dreta) sprite->changeAnimation(DRAW_RIGHT);
						else sprite->changeAnimation(DRAW_LEFT);
						unshealthed = false;
					}
					else {
						if (anim != RUN_RIGHT && anim != RUN_LEFT) {
							if (dreta) sprite->changeAnimation(RUN_RIGHT);
							else sprite->changeAnimation(RUN_LEFT);
						}
					}
				}
			}

			if (initialPos == pos && unshealthed && anim != UNSHEATHED_LEFT && anim != UNSHEATHED_RIGHT) {
				if (dreta) sprite->changeAnimation(UNSHEATHED_RIGHT);
				else sprite->changeAnimation(UNSHEATHED_LEFT);
			}

			else if (initialPos == pos && !unshealthed && anim != IDLE_LEFT && anim != IDLE_RIGHT) {
				if (dreta) sprite->changeAnimation(IDLE_RIGHT);
				else sprite->changeAnimation(IDLE_LEFT);
			}

			if (Game::instance().getKey('a')) {

				unshealthed = true;
				if (timeBetweenAttacks > MAX_TIME_BETWEEN_ATTACKS) {
					if (dreta) sprite->changeAnimation(ATTACK_1_RIGHT);
					else sprite->changeAnimation(ATTACK_1_LEFT);
					chargingAtack1 = true;
				}
				else {

					if (sprite->previousAnimation() == ATTACK_1_RIGHT || sprite->previousAnimation() == ATTACK_1_LEFT) {
						if (dreta) sprite->changeAnimation(ATTACK_2_RIGHT);
						else sprite->changeAnimation(ATTACK_2_LEFT);
						chargingAtack2 = true;
					}
					else if (sprite->previousAnimation() == ATTACK_2_RIGHT || sprite->previousAnimation() == ATTACK_2_LEFT) {
						if (dreta) sprite->changeAnimation(ATTACK_3_RIGHT);
						else sprite->changeAnimation(ATTACK_3_LEFT);
						chargingAtack3 = true;
					}
					else {
						if (dreta) sprite->changeAnimation(ATTACK_1_RIGHT);
						else sprite->changeAnimation(ATTACK_1_LEFT);
					}
				}
				timeBetweenAttacks = 0;
			}
			if (chargingAtack1 || chargingAtack2 || chargingAtack3) {
				timeChargingAttack += deltaTime;
				if ((chargingAtack2 && timeChargingAttack > ATACK_2_CHARGING_TIME) || (timeChargingAttack > ATACK_1_3_CHARGING_TIME)) {
					attacking = true;
					PlaySound(TEXT("audio/hit_placeholder.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
					chargingAtack1 = chargingAtack2 = chargingAtack3 = false;
				}
			}

			else if (Game::instance().getKey('s')) {
				if (dreta) sprite->changeAnimation(DRAW_RIGHT);
				else sprite->changeAnimation(DRAW_LEFT);
			}
		}
	}
	else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (!dreta) sprite->changeAnimation(DIEDEDEDEDED_LEFT);
			else sprite->changeAnimation(DIEDEDEDEDED_RIGHT);
		}
	}

	setPosition();
}

box Adventurer::hitBox() {
	box hitBox;
	if (dreta) {
		hitBox.mins = glm::ivec2(pos.x + size.x - colisionOffset.x, pos.y); //no foto offsets ni res perque ocupa tota la vertical
		hitBox.maxs = glm::ivec2(pos.x + size.x, pos.y + size.y);
	}
	else {
		hitBox.mins = glm::ivec2(pos.x, pos.y);
		hitBox.maxs = glm::ivec2(pos.x + colisionOffset.x, pos.y + size.y);
	}
	return hitBox;
}