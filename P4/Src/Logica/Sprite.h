#pragma once
#ifndef SPRITE_H_
#define SPRITE_H_

#include <Image.h>
class Sprite
{
public:
	Sprite() {};
	//constructora que recibe una imagen y las cuatro esquinas del cuadrado que conforma el sprite que se encuentra dentro de la imagen
	Sprite(const Image* i, float x1, float x2, float y1, float y2);
	//dibuja en pantalla (llamada directa a Renderer.h) el sprite
	void draw(float tx1, float ty1, float w, float h) const;
	//Metodo estatico que nos sirve para crear un sprite a partir de un sprite sheet
	/*
		entendemos un sprite sheet como un conjunto de sprites de un mismo tamaño que se dividen en x filas y columnas
		trataremos la imagen que se pasa como parametro como un sprite sheet, e indicandole en qué fila y columna y cuantas filas y columnas tiene este sprite sheet,
		este método devolverá un sprite con su cuadrado definido dentro de esta imagen.
	*/
	static Sprite fromRegularSpriteSheet(const Image* i, int numFil, int numCol, int fila, int col);
	//getters
	float width() { return w; } float height() { return h; }
private:
	//imagen source
	const Image* _mySourceImage;
	//rectángulo source: posición relativa a la imagen
	float sx1, sx2, sy1, sy2;
	//ancho y alto del sprite
	float w; float h;

};
#endif // !SPRITE_H_