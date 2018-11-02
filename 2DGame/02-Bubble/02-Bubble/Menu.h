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
	int getChoice();

	enum adv_anim { IDLE_ADV };
	enum greenAdv_anim { IDLE_GREEN_ADV };
	enum knigh_anim { IDLE_KNIGHT };
	enum sword_anim { IDLE_SWORD };

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture spritesheet;
	Texture spritesheet_options;
	Texture spritesheet_adv;
	Texture spritesheet_greenAdv;
	Texture spritesheet_knight;
	Texture spritesheet_sword;

	Sprite *sprite;
	Sprite *sprite_options;
	Sprite *sprite_adv;
	Sprite *sprite_greenAdv;
	Sprite *sprite_knight;
	Sprite *sprite_sword;

	bool play = false;
	bool exit = false;
	bool selection = false;
	bool first = true;
	int choice = 1;

	void selectionMenu();

	glm::ivec2 sizeMenu;
};


#endif // _SCENE_INCLUDE