#include "LightBandit.h"

#define MOVEMENT_SPEED 8
#define altframe 1.f/10.f
#define anchframe 1.f/8.f

enum PlayerAnims
{
	IDLE_LEFT, MOVE_LEFT, ATACK_LEFT, DIE_LEFT, HIT_LEFT, IDLE_RIGHT, MOVE_RIGHT, ATACK_RIGHT, DIE_RIGHT, HIT_RIGHT
};

#define MOVEMENT_SPEED 8
#define ALT_FRAME 1.f/10.f
#define ANCH_FRAME 1.f/8.f
#define ALT_FRAME_PIXELS 48.f
#define ANCH_FRAME_PIXELS 51.f
#define ATACK_CHARGING_TIME (1000.f/MOVEMENT_SPEED) *4.f //aqui nomes s'ha de substituir el 4 per el num de frames que hagi d'esperar abans d'atacar

//void LightBandit::init(const glm::ivec2 &posInicial, ShaderProgram &shaderProgram) {
//	vulnerable = false;
//	alive = true;
//	lifes = 99;
//	size = glm::ivec2(43 * 3, 37 * 3);
//	colisionBox.x = size.x;
//	colisionBox.y = (size.y) / ALT_FRAME_PIXELS;				//37 perque te 37 pixels i vull que sigui nomes un pixel de ample
//	colisionOffset.x = (size.x *16.0f) / ANCH_FRAME_PIXELS;		//21 son els pixels que em sobren per davant i 68 el total
//	colisionOffset.y = (size.y) - colisionBox.y;	//aixo esta bé mentre es recolzi al terra per la part mes baixa (que en principi sera aixi amb tot personatge)
//	pos = posInicial;
//	timeChargingAttack = 0.f;
//	spritesheet.loadFromFile("images/LightBandit.png", TEXTURE_PIXEL_FORMAT_RGBA);
//	spritesheet.setMagFilter(GL_NEAREST);
//	sprite = Sprite::createSprite(size, glm::vec2(ANCH_FRAME, ALT_FRAME), &spritesheet, &shaderProgram);
//	sprite->setNumberAnimations(20);
//
//	for (int i = 0; i < 5; i++) {
//		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
//		for (int j = 0; j < 8; j++)
//			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * j, ALT_FRAME * i));
//	}
//
//	for (int i = 5; i < 10; i++) {
//		sprite->setAnimationSpeed(i, MOVEMENT_SPEED);
//		for (int j = 0; j < 8; j++)
//			sprite->addKeyframe(i, glm::vec2(ANCH_FRAME * (8 - 1 - j), ALT_FRAME * i));
//	}
//
//	sprite->changeAnimation(MOVE_LEFT);
//	sprite->setPosition(glm::vec2(pos.x, pos.y));
//}