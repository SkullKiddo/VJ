#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#define NUM_ENEMIES 2

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"
#include "TexturedQuad.h"
#include "Texture.h"

class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();
	void startPlaying();
	void stopPlaying();
	bool getPlaying();
	bool getExit();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture spritesheet;
	Texture spritesheet_options;
	Sprite *sprite;
	Sprite *sprite_options;

	bool play = false;
	bool exit = false;

	glm::ivec2 sizeMenu;
};


#endif // _SCENE_INCLUDE