#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE

#include "Character.h"

class Skeleton : public Character
{
public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void hit();
	virtual box hitBox();
};

#endif