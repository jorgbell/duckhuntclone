#include "Cursor.h"
#include "Game.h"
#include <Input.h>
#include <Renderer.h>

Cursor::Cursor(): 
	GameObject(Renderer::Instance()->getWidth() / 2, Renderer::Instance()->getHeight() / 2, CENTER)
{}

void Cursor::update(float t)
{
}

void Cursor::handleInput()
{
	auto catchedInput = Input::Instance()->getFrameInput();

	if (!shot) {
		if (catchedInput.cross || catchedInput.circle || catchedInput.l2 > Game::PRESS_UMBRAL || catchedInput.r2 > Game::PRESS_UMBRAL) {
			shot = true;

			int i = Game::_poolPatos->GetPatos().size() - 1;
			bool encontrado = false;
			while (!encontrado && i >= 0) {
				encontrado = Game::_poolPatos->GetPatos()[i]->checkShoot(x+ (width / 2), y+ (height / 2));
				i--;
			}
			i++;
			if (encontrado && !Game::_poolPatos->GetPatos()[i]->isShot() && !Game::_poolPatos->GetPatos()[i]->isFalling())  Game::_poolPatos->GetPatos()[i]->shot();
		}
	}
	else {
		if (!catchedInput.cross && !catchedInput.circle && catchedInput.l2 < Game::RELEASE_UMBRAL && catchedInput.r2 < Game::RELEASE_UMBRAL) {
			shot = false;
		}
	}

	SetPos(x + ((15 * catchedInput.leftStickX) + (catchedInput.gyroY*-20)),
		y + (15 * catchedInput.leftStickY) + (catchedInput.gyroX*-20));
	clampPosToWindowSize();
}
