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
	
	for (Character *enemy : enemies_lvl1) {
		enemy = NULL;
	}
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	
	for (Character *enemy : enemies_lvl1) {
		if (enemy != NULL) delete enemy;
	}
}


void Scene::init()
{
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	enemies[level] = vector<Character*> (numEnemiesLevel[level]);
	//inicialitzar enemics

	if (level == 0) {
	setBackground("Background/Arena.png");
	map = TileMap::createTileMap("levels/Arena_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
 		enemies[0][0] = new Skeleton();
		enemies[0][0]->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);
		enemies[0][1] = new HeavyBandit();
		enemies[0][1]->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);
		enemies[0][2] = new LightBandit();
		enemies[0][2]->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);
	}

	else if (level == 1) {
		setBackground("Background/lvl2.jpg");
		map = TileMap::createTileMap("levels/Arena_collision.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		enemies[1][0] = new Wizard();
		enemies[1][0]->init(glm::ivec2(20, SCREEN_HEIGHT / 2), texProgram);
		enemies[1][1] = new Wizard();
		enemies[1][1]->init(glm::ivec2(SCREEN_WIDTH / 2, 20), texProgram);
		enemies[1][2] = new Wizard();
		enemies[1][2]->init(glm::ivec2(SCREEN_WIDTH -20, SCREEN_HEIGHT / 2), texProgram);
		enemies[1][3] = new Wizard();
		enemies[1][3]->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT -20), texProgram);
		
	}

	switch (choice)
	{
	case 2: 
		player = new Adventurer();
		break;
	case 3:
		player = new Green_Adventurer();
		break;
	case 1:
		player = new Knight();
		break;
	}

	player->setTileMap(map);
	player->init(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), texProgram);

	for (Character* enemy : enemies[level]) {
		enemy->setTileMap(map);
		enemy->target = player;
	}


	/*for (int i = 0; i < NUM_ENEMIES; i++) {
		enemies_lvl1[i]->setTileMap(map);
	}*/
	//asignar objectius
	
}

void Scene::update(int deltaTime)
{
	if (Game::instance().getKey(27)) {
		escape = true;
	}

	currentTime += deltaTime;
	//player->update(deltaTime);
	for (Character *enemy : enemies[level]) {
		enemy->update(deltaTime);
	}
	//float cameraOffsetX = -20 + player->posPlayer.x;
	//int maxOffsetX = float(SCREEN_WIDTH) + map->getMapSize().x+64; //TODO dependre el maxim offset de la camara del tamany de tile i personatge
	//cameraOffsetX = (cameraOffsetX < maxOffsetX)?cameraOffsetX : maxOffsetX; //el maxim
	//projection = glm::ortho(0.f + cameraOffsetX, float(SCREEN_WIDTH) + cameraOffsetX, float(SCREEN_HEIGHT - 1), 0.f);
	
	if (Game::instance().getKey('h')) {
		player->hit();
		for (Character* enemy: enemies[level]) {
			enemy->hit();
		}
	}
	if (Game::instance().getKey('n') && level == 0) {
		level++;
		init();
	}
	player->update(deltaTime);
	handleAtacks();
	
}

void Scene::handleAtacks() {	//comprova si esta atacant cada enemic i ens golpeja si estem a la seva hitbox
for (Character *enemy : enemies[level]) {
	if (player->attacking && player->canHit(enemy)) enemy->hit();
	//if (enemy->attacking && enemy->canHit(player)) player->hit();
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
	std::sort(enemies[level].begin(), enemies[level].end(), cmp);
	bool drawnPlayer = false;
	for (Character *character : enemies[level]) {
		if (!drawnPlayer && (character->pos.y + character->size.y) > (player->pos.y + player->size.y)) {
			player->render();
			drawnPlayer = true;
		}
		character->render();
	}
	if (!drawnPlayer) player->render();
	//player->render();

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

void Scene::setBackground(const string &filename) {
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texs.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::setCollsion() {
	bcollision = !bcollision;
}

void Scene::keepPlaying() {
	escape = false;
}

bool Scene::getEscape() {
	return escape;
}

void Scene::setChoice(int choice) {
	this->choice = choice;
	init();
}