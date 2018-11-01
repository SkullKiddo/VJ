#ifndef _ADVENTURER_INCLUDE
#define _ADVENTURER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Character.h"

class Adventurer : public Character
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void hit();
	box hitBox();

private:
	int animSize[13] = {4,4,6,8,5,4,7,4,6,10,4,8,8};
	bool unshealthed;
	int timeBetweenAttacks;
	bool chargingAtack1, chargingAtack2, chargingAtack3;
};

#endif