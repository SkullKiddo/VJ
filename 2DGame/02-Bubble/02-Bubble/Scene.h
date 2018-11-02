#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <vector>

#define NUM_LEVELS 2
#define NUM_ENEMIES_LVL1 3
#define NUM_ENEMIES_LVL2 4



#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Skeleton.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "HeavyBandit.h"
#include "Adventurer.h"
#include "Knight.h"
#include "Wizard.h"
#include "LightBandit.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

struct Maps
{
	TileMap *map;
	TileMap *collisions;
};

struct caractersInitialData
{
	int caracterType;
	glm::ivec2 pos;
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
	bool getEscape();
	void keepPlaying();
	int level = 0;

private:
	void initShaders();

private:
	TileMap *map;
	Maps mapping;
	Character* player;
	Character *enemies_lvl1[NUM_ENEMIES_LVL1];
	Character *enemies_lvl2[NUM_ENEMIES_LVL2];
	int numLevels = 2;
	vector<vector<Character*> > enemies = vector<vector<Character*> >(numLevels);;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int numEnemiesLevel[2] = { NUM_ENEMIES_LVL1, NUM_ENEMIES_LVL2 };
	Texture texs;
	TexturedQuad *background;
	bool bcollision = false;
	bool escape = false;
	bool first = false;
	void handleAtacks();
};


#endif // _SCENE_INCLUDE

