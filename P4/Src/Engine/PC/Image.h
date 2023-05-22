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
	//método estático factoría para crear una imagen. Tanto la imagen de PC como la de PS4 tienen la misma factoria, pero está implementada de manera distinta.
	//De esta manera creamos la imagen en dos pasos, también para gestionar los errores.
	//Crea una imagen, le da los valores de ancho, alto y el array de pixeles, intentará crear la textura de SDL, y devolverá una imagen creada o nula.
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
	//método que creará una textura de sdl. En caso de generar un error, devuelve false y se gestionará el error en la lógica.
	bool createSDLTexture();
};

#endif // !IMAGE_H_

