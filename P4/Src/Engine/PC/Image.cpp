#include "Image.h"
#include "RendererPC.h"
#include <SDL.h>
#include <iostream>

Image::Image()
{
}

Image* Image::buildImage(uint32_t* pixels, float w, float h) {
    Image* i = new Image();
    i->_width = w; i->_height = h; i->_imageBuffer = pixels;
    if (!i->createSDLTexture()) return nullptr;
    return i;
}

Image::~Image()
{
    SDL_DestroyTexture(_sdlTexture);
    free(_imageBuffer);
}

float Image::width() const
{
    return _width;
}

float Image::height() const
{
    return _height;
}

SDL_Texture* Image::getSdlTexture() const
{
    return _sdlTexture;
}
uint32_t* Image::get_imagePixels() const
{
    return _imageBuffer;
}

bool Image::createSDLTexture() {

    //PARAMETROS: buffer de la imagen, ancho y alto de la imagen en pixeles, profundidad en bits, tamaño en bytes de una fila de pixeles (width * 4bytes),formato
    //el formato esta porque hay que meterle una mascara de colores, y ese define se cambia solo segun SDL_BYTE_ORDER
    SDL_Surface* s = SDL_CreateRGBSurfaceFrom(_imageBuffer, _width, _height, 32, 4 * _width, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (s == NULL) {
        std::cerr << "ERROR en la creacion de SDL Surface: " << SDL_GetError() << std::endl;
        return false;
    }
    _sdlTexture = SDL_CreateTextureFromSurface(RendererPC::Instance()->_renderer, s);
    if (_sdlTexture == NULL) {
        std::cerr << "ERROR en la creacion de SDL texture: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_FreeSurface(s);
    return true;
}