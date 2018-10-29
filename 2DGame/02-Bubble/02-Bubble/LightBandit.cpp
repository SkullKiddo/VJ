#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "LightBandit.h"
#include "Game.h"
#include <Mmsystem.h>

#define MOVEMENT_SPEED 8
#define altframe 1.f/10.f
#define anchframe 1.f/8.f

enum PlayerAnims
{
	IDDLE_LEFT, MOVE_LEFT, ATACK_LEFT, DIE_LEFT, HIT_LEFT, IDDLE_RIGHT, MOVE_RIGHT, ATACK_RIGHT, DIE_RIGHT, HIT_RIGHT
};

void LightBandit::hit() {
	if (vulnerable && alive) {
		vulnerable = false;
		if (lifes > 1) {
			lifes--;
			if (sprite->animation() > 4) sprite->changeAnimation(HIT_RIGHT);
			else sprite->changeAnimation(HIT_LEFT);
		}
		else {
			lifes = 0;
			alive = false;
			if (sprite->animation() > 4) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);

		}
	}
}

void LightBandit::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
	//pos = tileMapPos;
	vulnerable = false;
	alive = true;
	lifes = 3;
	size = glm::ivec2(43 * 3, 37 * 3);
	colisionBox.x = size.x;
	colisionBox.y = (size.y) / 48.0f;				//37 perque te 37 pixels i vull que sigui nomes un pixel de ample
	colisionOffset.x = (size.x *16.0f) / 51.0f;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (size.y) - colisionBox.y;	//aixo esta bé mentre es recolzi al terra per la part mes baixa (que en principi sera aixi amb tot personatge)
	pos = posInicial;
	spritesheet.loadFromFile("images/HeavyBandit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(size, glm::vec2(anchframe, altframe), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);

	for (int i = 0; i < 5; i++) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < 8; j++)
			sprite->addKeyframe(i, glm::vec2(anchframe * j, altframe * i));
	}

	for (int i = 5; i < 10; i++) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < 8; j++)
			sprite->addKeyframe(i, glm::vec2(anchframe * (8 - 1 - j), altframe * i));
	}

	//sprite->setAnimationSpeed(HIT_LEFT, MOVEMENT_SPEED);
	//for (int j = 0; j < 8; j++)
	//	sprite->addKeyframe(HIT_LEFT, glm::vec2(anchframe * j, altframe * IDDLE_LEFT));

	//sprite->setAnimationSpeed(HIT_RIGHT, MOVEMENT_SPEED);
	//for (int j = 0; j < 8; j++)
	//	sprite->addKeyframe(HIT_RIGHT, glm::vec2(anchframe * j, altframe * IDDLE_RIGHT));

	//sprite->setAnimationSpeed(DIEDEDED_RIGHT, MOVEMENT_SPEED);
	//sprite->addKeyframe(DIEDEDED_RIGHT, glm::vec2(anchframe * 14, altframe * 4));

	//sprite->setAnimationSpeed(DIEDEDED_LEFT, MOVEMENT_SPEED);
	//sprite->addKeyframe(DIEDEDED_LEFT, glm::vec2(anchframe * 0, altframe * 5));


	sprite->changeAnimation(MOVE_LEFT);
	//tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(pos.x, pos.y));
	//setPosition();
}

void LightBandit::update(int deltaTime)
{

	sprite->update(deltaTime);
	int anim = sprite->animation();
	bool dreta = anim > 4;
	if (sprite->finished()) vulnerable = true;
	if (alive) {
		if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != ATACK_LEFT && anim != ATACK_RIGHT)) {
			auto initialPos = pos;

			if (Game::instance().getKey('b')) {
				//PlaySound(TEXT("audio/axeSwingCutre.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
				mciSendString(L"play audio/axeSwingCutre.wav", NULL, 0, NULL);
				if (dreta) sprite->changeAnimation(ATACK_RIGHT);
				else sprite->changeAnimation(ATACK_LEFT);
			}
			else if (Game::instance().getKey('d'))
			{

				pos.x += 1;
				if (map->collisionMoveRight(pos, colisionBox, colisionOffset))
				{
					pos.x -= 1;
				}
				else if (anim != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			else if (Game::instance().getKey('a'))
			{
				pos.x -= 1;
				if (map->collisionMoveLeft(pos, colisionBox, colisionOffset))
				{
					pos.x += 1;
				}
				else if (anim != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
			if (Game::instance().getKey('s'))
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
			else if (Game::instance().getKey('w'))
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
	}
	/*else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (dreta) sprite->changeAnimation(DIEDEDED_RIGHT);
			else sprite->changeAnimation(DIEDEDED_LEFT);
		}
	}*/

	setPosition();
}

void LightBandit::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void LightBandit::setPosition()
{
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void LightBandit::render()
{
	sprite->render();
}
