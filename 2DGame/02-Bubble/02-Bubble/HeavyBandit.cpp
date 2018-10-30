#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "HeavyBandit.h"
#include "Game.h"
#include <Mmsystem.h>

#define MOVEMENT_SPEED 8
#define ALT_FRAME 1.f/10.f
#define ANCH_FRAME 1.f/8.f
#define ALT_FRAME_PIXELS 48.f
#define ANCH_FRAME_PIXELS 51.f
#define ATACK_CHARGING_TIME (1000.f/MOVEMENT_SPEED) *4.f //aqui nomes s'ha de substituir el 4 per el num de frames que hagi d'esperar abans d'atacar


enum PlayerAnims
{
	IDDLE_LEFT, MOVE_LEFT, ATACK_LEFT, DIE_LEFT, HIT_LEFT, IDDLE_RIGHT, MOVE_RIGHT, ATACK_RIGHT, DIE_RIGHT, HIT_RIGHT
};

void HeavyBandit::hit() {
	if (vulnerable && alive) {
		chargingAtack = false;
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

void HeavyBandit::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
	vulnerable = false;
	alive = true;
	lifes = 99;
	size = glm::ivec2(43 * 3, 37 * 3);
	colisionBox.x = size.x;
	colisionBox.y = (size.y) / ALT_FRAME_PIXELS;				//37 perque te 37 pixels i vull que sigui nomes un pixel de ample
	colisionOffset.x = (size.x *16.0f) / ANCH_FRAME_PIXELS;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (size.y) - colisionBox.y;	//aixo esta bé mentre es recolzi al terra per la part mes baixa (que en principi sera aixi amb tot personatge)
	pos = posInicial;
	timeChargingAtack = 0.f;
	spritesheet.loadFromFile("images/HeavyBandit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(size, glm::vec2(ANCH_FRAME, ALT_FRAME), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);

	for (int i = 0; i < 5; i ++) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < 8; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * j, ALT_FRAME * i));
	}

	for (int i = 5; i < 10; i ++) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < 8; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * (8 - 1 - j), ALT_FRAME * i));
	}

	sprite->changeAnimation(MOVE_LEFT);
	sprite->setPosition(glm::vec2(pos.x, pos.y));
}

void HeavyBandit::update(int deltaTime)
{
	killTarget();
	sprite->update(deltaTime);
	atacking = false;
	int anim = sprite->animation();
	dreta = anim > 4;
	if (sprite->finished()) vulnerable = true;
	if (alive) {
		if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != ATACK_LEFT && anim != ATACK_RIGHT)) {
			auto initialPos = pos;

			if (atackTarguet) {
				//PlaySound(TEXT("audio/axeSwingCutre.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
				chargingAtack = true;
				mciSendString(L"play audio/axeSwingCutre.wav", NULL, 0, NULL);
				if (dreta) sprite->changeAnimation(ATACK_RIGHT);
				else sprite->changeAnimation(ATACK_LEFT);
			}
			else if (moveRight)
			{

				pos.x += 1;
				if (map->collisionMoveRight(pos, colisionBox, colisionOffset))
				{
					pos.x -= 1;
				}
				else if (anim != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			else if (moveLeft)
			{
				pos.x -= 1;
				if (map->collisionMoveLeft(pos, colisionBox, colisionOffset))
				{
					pos.x += 1;
				}
				else if (anim != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
			if (moveDown)
			{
				pos.y += 1;
				if (map->collisionMoveDown(pos, colisionBox, colisionOffset))
				{
					pos.y -= 1;
				}
				else if (anim != MOVE_RIGHT && anim != MOVE_LEFT) {
					if (!dreta) sprite->changeAnimation(MOVE_LEFT);
					else sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			else if (moveUp)
			{
				pos.y -= 1;
				if (map->collisionMoveUp(pos, colisionBox, colisionOffset))
				{
					pos.y += 1;
				}
				else if (anim != MOVE_RIGHT && anim != MOVE_LEFT) {
					if (!dreta) sprite->changeAnimation(MOVE_LEFT);
					else sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			if (initialPos == pos && anim != IDDLE_LEFT && anim != IDDLE_RIGHT) {
				if (dreta) sprite->changeAnimation(IDDLE_RIGHT);
				else sprite->changeAnimation(IDDLE_LEFT);
			}
		}
		if (chargingAtack) {
			timeChargingAtack += deltaTime;
			if (timeChargingAtack > ATACK_CHARGING_TIME) {
				mciSendString(L"play audio/axeSwingCutre.wav", NULL, 0, NULL);
				atacking = true;
				
				chargingAtack = false;
				timeChargingAtack = 0.f;
			}
		}
	}
	/*else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (dreta) sprite->changeAnimation(DIEDEDED_RIGHT);
			else sprite->changeAnimation(DIEDEDED_LEFT);
		}
	}*/

	setPosition();
}

box HeavyBandit::hitBox() {
	int ymin = pos.y + (3 * size.y) / ALT_FRAME_PIXELS;
	int ymax = pos.y + size.y - (9 * size.y) / ALT_FRAME_PIXELS;
	box hitBox;
	if (dreta) {
		hitBox.mins = glm::ivec2(pos.x + colisionOffset.x, ymin); //no foto offsets ni res perque ocupa tota la vertical
		hitBox.maxs = glm::ivec2(pos.x + size.x, ymax);
	}
	else {
		hitBox.mins = glm::ivec2(pos.x, ymin);
		hitBox.maxs = glm::ivec2(pos.x + size.x - colisionOffset.x, ymax);
	}
	return hitBox;
}