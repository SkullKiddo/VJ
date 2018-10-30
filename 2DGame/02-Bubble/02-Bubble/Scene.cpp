#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <algorithm>


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 7


Scene::Scene()
{
	map = NULL;
	for (Character* enemy: enemies) {
		enemy = NULL;
	}
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;

	for (Character* enemy: enemies) {
		if (enemy != NULL) delete enemy;
	}
}


void Scene::init()
{
	initShaders();
	setBackground("Background/Arena.png");
	map = TileMap::createTileMap("levels/Arena_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;


	//inicialitzar enemics
	adventurer = new Adventurer();
	adventurer->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);
	enemies[0] = new Skeleton();
	enemies[0]->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);
	enemies[1] = new HeavyBandit();
	enemies[1]->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);

	for (int i = 0; i < NUM_ENEMIES; i++) {
		enemies[i]->setTileMap(map);
	}
	adventurer->setTileMap(map);

	//asignar objectius
	for (Character* enemy : enemies) {
		enemy->target = adventurer;
	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	for (Character* enemy: enemies) {
		enemy->update(deltaTime);
	}
	if (Game::instance().getKey('h')) {
		adventurer->hit();
		for (Character* enemy: enemies) {
			enemy->hit();
		}
	}
	adventurer->update(deltaTime);
	handleAtacks();
	
}

void Scene::handleAtacks() {	//comprova si esta atacant cada enemic i ens golpeja si estem a la seva hitbox
	for (Character *enemy : enemies) {
		if (adventurer->atacking && adventurer->canHit(enemy)) enemy->hit();
		//if (enemy->atacking && enemy->canHit(adventurer)) adventurer->hit();
	}

}

bool cmp(const Character *e1, const Character *e2) {
	int y1 = e1->pos.y + e1->size.y;
	int y2 = e2->pos.y + e2->size.y;

	return y1 < y2;
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
	background->render(texs);
	if (bcollision) map->render();
	auto e1 = enemies[0];

	std::vector<Character*> myvector(enemies, enemies + NUM_ENEMIES);	//funciona, no preguntis
	std::sort(myvector.begin(), myvector.end(), cmp);
	for (Character *character : myvector) {
		character->render();
	}
	adventurer->render();

	//Debug pero no va
	//glm::vec2 geom[2] = { skeleton->hitBox.mins, skeleton->hitBox.maxs };
	//glm::vec2 geom[2] = {glm::ivec2(400,0), glm::ivec2(850,550), };

	//glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	//TexturedQuad *HB = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	//Texture text;
	//text.loadFromFile("images/Red.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//HB->render(text);
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

void Scene::setBackground(const string &filename) {
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::setCollsion() {
	bcollision = !bcollision;
}



