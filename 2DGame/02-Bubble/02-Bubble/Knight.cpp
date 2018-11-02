#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Knight.h"
#include "Game.h"
#include <Mmsystem.h>

#define MOVEMENT_SPEED 8
#define SIZE 10
#define POSX 10
#define POSY 10
#define ALT_FRAME 1.f/12.f
#define ANCH_FRAME 1.f/10.f
#define ANCH_FRAME_PIXELS 80
#define ALT_FRAME_PIXELS 42

#define MAX_TIME_BETWEEN_ATTACKS 2000

#define ATTACK_CHARGING_TIME (1000.f/MOVEMENT_SPEED) *5.f

enum KnightAnims
{
	ATTACK_RIGHT, ATTACK_LEFT, MOVE_RIGHT, MOVE_LEFT, RISE_SHIELD_RIGHT, RISE_SHIELD_LEFT, DIE_RIGHT,
	DIE_LEFT, IDLE_RIGHT, IDLE_LEFT, SHIELD_STRIKE_RIGHT, SHIELD_STRIKE_LEFT, LOWER_SHIELD_RIGHT, LOWER_SHIELD_LEFT, HIT_RIGHT, HIT_LEFT
};

void Knight::hit() {
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
			sprite->stay();

		}
	}
}

void Knight::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
	vulnerable = false;
	alive = true;
	lifes = 3;
	size = glm::ivec2(50 * 3, 37 * 3);
	colisionBox.x = size.x;
	colisionBox.y = size.y / ALT_FRAME_PIXELS;
	colisionOffset.x = (size.x *33.0f) / ANCH_FRAME_PIXELS;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (size.y) - colisionBox.y;
	pos = posInicial;
	spritesheet.loadFromFile("images/knight.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(size, glm::vec2(ANCH_FRAME, ALT_FRAME), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(15);

	for (int i = 0; i < 12; i += 2) {
		sprite->setAnimationSpeed(i, animSpeed[i/2]);
		for (int j = 0; j < animSize[i / 2]; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * j, ALT_FRAME * i));
	}

	for (int i = 1; i < 12; i += 2) {
		sprite->setAnimationSpeed(i, animSpeed[i/2]);
		for (int j = 0; j < animSize[i / 2]; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * (animSize[i / 2] - 1 - j), ALT_FRAME * i));
	}


	sprite->setAnimationSpeed(HIT_RIGHT, MOVEMENT_SPEED);
	sprite->setAnimationSpeed(HIT_LEFT, MOVEMENT_SPEED);
	for (int j = 0; j < 3; ++j) {
		sprite->addKeyframe(HIT_RIGHT, glm::vec2(ANCH_FRAME * j, ALT_FRAME * DIE_RIGHT));
		sprite->addKeyframe(HIT_LEFT, glm::vec2(ANCH_FRAME * (10 - 1 - j), ALT_FRAME * DIE_LEFT));
	}

	sprite->setAnimationSpeed(LOWER_SHIELD_RIGHT, animSpeed[RISE_SHIELD_RIGHT / 2]);
	for (int j = 0; j < animSize[RISE_SHIELD_RIGHT / 2]; j++)
		sprite->addKeyframe(LOWER_SHIELD_RIGHT, glm::vec2(ANCH_FRAME * (animSize[LOWER_SHIELD_RIGHT / 2] - 2 - j), ALT_FRAME * RISE_SHIELD_RIGHT));

	sprite->setAnimationSpeed(LOWER_SHIELD_LEFT, animSpeed[RISE_SHIELD_LEFT / 2]);
	for (int j = 0; j < animSize[RISE_SHIELD_LEFT / 2]; j++)
		sprite->addKeyframe(LOWER_SHIELD_LEFT, glm::vec2(ANCH_FRAME * (j), ALT_FRAME * RISE_SHIELD_LEFT));



	sprite->setPosition(glm::vec2(pos.x, pos.y));
	sprite->changeAnimation(LOWER_SHIELD_RIGHT);
	//sprite->stay();
}

