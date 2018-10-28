#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skeleton.h"
#include "Game.h"
#include <Mmsystem.h>
#include "TexturedQuad.h"

#define MOVEMENT_SPEED 8.f
#define SIZE 10
#define POSX 10
#define POSY 10
#define ALT_FRAME 1.f/12.f
#define ANCH_FRAME 1.f/18.f
#define ALT_FRAME_PIXELS 37.f
#define ANCH_FRAME_PIXELS 58.f
#define ATACK_CHARGING_TIME (1000.f/MOVEMENT_SPEED) *7.f //aqui nomes s'ha de substituir el 7 per el num de frames que hagi d'esperar abans d'atacar

enum PlayerAnims
{
	ATACK_RIGHT, ATACK_LEFT, REACT_RIGHT, REACT_LEFT, DIE_RIGHT, DIE_LEFT, IDDLE_RIGHT, IDDLE_LEFT, HIT_RIGHT, HIT_LEFT, MOVE_RIGHT, MOVE_LEFT, DIEDEDED_RIGHT, DIEDEDED_LEFT
};

int skeletonAnimSize[] = {18,4,15,11,8,13};

void Skeleton::hit() {
	if (vulnerable && alive) {
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

void Skeleton::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {

	//pos = tileMapPos;
	vulnerable = false;
	alive = true;
	lifes = 99;
	size = glm::ivec2(80, 80);
	colisionBox.x = size.x;		//22 es els pixels quefa d'ample el personatge, 68 el tamany total del sprite
	colisionBox.y = (size.y) / ALT_FRAME_PIXELS;				//37 perque te 37 pixels i vull que sigui nomes un pixel de ample
	colisionOffset.x = (size.x *26.0f) / ANCH_FRAME_PIXELS;		//21 son els pixels que em sobren per davant i 68 el total
	colisionOffset.y = (size.y) - colisionBox.y;	//aixo esta bé mentre es recolzi al terra per la part mes baixa (que en principi sera aixi amb tot personatge)
	//hitBoxOffset.y = (size.y * 37.f) / ALT_FRAME_PIXELS; Aixo no em cal perque el atack ocupa tota la vertical del sprite
	pos = posInicial;
	timeChargingAtack = 0.f;
	spritesheet.loadFromFile("images/esqueleto.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(size, glm::vec2(ANCH_FRAME,ALT_FRAME), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);

	for (int i = 0; i < 12; i+=2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < skeletonAnimSize[i/2]; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * j, ALT_FRAME * i));
	}

	for (int i = 1; i < 12; i+=2) {
		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
		for (int j = 0; j < skeletonAnimSize[i/2]; j++)
			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * (skeletonAnimSize[i/2]-1 - j), ALT_FRAME * i));
	}

	sprite->setAnimationSpeed(DIEDEDED_RIGHT, MOVEMENT_SPEED);
	sprite->addKeyframe(DIEDEDED_RIGHT, glm::vec2(ANCH_FRAME * 14, ALT_FRAME * 4));

	sprite->setAnimationSpeed(DIEDEDED_LEFT, MOVEMENT_SPEED);
	sprite->addKeyframe(DIEDEDED_LEFT, glm::vec2(ANCH_FRAME * 0, ALT_FRAME * 5));
	

	sprite->changeAnimation(DIEDEDED_LEFT);
	//tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(pos.x, pos.y));
	//setPosition();
}

void Skeleton::update(int deltaTime)
{

	sprite->update(deltaTime);
	float debug = ATACK_CHARGING_TIME;
	atacking = false;
	int anim = sprite->animation();
	dreta = anim % 2 == 0;
	if (sprite->finished()) vulnerable = true;
	if (alive) {
		if (sprite->finished() || (anim != HIT_LEFT && anim != HIT_RIGHT && anim != ATACK_LEFT && anim != ATACK_RIGHT)) {
			auto initialPos = pos;

			if (Game::instance().getKey('b')) {
				chargingAtack = true;
				//PlaySound(TEXT("audio/axeSwingCutre.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT | SND_NOSTOP);
				if (dreta) sprite->changeAnimation(ATACK_RIGHT);
				else sprite->changeAnimation(ATACK_LEFT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				dreta = true;
				pos.x += 1;
				if (map->collisionMoveRight(pos, colisionBox,colisionOffset))
				{
					pos.x -= 1;
				}
				else if (anim != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				dreta = false;
				pos.x -= 1;
				if (map->collisionMoveLeft(pos, colisionBox, colisionOffset))
				{
					pos.x += 1;
				}
				else if (anim != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
			if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
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
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
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
			if (initialPos == pos){
				if (dreta && anim != IDDLE_RIGHT) sprite->changeAnimation(IDDLE_RIGHT);
				else if(!dreta && anim != IDDLE_LEFT) sprite->changeAnimation(IDDLE_LEFT);
			}
		}
	}
	else {
		if ((anim == DIE_LEFT || anim == DIE_RIGHT) && sprite->finished()) {
			if (dreta) sprite->changeAnimation(DIEDEDED_RIGHT);
			else sprite->changeAnimation(DIEDEDED_LEFT);
		}
	}
	if (chargingAtack) {
		timeChargingAtack += deltaTime;
		if (timeChargingAtack > ATACK_CHARGING_TIME) {
			mciSendString(L"play audio/axeSwingCutre.wav", NULL, 0, NULL);
			atacking = true;
			if (dreta) {
				hitBox.mins = glm::ivec2(pos.x + size.x - colisionOffset.x, pos.y); //no foto offsets ni res perque ocupa tota la vertical
				hitBox.maxs = glm::ivec2(pos.x + size.x, pos.y + size.y);
			}
			else {
				hitBox.mins = glm::ivec2(pos.x, pos.y);
				hitBox.maxs = glm::ivec2(pos.x + colisionOffset.x, pos.y + size.y);
			}
			chargingAtack = false;
			timeChargingAtack = 0.f;
		}
	}
	setPosition();
}







//void Skeleton::render()
//{
//	sprite->render();
//}
