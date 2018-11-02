#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Wizard.h"
#include "Game.h"
#include <Mmsystem.h>
#include "Texture.h"

#define MOVEMENT_SPEED 8.f
#define SIZE 10
#define POSX 10
#define POSY 10
#define ALT_FRAME 1.f/12.f
#define ANCH_FRAME 1.f/10.f
#define ALT_FRAME_PIXELS 81.f
#define ANCH_FRAME_PIXELS 80.f
#define ATTACK_CHARGING_TIME (1000.f/MOVEMENT_SPEED) *7.f
#define PROJECTILE_SPEED 10.f
#define PROJECTILE_ANCH_FRAME 1.f/4.f
#define PROJECTILE_ALT_FRAME 1.f/2.f


enum Anims {
	DIE_LEFT, BEGIN_MOVE_LEFT, BEGIN_IDLE_LEFT, SHOT_LEFT, IDLE_LEFT, MOVE_LEFT, HIT_LEFT, END_MOVE_LEFT,
	DIE_RIGHT, BEGIN_MOVE_RIGHT, BEGIN_IDLE_RIGHT, SHOT_RIGHT, IDLE_RIGHT, MOVE_RIGHT, HIT_RIGHT, END_MOVE_RIGHT
};

enum Projectile {RIGHT, LEFT};

void Wizard::hit() {
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

void Wizard::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {


	//pos = tileMapPos;
	vulnerable = false;
	alive = true;
	lifes = 99;
	size = glm::ivec2(80, 80);
	colisionBox.x = size.x;		//22 es els pixels quefa d'ample el personatge, 68 el tamany total del sprite
	colisionBox.y = (size.y) / ALT_FRAME_PIXELS;				//37 perque te 37 pixels i vull que sigui nomes un pixel de ample
	colisionOffset.x = (size.x *26.0f) / ANCH_FRAME_PIXELS;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (size.y) - colisionBox.y;	//aixo esta bé mentre es recolzi al terra per la part mes baixa (que en principi sera aixi amb tot personatge)
	//hitBoxOffset.y = (size.y * 37.f) / ALT_FRAME_PIXELS; Aixo no em cal perque el ATTACK ocupa tota la vertical del sprite
	pos = posInicial;

	//INICIALITZACIO PROJECTIL
	projectileTexture;
	projectileTexture.loadFromFile("images/projectile.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projectile = Sprite::createSprite(glm::ivec2(800, 200 / ALT_FRAME_PIXELS), glm::ivec2(0.25, 0.5), &projectileTexture, &shaderProgram);
	//projectile = Sprite::createSprite(glm::ivec2(20, 10), glm::ivec2(0.25, 0.5), &projectileTexture, &shader);
	projectile->setNumberAnimations(2);
	projectile->setAnimationSpeed(RIGHT, PROJECTILE_SPEED);
	projectile->setAnimationSpeed(LEFT, PROJECTILE_SPEED);
	for (int j = 0; j < 4; j++) {
		projectile->addKeyframe(LEFT, glm::vec2(PROJECTILE_ANCH_FRAME * j, PROJECTILE_ALT_FRAME * 0));
		projectile->addKeyframe(RIGHT, glm::vec2(PROJECTILE_ANCH_FRAME * j, PROJECTILE_ALT_FRAME * 1));
	}


	timeChargingAttack = 0.f;
	spritesheet.loadFromFile("images/wizard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(size, glm::vec2(ANCH_FRAME, ALT_FRAME), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);

	for (int i = 0; i < 6; i++) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i]; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * j, ALT_FRAME * i));
	}

	for (int i = 8; i < 14; i++) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < animSize[i-8]; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * (animSize[i -8] - 1 - j), ALT_FRAME * (i-2)));
	}


	sprite->setAnimationSpeed(HIT_RIGHT, MOVEMENT_SPEED);
	sprite->setAnimationSpeed(HIT_LEFT, MOVEMENT_SPEED);
	sprite->setAnimationSpeed(END_MOVE_RIGHT, MOVEMENT_SPEED);
	sprite->setAnimationSpeed(END_MOVE_LEFT, MOVEMENT_SPEED);
	for (int j = 0; j < 2; ++j) {
		sprite->addKeyframe(HIT_RIGHT, glm::vec2(ANCH_FRAME * j, ALT_FRAME * DIE_RIGHT));
		sprite->addKeyframe(HIT_LEFT, glm::vec2(ANCH_FRAME * (10 - 1 - j), ALT_FRAME * DIE_LEFT));
	}

	for (int j = 0; j < 3; ++j) {
		sprite->addKeyframe(END_MOVE_RIGHT, glm::vec2(ANCH_FRAME * j, ALT_FRAME * 7));
		sprite->addKeyframe(END_MOVE_LEFT, glm::vec2(ANCH_FRAME * (2-j), ALT_FRAME * BEGIN_MOVE_LEFT));
	}
	sprite->setPosition(pos);
	//sprite->changeAnimation(SHOT_LEFT);
}