void Knight::update(int deltaTime) {
	sprite->update(deltaTime);
	//float debug = ATTACK_CHARGING_TIME;
	//attacking = false;
	//int anim = sprite->animation();
	//dreta = anim % 2 == 0;
	//if (sprite->finished()) vulnerable = true;
	//if (alive) {
	//	if (blocking) {
	//		if (!Game::instance().getKey('s')) {
	//			blocking = false;
	//			if (dreta) sprite->changeAnimation(LOWER_SHIELD_RIGHT);
	//			else sprite->changeAnimation(LOWER_SHIELD_LEFT);
	//		}
	//		if (Game::instance().getKey('a') && (sprite->finished() || anim != SHIELD_STRIKE_RIGHT && anim != SHIELD_STRIKE_LEFT)) {
	//			if (dreta) sprite->changeAnimation(SHIELD_STRIKE_RIGHT);
	//			else sprite->changeAnimation(SHIELD_STRIKE_LEFT);
	//			sprite->stay();
	//		}
	//	}
	//	else if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != ATTACK_LEFT && anim != ATTACK_RIGHT && anim != LOWER_SHIELD_LEFT && anim != LOWER_SHIELD_RIGHT && anim != RISE_SHIELD_LEFT && anim != RISE_SHIELD_RIGHT)) {
	//		auto initialPos = pos;

	//		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	//		{
	//			dreta = true;
	//			pos.x += 1;
	//			if (map->collisionMoveRight(pos, colisionBox, colisionOffset))
	//			{
	//				pos.x -= 1;
	//			}
	//			else if (anim != MOVE_RIGHT)
	//				sprite->changeAnimation(MOVE_RIGHT);
	//		}
	//		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	//		{
	//			dreta = false;
	//			pos.x -= 1;
	//			if (map->collisionMoveLeft(pos, colisionBox, colisionOffset))
	//			{
	//				pos.x += 1;
	//			}
	//			else if (anim != MOVE_LEFT)
	//				sprite->changeAnimation(MOVE_LEFT);
	//		}
	//		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	//		{
	//			pos.y += 1;
	//			if (map->collisionMoveDown(pos, colisionBox, colisionOffset))
	//			{
	//				pos.y -= 1;
	//			}
	//			else if (anim != MOVE_RIGHT && anim != MOVE_LEFT) {
	//				if (!dreta) sprite->changeAnimation(MOVE_LEFT);
	//				else sprite->changeAnimation(MOVE_RIGHT);
	//			}
	//		}
	//		else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	//		{
	//			pos.y -= 1;
	//			if (map->collisionMoveUp(pos, colisionBox, colisionOffset))
	//			{
	//				pos.y += 1;
	//			}
	//			else if (anim != MOVE_RIGHT && anim != MOVE_LEFT) {
	//				if (!dreta) sprite->changeAnimation(MOVE_LEFT);
	//				else sprite->changeAnimation(MOVE_RIGHT);
	//			}
	//		}

	//		if (Game::instance().getKey('a')) {
	//			chargingAttack = true;
	//			//PlaySound(TEXT("audio/axeSwingCutre.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
	//			if (dreta) sprite->changeAnimation(ATTACK_RIGHT);
	//			else sprite->changeAnimation(ATTACK_LEFT);
	//		}

	//		if (Game::instance().getKey('s')) {
	//			blocking = true;
	//			if (dreta) sprite->changeAnimation(RISE_SHIELD_RIGHT);
	//			else sprite->changeAnimation(RISE_SHIELD_LEFT);
	//			sprite->stay();
	//		}

	//		if (initialPos == pos) {
	//			if (dreta && anim != IDLE_RIGHT) sprite->changeAnimation(IDLE_RIGHT);
	//			else if (!dreta && anim != IDLE_LEFT) sprite->changeAnimation(IDLE_LEFT);
	//		}
	//	}
	//}
	//else {
	//}
	//if (chargingAttack) {
	//	timeChargingAttack+= deltaTime;
	//	if (timeChargingAttack> ATTACK_CHARGING_TIME) {
	//		PlaySound(TEXT("audio/hit_placeholder.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
	//		attacking = true;
	//		chargingAttack= false;
	//		timeChargingAttack= 0.f;
	//	}
	//}
	//setPosition();
}

box Knight::hitBox() {
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