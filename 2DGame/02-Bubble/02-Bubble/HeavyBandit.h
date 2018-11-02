#ifndef _HBANDIT_INCLUDE
#define _HBANDIT_INCLUDE

#include "Character.h"

class HeavyBandit : public Character
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void hit();
	box hitBox();
};

#endif