#ifndef _HBANDIT_INCLUDE
#define _HBANDIT_INCLUDE

#include "Enemy.h"

class HeavyBandit : public Enemy
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void hit();
};

#endif