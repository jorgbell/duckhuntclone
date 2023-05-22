#include "RendererPC.h"
#include "Image.h"
#include <iostream>
#include <SDL.h>

RendererPC* RendererPC::_instance = nullptr;
RendererPC::RendererPC() {};

RendererPC* RendererPC::Instance() {
	return _instance;
}

bool RendererPC::Init(const char* window_name, bool fullscreen, int window_width, int window_height) {
	if (_instance == nullptr) {
		_instance = new RendererPC();
	}
	Uint32 flags;
	if (fullscreen) flags = SDL_WindowFlags::SDL_WINDOW_SHOWN || SDL_WindowFlags::SDL_WINDOW_FULLSCREEN;
	else
		flags = SDL_WindowFlags::SDL_WINDOW_SHOWN;


	_instance->_window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, flags);
	if (_instance->_window == nullptr) {
		std::cerr << "Error al crear la ventana" << SDL_GetError() << '\n';
		return false;
	}
	//TODO: Aqui le estoy metiendo VSYNC, igual habria que definirlo en otro sitio.
	_instance->_renderer = SDL_CreateRenderer(_instance->_window, -1, SYNCTYPE);
	if (_instance->_renderer == nullptr) {
		std::cerr << "Error al crear el renderer" << SDL_GetError() << '\n';
		return false;
	}
	SDL_SetRenderTarget(_instance->_renderer, NULL);

	return true;
};

bool RendererPC::Release() {
	SDL_DestroyRenderer(_instance->_renderer);
	SDL_DestroyWindow(_instance->_window);
	delete _instance;
	return true;
}

void RendererPC::clear(uint32_t color) {
	uint8_t a = (color & 0xFF000000) >> 24;
	uint8_t r = (color & 0x00FF0000) >> 16;
	uint8_t g = (color & 0x0000FF00) >> 8;
	uint8_t b = (color & 0x000000FF);

	SDL_SetRenderDrawColor(_instance->_renderer, r, g, b, a); //TODO cambiar por color
	if (SDL_RenderClear(_instance->_renderer) < 0)
		std::cerr << "error clear" << std::endl;
}

void RendererPC::putPixel(float x, float y, uint32_t color)
{

}


//este metodo tendra que tener otra version para hacer el conteo de frames
void RendererPC::drawRect(float x, float y, float width, float height, uint32_t color) {
	uint8_t a = (color & 0xFF000000) >> 24;
	uint8_t r = (color & 0x00FF0000) >> 16;
	uint8_t g = (color & 0x0000FF00) >> 8;
	uint8_t b = (color & 0x000000FF);
	SDL_Rect rectangle;
	rectangle.x = x; rectangle.y = y; rectangle.w = width; rectangle.h = height;
	SDL_SetRenderDrawColor(_instance->_renderer, r, g, b, a); //TODO cambiar por color
	SDL_RenderFillRect(_instance->_renderer, &rectangle);

}


void RendererPC::drawImage(const Image* image, float sx1, float sy1, float sx2, float sy2, float tx1, float ty1, float tx2, float ty2)
{
	SDL_Rect source = { sx1,sy1,sx2 - sx1,sy2 - sy1 };
	SDL_Rect dest = { tx1,ty1,tx2 - tx1+1,ty2 - ty1+1 };

	SDL_RenderCopy(_instance->_renderer, image->getSdlTexture(), &source, &dest);
}

void RendererPC::present() {
	SDL_RenderPresent(_instance->_renderer);
}

int RendererPC::getWidth() {
	int w, h;
	SDL_GetWindowSize(_instance->_window, &w, &h);
	return w;
}

int RendererPC::getHeight() {
	int w, h;
	SDL_GetWindowSize(_instance->_window, &w, &h);
	return h;
}