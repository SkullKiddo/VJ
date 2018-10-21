#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 7


Scene::Scene()
{
	map = NULL;
	player = NULL;
	skeleton = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (skeleton != NULL)
		delete skeleton;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/cuevaLava.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram); //"levels/cuevaLava.txt"     "levels/level01.txt" //TODO pasarli a create map una imatge de background
	//Background back = Background("images/rocks.jpeg",texProgram,glm::vec2(0.f,0.f));
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH/2 - 1), float(SCREEN_HEIGHT/2 -1), 0.f);
	currentTime = 0.0f;

	skeleton = new Skeleton();
	skeleton->init(glm::ivec2(20, 20), texProgram);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	skeleton->update(deltaTime);
	float cameraOffsetX = -20 + player->posPlayer.x;
	int maxOffsetX = float(SCREEN_WIDTH) + map->getMapSize().x+64; //TODO dependre el maxim offset de la camara del tamany de tile i personatge
	cameraOffsetX = (cameraOffsetX < maxOffsetX)?cameraOffsetX : maxOffsetX; //el maxim
	//projection = glm::ortho(0.f + cameraOffsetX, float(SCREEN_WIDTH) + cameraOffsetX, float(SCREEN_HEIGHT - 1), 0.f);
	if (Game::instance().getKey('h')) skeleton->hit();
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	skeleton->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



