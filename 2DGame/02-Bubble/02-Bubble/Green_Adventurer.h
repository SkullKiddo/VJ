#ifndef _GREEN_ADVENTURER_INCLUDE
#define _GREEN_ADVENTURER_INCLUDE

#include "Character.h"

class Green_Adventurer : public Character
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void hit();
	void render();
	box hitBox();

private:
	int animSize[13] = { 4,4,6,8,5,4,7,4,6,10,4,8,8 };
	bool unshealthed;
	int timeBetweenAttacks;
	bool chargingAtack1, chargingAtack2, chargingAtack3;
};

#endif