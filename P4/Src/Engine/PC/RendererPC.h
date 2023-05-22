#pragma once
#ifndef RENDERERPC_H_
#define RENDERERPC_H_
#include <stdint.h>

//forward declaration
class SDL_Window;
class SDL_Renderer;
class Image;
#define ACCELERATED SDL_RendererFlags::SDL_RENDERER_ACCELERATED
#define VSYNC SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC

/*
	PAR�METROS MODIFICABLES PARA HACER PRUEBAS:
	Para cambiar las flags de renderizado de SDL (seg�n se quiera VSYNC o ACCELERATED),
	se debe modificar el #define SYNCTYPE y darle uno de los valores de las dos l�neas anteriores (ACCELERATED o VSYNC)
*/

#define SYNCTYPE VSYNC

class RendererPC {
public:
	friend class Image;
	//constructora
	RendererPC();
	//devuelve la isntancia
	static RendererPC* Instance();
	//inicializa la instancia y crea la ventana seg�n los par�metros que vienen desde la l�gica
	static bool Init(const char* window_name, bool fullscreen, int window_width, int window_height);
	//libera la instancia
	static bool Release();
	//limpia la pantalla rellenandola de un color por parametro
	void clear(uint32_t color);
	//este metodo no se usara en esta practica, pero podemos hacer pruebas con el
	void putPixel(float x, float y, uint32_t color);
	//este metodo tendra que tener otra version para hacer el conteo de frames
	void drawRect(float x, float y, float width, float height, uint32_t color);
	//dibuja una imagen indicando qu� parte de la imagen se dibujar� y en qu� posicion de la pantalla se har� (cuadrado s y cuadrado t)
	void drawImage(const Image* image, float sx1, float sy1, float sx2, float sy2, float tx1, float ty1, float tx2, float ty2);
	//muestra en la ventana todos los pixeles dibujados
	void present();
	//getters
	int getWidth();
	int getHeight();

private:

	SDL_Renderer* _renderer;
	static RendererPC* _instance;
	SDL_Window* _window;

};
#endif // !RENDERERPC_H_