void Wizard::update(int deltaTime)
{
	killTarget();
	sprite->update(deltaTime);
	float debug = ATTACK_CHARGING_TIME;
	attacking = false;
	int anim = sprite->animation();
	dreta = anim > 7;
	if (sprite->finished()) vulnerable = true;
	if (projectileShot) updateProjectile(deltaTime);
	if (alive) {
		if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != SHOT_LEFT && anim != SHOT_RIGHT)) {
			auto initialPos = pos;

			if (Game::instance().getKey('a')) {
				attacking = true;
				//PlaySound(TEXT("audio/axeSwingCutre.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
				if (dreta) sprite->changeAnimation(SHOT_RIGHT);
				else sprite->changeAnimation(SHOT_LEFT);
				shotProjectile();
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				dreta = true;
				pos.x += 1;
				if (map->collisionMoveRight(pos, colisionBox, colisionOffset))
				{
					pos.x -= 1;
				}
				else if (anim != MOVE_RIGHT && anim != BEGIN_MOVE_RIGHT)
					sprite->changeAnimation(BEGIN_MOVE_RIGHT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				dreta = false;
				pos.x -= 1;
				if (map->collisionMoveLeft(pos, colisionBox, colisionOffset))
				{
					pos.x += 1;
				}
				else if (anim != MOVE_LEFT && anim != BEGIN_MOVE_LEFT)
					sprite->changeAnimation(BEGIN_MOVE_LEFT);
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				pos.y += 1;
				if (map->collisionMoveDown(pos, colisionBox, colisionOffset))
				{
					pos.y -= 1;
				}
				else if (anim != MOVE_RIGHT && anim != MOVE_LEFT && anim != BEGIN_MOVE_RIGHT && anim != BEGIN_MOVE_LEFT) {
					if (!dreta) sprite->changeAnimation(BEGIN_MOVE_LEFT);
					else sprite->changeAnimation(BEGIN_MOVE_RIGHT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				pos.y -= 1;
				if (map->collisionMoveUp(pos, colisionBox, colisionOffset))
				{
					pos.y += 1;
				}
				else if (anim != MOVE_RIGHT && anim != MOVE_LEFT && anim != BEGIN_MOVE_RIGHT && anim != BEGIN_MOVE_LEFT) {
					if (!dreta) sprite->changeAnimation(BEGIN_MOVE_LEFT);
					else sprite->changeAnimation(BEGIN_MOVE_RIGHT);
				}
			}
			if (sprite->finished()) {
				if (anim == BEGIN_MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
				else if (anim == BEGIN_MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			}
			if (initialPos == pos) {
				if (sprite->finished() && anim != IDLE_LEFT && anim != IDLE_RIGHT) {
					if (anim == MOVE_RIGHT)
						sprite->changeAnimation(END_MOVE_RIGHT);
					else if (anim == MOVE_LEFT)
						sprite->changeAnimation(END_MOVE_LEFT);
					else if (anim == END_MOVE_RIGHT) sprite->changeAnimation(BEGIN_IDLE_RIGHT);
					else if (anim == END_MOVE_LEFT) sprite->changeAnimation(BEGIN_IDLE_LEFT);
					else if (anim == BEGIN_IDLE_RIGHT) sprite->changeAnimation(IDLE_RIGHT);
					else if (anim == BEGIN_IDLE_LEFT) sprite->changeAnimation(IDLE_LEFT);
					else if (dreta) sprite->changeAnimation(BEGIN_IDLE_RIGHT);
					else sprite->changeAnimation(BEGIN_IDLE_LEFT);
				}
			}
		}
	}
	if (chargingAttack) {
		timeChargingAttack += deltaTime;
		if (timeChargingAttack > ATTACK_CHARGING_TIME) {
			PlaySound(TEXT("audio/hit_placeholder.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
			attacking = true;
			chargingAttack = false;
			timeChargingAttack = 0.f;
		}
	}
	setPosition();
}


box Wizard::hitBox() {
	box caja;
	return caja;
}

void Wizard::shotProjectile() {
	projectileShot = true;
	if (dreta) projectilePos.x = pos.x + (8*size.x)/ALT_FRAME_PIXELS;
	else projectilePos.x = pos.x - (74 * size.x) / ALT_FRAME_PIXELS;
	projectilePos.y = pos.y + (42*size.y)/ANCH_FRAME_PIXELS;
	projectile->setPosition(projectilePos);
	projectile->changeAnimation(LEFT);
}

void Wizard::updateProjectile(int deltaTime) {

	projectile->update(deltaTime);
	if (projectile->animation() == LEFT) projectilePos.x -= 2;
	else projectilePos.x -= 2;
	projectile->setPosition(projectilePos);
}

void Wizard::render() {
	if(projectileShot)
		projectile->render();
	sprite->render();
}