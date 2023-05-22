#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <Platform.h>
#include <Decoder.h>
#include <vector>
#include "PoolPatos.h"


float Game::aspect = 0.0f;
float Game::ORIGINAL_WIDTH = 320;
float Game::ORIGINAL_HEIGHT = 180;

float Game::PRESS_UMBRAL = 0.7f;
float Game::RELEASE_UMBRAL = 0.3f;
float Game::SUELO = -1.0f;
int Game::ORIGINAL_TILE = 8;
PoolPatos* Game::_poolPatos = nullptr;


Game::Game()
{
}


Game::~Game() {
	delete _poolPatos;

	for (auto i : gameObjects) {
		delete i;
	}
	gameObjects.clear();

}
bool Game::loadGO()
{

	//Carga del resto de sprites EN EL ORDEN DE DIBUJADO y objetos estaticos
	GameObject* suelo = new GameObject();
	if (!suelo->loadImageAsSingle("assets/suelo.rgba"))
		return false;
	SUELO = Renderer::Instance()->getHeight() - suelo->GetH();
	suelo->SetY(SUELO);

	GameObject* arbol = new GameObject((ORIGINAL_TILE * aspect) * 4);
	if (!arbol->loadImageAsSingle("assets/arbol.rgba"))
		return false;
	arbol->SetY(SUELO - arbol->GetH());

	GameObject* arbusto = new GameObject(Renderer::Instance()->getWidth() - ((ORIGINAL_TILE * aspect) * 12));
	if (!arbusto->loadImageAsSingle("assets/arbusto.rgba"))
		return false;
	arbusto->SetY(SUELO - arbusto->GetH());

	game_cursor = new Cursor();
	if (!game_cursor->loadImageAsSingle("assets/cursor.rgba"))
		return false;

	//añadidos en orden de renderizado
	gameObjects.push_back(suelo);
	gameObjects.push_back(arbusto);
	gameObjects.push_back(arbol);
	gameObjects.push_back(game_cursor);

	return true;
}

void Game::handleInput()
{
	catchedInput = Input::Instance()->getFrameInput();
	game_cursor->handleInput();
}

void Game::updatePhysics(float t)
{
	_poolPatos->updatePhysics(t);
}

void Game::update(float t)
{
	handleInput();
	_poolPatos->update(t);

	for (GameObject* go : gameObjects)
		go->render();
}

bool Game::InitPool(int max)
{
	//sacamos el factor para multiplicar todos los sprites
	aspect = Renderer::Instance()->getWidth() / ORIGINAL_WIDTH;

	if (!loadGO())
		return false;

	_poolPatos = new PoolPatos(max, Renderer::Instance()->getWidth(), Renderer::Instance()->getHeight());
	if (!_poolPatos->InitPool())
		return false;


	return true;
}
