#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"


#define CAMERA_WIDTH 1680
#define CAMERA_HEIGHT 1040


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

struct Mapping
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
	void setCollsion();

private:
	void initShaders();

private:
	TileMap *map;
	bool bcollision = false;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Mapping mapping;

};


#endif // _SCENE_INCLUDE

