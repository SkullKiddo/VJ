#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Menu.h"
#include "Game.h"
#include <algorithm>

#define MAX_TIME_BETWEEN_OPTIONS 1000
#define MOVEMENT_SPEED 8
#define CHARACTER_SPEED 6
#define POSX 0
#define POSY 0

#define altframe 1.f/3.f
#define anchframe 1.f/8.f

#define altframe_options 1.f
#define anchframe_options 1.f/4.f

#define altframe_adv 1.f/26.f
#define anchframe_adv 1.f/10.f

#define ALT_FRAME_KNIGHT 1.f/12.f
#define ANCH_FRAME_KNIGHT 1.f/10.f

#define ALT_FRAME_SWORD 1.f/5.f
#define ANCH_FRAME_SWORD 1.f





enum setAnim { MENU, PLAY, INSTRUCTIONS, CREDITS, EXIT };

Menu::Menu() {}

Menu::~ Menu() {}

void Menu::init() {

	play = false;
	exit = false;
	selection = false;
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

	//BACKGROUND

	spritesheet.loadFromFile("Background/Menu_Sprite_Sheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sizeMenu = glm::ivec2(1680, 1040);

	sprite = Sprite::createSprite(sizeMenu, glm::vec2(anchframe, altframe), &spritesheet, &texProgram);
	sprite->setNumberAnimations(5);
	sprite->setAnimationSpeed(MENU, MOVEMENT_SPEED);
	int col = 0;
	for (int j = 0; j < 3; ++j) {
		if (j < 2) col = 7;
		else col = 8;
		for (int i = 0; i < col; ++i)
			sprite->addKeyframe(MENU, glm::vec2(anchframe * i, altframe * j));
	}

	//OPTIONS
	
	spritesheet_options.loadFromFile("Background/Menu_Options.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet_options.setMagFilter(GL_NEAREST);

	sprite_options = Sprite::createSprite(sizeMenu, glm::vec2(anchframe_options, altframe_options), &spritesheet_options, &texProgram);
	sprite_options->setNumberAnimations(5);
	sprite_options->setAnimationSpeed(PLAY,MOVEMENT_SPEED);
	sprite_options->addKeyframe(PLAY, glm::vec2(anchframe_options * 0, altframe_options * 0));
	sprite_options->setAnimationSpeed(INSTRUCTIONS, MOVEMENT_SPEED);
	sprite_options->addKeyframe(INSTRUCTIONS, glm::vec2(anchframe_options * 1, altframe_options * 0));
	sprite_options->setAnimationSpeed(CREDITS, MOVEMENT_SPEED);
	sprite_options->addKeyframe(CREDITS, glm::vec2(anchframe_options * 2, altframe_options * 0));
	sprite_options->setAnimationSpeed(EXIT, MOVEMENT_SPEED);
	sprite_options->addKeyframe(EXIT, glm::vec2(anchframe_options * 3, altframe_options * 0));
	sprite_options->setPosition(glm::ivec2(POSX, POSY));

	sprite_options->changeAnimation(PLAY); //animacio per defecte

	//SWORD

	spritesheet_sword.loadFromFile("images/selection_sword.png",TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet_sword.setMagFilter(GL_NEAREST);

	sprite_sword = Sprite::createSprite(glm::ivec2(231, 797)/2, glm::vec2(ANCH_FRAME_SWORD, ALT_FRAME_SWORD), &spritesheet_sword, &texProgram);
	sprite_sword->setNumberAnimations(1);
	sprite_sword->setAnimationSpeed(IDLE_SWORD, CHARACTER_SPEED);
	for (int i = 0; i < 5; ++i) {
		sprite_sword->addKeyframe(IDLE_SWORD, glm::vec2(ANCH_FRAME_SWORD * 0 , ALT_FRAME_SWORD * i));
	}

	sprite_sword->setPosition(glm::vec2(455,150));
	sprite_sword->changeAnimation(IDLE_SWORD);

	//ADVENTURER 

	spritesheet_adv.loadFromFile("images/Adventurer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet_adv.setMagFilter(GL_NEAREST);

	sprite_adv = Sprite::createSprite(glm::ivec2(50, 37)*9, glm::vec2(anchframe_adv, altframe_adv), &spritesheet_adv, &texProgram);
	sprite_adv->setNumberAnimations(1);
	sprite_adv->setAnimationSpeed(IDLE_ADV, CHARACTER_SPEED);
	for (int i = 0; i < 4; ++i) {
		sprite_adv->addKeyframe(IDLE_ADV, glm::vec2(anchframe_adv * i, altframe_adv * 1));
	}

	sprite_adv->setPosition(glm::vec2(754, 1040 / 1.85));
	sprite_adv->changeAnimation(IDLE_ADV);

	//GREEN ADVENTURER

	spritesheet_greenAdv.loadFromFile("images/green_adventurer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet_greenAdv.setMagFilter(GL_NEAREST);

	sprite_greenAdv = Sprite::createSprite(glm::ivec2(50, 37)*9, glm::vec2(anchframe_adv, altframe_adv), &spritesheet_greenAdv, &texProgram);
	sprite_greenAdv->setNumberAnimations(1);
	sprite_greenAdv->setAnimationSpeed(IDLE_ADV, CHARACTER_SPEED);
	for (int i = 0; i < 4; ++i) {
		sprite_greenAdv->addKeyframe(IDLE_ADV, glm::vec2(anchframe_adv * i, altframe_adv * 1));
	}

	sprite_greenAdv->setPosition(glm::vec2(1680 / 1.7, 1040 / 1.65));
	sprite_greenAdv->changeAnimation(IDLE_GREEN_ADV);

	//KNIGHT
	spritesheet_knight.loadFromFile("images/knight.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet_knight.setMagFilter(GL_NEAREST);

	sprite_knight = Sprite::createSprite(glm::ivec2(80, 42)*13, glm::vec2(ANCH_FRAME_KNIGHT, ALT_FRAME_KNIGHT), &spritesheet_knight, &texProgram);
	sprite_knight->setNumberAnimations(1);
	sprite_knight->setAnimationSpeed(IDLE_ADV, CHARACTER_SPEED);
	for (int i = 0; i < 4; ++i) {
		sprite_knight->addKeyframe(IDLE_ADV, glm::vec2(ANCH_FRAME_KNIGHT * i, ALT_FRAME_KNIGHT * 8));
	}

	sprite_knight->setPosition(glm::vec2(10, 420));
	sprite_knight->changeAnimation(IDLE_KNIGHT);

}

void Menu::update(int deltaTime) {

	sprite->update(deltaTime);
	sprite_options->update(deltaTime);
	sprite_adv->update(deltaTime);
	sprite_greenAdv->update(deltaTime);
	sprite_knight->update(deltaTime);
	sprite_sword->update(deltaTime);
	auto anim = sprite_options->animation();

	if (sprite->finished())
		sprite->changeAnimation(MENU);

	if (sprite_options->finished() || (anim != PLAY && anim != INSTRUCTIONS && anim != CREDITS && anim != EXIT))
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			switch (anim) {
			case PLAY:
				sprite_options->changeAnimation(INSTRUCTIONS);
				break;
			case INSTRUCTIONS:
				sprite_options->changeAnimation(CREDITS);
				break;
			case CREDITS:
				sprite_options->changeAnimation(EXIT);
				break;
			default:
				sprite_options->changeAnimation(PLAY);
				break;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			switch (anim) {
			case PLAY:
				sprite_options->changeAnimation(EXIT);
				break;
			case INSTRUCTIONS:
				sprite_options->changeAnimation(PLAY);
				break;
			case CREDITS:
				sprite_options->changeAnimation(INSTRUCTIONS);
				break;
			default:
				sprite_options->changeAnimation(CREDITS);
				break;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && sprite_sword->finished()) {
			sprite_sword->changeAnimation(IDLE_SWORD);
			switch (choice) {
			case 1:
				choice = 2;
				sprite_sword->setPosition(glm::vec2(920, 250));
				break;
			case 2:
				choice = 3;
				sprite_sword->setPosition(glm::vec2(1160, 320));
				break;
			case 3:
				choice = 1;
				sprite_sword->setPosition(glm::vec2(455, 150));
				break;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && sprite_sword->finished()) {
			sprite_sword->changeAnimation(IDLE_SWORD);
			switch (choice) {
			case 1:
				choice = 3;
				sprite_sword->setPosition(glm::vec2(455, 150));
				break;
			case 2:
				choice = 1;
				sprite_sword->setPosition(glm::vec2(920, 250));
				break;
			case 3:
				choice = 2;
				sprite_sword->setPosition(glm::vec2(1160, 320));
				break;
			}
		}
		if (Game::instance().getKey('e')) {
			if (!selection) {
				switch (anim)
				{
				case PLAY:
					if (first) {
						selection = true;
					}
					else play = true;
					break;
				case EXIT:
					exit = true;
					break;
				}
			}
		}
		else if (Game::instance().getKey('s')) {
			play = true;
			first = false;
		}
	}
}

void Menu::render() {

	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	sprite->render();
	
	if (!selection) sprite_options->render();
	else {
		sprite_adv->render();
		sprite_greenAdv->render();
		sprite_knight->render();
		sprite_sword->render();
	}
}

void Menu::stopPlaying() {
	play = false;
}

bool Menu::getPlaying() {
	return play;
}

bool Menu::getExit() {
	return exit;
}

void Menu::initShaders() {

	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

int Menu::getChoice() {
	return choice;
}

bool Menu::getChosen() {
	return selection;
}