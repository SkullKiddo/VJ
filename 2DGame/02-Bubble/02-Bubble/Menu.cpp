#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Menu.h"
#include "Game.h"
#include <algorithm>

#define MOVEMENT_SPEED 7
#define POSX 0
#define POSY 0

#define altframe 1.f/3.f
#define anchframe 1.f/8.f

#define altframe_options 1.f
#define anchframe_options 1.f/4.f



enum setAnim { MENU, PLAY, INSTRUCTIONS, CREDITS, EXIT };

Menu::Menu() {}

Menu::~ Menu() {}

void Menu::init() {

	play = false;
	exit = false;
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
}

void Menu::update(int deltaTime) {

	sprite->update(deltaTime);
	sprite_options->update(deltaTime);
	auto anim = sprite_options->animation();

	if (sprite->finished())
		sprite->changeAnimation(MENU);

	if (sprite_options->finished() || anim != PLAY && anim != INSTRUCTIONS && anim != CREDITS && anim != EXIT)
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
		if (Game::instance().getKey('e')) {
			switch (anim)
			{
			case PLAY :
				play = true;
				break;
			case EXIT :
				exit = true;
				break;
			}
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
	sprite_options->render();
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