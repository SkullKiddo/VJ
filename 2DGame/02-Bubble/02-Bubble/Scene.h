#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#define NUM_ENEMIES 2


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Skeleton.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "HeavyBandit.h"
#include "Adventurer.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

struct Maps
{
	TileMap *map;
	TileMap *collisions;
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void setBackground(const string &filename);
	void setCollsion();

private:
	void initShaders();

private:
	TileMap *map;
	Maps mapping;
	Adventurer* adventurer;
	Character *enemies[NUM_ENEMIES];
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture texs;
	TexturedQuad *background;
	bool bcollision = false;
	void handleAtacks();
};


#endif // _SCENE_INCLUDE

