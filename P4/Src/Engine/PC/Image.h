#pragma once
#ifndef IMAGE_H_
#define IMAGE_H_
#include <stdint.h>
//forward declaration
class SDL_Texture;
class Image {

public:
	Image();
	~Image();
	float width() const;
	float height() const;
	SDL_Texture* getSdlTexture() const;
	//m�todo est�tico factor�a para crear una imagen. Tanto la imagen de PC como la de PS4 tienen la misma factoria, pero est� implementada de manera distinta.
	//De esta manera creamos la imagen en dos pasos, tambi�n para gestionar los errores.
	//Crea una imagen, le da los valores de ancho, alto y el array de pixeles, intentar� crear la textura de SDL, y devolver� una imagen creada o nula.
	static Image* buildImage(uint32_t* pixels, float w, float h);
	uint32_t* get_imagePixels() const;		//pixeles integros de la imagen
	const char* filename;
private:
	//textura de sdl
	SDL_Texture* _sdlTexture;
	//ancho y alto de la imagen
	float _width, _height;
	//pixeles integros de la imagen
	uint32_t* _imageBuffer;
	//m�todo que crear� una textura de sdl. En caso de generar un error, devuelve false y se gestionar� el error en la l�gica.
	bool createSDLTexture();
};

#endif // !IMAGE_H_

