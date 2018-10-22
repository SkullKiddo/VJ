#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Skeleton.h"
#include "TexturedQuad.h"
#include "Texture.h"

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
	Player *player;
	Skeleton *skeleton;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture texs;
	TexturedQuad *background;
	bool bcollision = false;
};


#endif // _SCENE_INCLUDE

