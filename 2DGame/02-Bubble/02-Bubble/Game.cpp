#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <Mmsystem.h>
#include <iostream>


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menu.init();
	scene.init();
	PlaySound(TEXT("audio/cyka.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
}

bool Game::update(int deltaTime)
{
	if (menu.getPlaying()) {
		if (chosen) {
			chosen = false;
			scene.setChoice(menu.getChoice());
		}
		if (first) { //El primer cop que iniciem o fem resum hem de posar que no volem tornar a sortir de la partida
			scene.keepPlaying();
			first = false;
			scene.update(deltaTime);
		}
		else if (!scene.getEscape()) scene.update(deltaTime); //si no volem pausar el joc seguim jugant
		else menu.stopPlaying();  //si volem pausar el joc deixem d'actualitzar-lo i passem al menú
	}
	else {
		menu.update(deltaTime); //si no volem iniciar partida ens quedem al menú
		first = true;
	}
	
	if (menu.getExit()) bPlay = false; //si cliquem exit des del menu sortim del joc

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (menu.getPlaying()) scene.render(); //Si estem jugant renderitzem la partida, sino el menu
	else menu.render();
}

void Game::keyPressed(int key)
{
	keys[key] = true;
	if (key == 'c') // Activar mapa de colisiones
		scene.setCollsion();
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





