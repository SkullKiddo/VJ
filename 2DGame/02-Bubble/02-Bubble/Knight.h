#ifndef _KNIGHT_INCLUDE
#define _KNIGHT_INCLUDE

#include "Character.h"

class Knight : public Character
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void hit();
	box hitBox();

private:
	bool blocking = false;
	int animSize[6] = {10,8,7,9,4,4};
	int animSpeed[6] = {8,8,5,8,8,5};
};

#endif