#include "PlatformPC.h"
#include "SDLEventObserver.h"
#include <SDL.h>
#include <iostream>

PlatformPC* PlatformPC::_instance = nullptr;

PlatformPC::PlatformPC() {};

PlatformPC* PlatformPC::Instance() {
	return _instance;
}

bool PlatformPC::Init() {
	if (_instance == nullptr) {
		_instance = new PlatformPC();
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;
	srand(std::time(nullptr));
	return true;
};

bool PlatformPC::Release() {
	SDL_Quit();
	delete _instance;
	return true;
}

bool PlatformPC::tick() {

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) return false;
			break;
		case SDL_QUIT:
			return false;
			break;
		default:
			break;
		}
		sendToObserver(event);
	}
	return true;
};

int PlatformPC::random()
{
	return rand();
}

uint32_t PlatformPC::getTime()
{
	return SDL_GetTicks();
}

FILE* PlatformPC::openFile(const char* fileName_)
{
	std::string root = "./";
	root += fileName_;
	FILE* file;
	fopen_s(&file, root.c_str(), "r");

	if (!file) {
		std::cerr << "ERROR: Archivo no encontrado: " << root << std::endl;
	}
	return file;
}

void PlatformPC::addObserver(SDLEventObserver* o) {
	_instance->observers.push_back(o);
}

void PlatformPC::sendToObserver(const SDL_Event& e) {
	for (SDLEventObserver* observer : _instance->observers )
	{
		observer->listen(e);
	}
}