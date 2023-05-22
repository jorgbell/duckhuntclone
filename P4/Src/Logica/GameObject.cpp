#include "GameObject.h"
#include "Renderer.h"
#include <Platform.h>
#include "Decoder.h"
#include "Game.h"
#include "RenderThread.h"
#include <iostream>

GameObject::GameObject(float x_, float y_, CLAMP_CONFIG c, float width_, float height_)

	: clamping(c),x(x_), y(y_), width(width_), height(height_)
{
}

GameObject::~GameObject()
{
	for (auto i : loadedImages) {
		if(i!=nullptr)
			delete i;
	}
}

bool GameObject::loadImageAsSingle(const char* path) {
	Image* im = Decoder::decodeImage(path);
	if (!im)
		return false;
	loadedImages.push_back(im);

	//añadimos al array un sprite cuyo tamaño source  es la imagen al completo
	_sprites.push_back(Sprite(im, 0, im->width(), 0, im->height()));
	//en caso de no haber definido el tamaño del gameobject, lo define aqui
	if (width == 0.0f || height == 0.0f) {
		SetSize(_sprites[0].width(), _sprites[0].height());
	}

	return true;

}
void GameObject::setSpriteArray(std::vector<Sprite> s) {
	_sprites = s;
	//en caso de no haber definido el tamaño del gameobject, lo define aqui
	if (_sprites.size()>0 || width == 0.0f || height == 0.0f) {
		SetSize(_sprites[0].width(), _sprites[0].height());
	}
}

void GameObject::clearSpriteArray() {
	_sprites.clear(); _sprites.resize(0);
}

void GameObject::render() {
	//dibuja el sprite en la posicion del Gameobject, con el ancho y el alto que tiene el propio GameObject.
	RenderThread::Enqueue({ _sprites[actualRenderIndex], x, y, width, height,CommandType::drawSprite });
}


/*Getters & Setters*/
void GameObject::SetPos(float x_, float y_)
{
	x = x_; y = y_;
}
void GameObject::SetSize(float w_, float h_) {
	width = w_ * Game::aspect;
	height = h_ * Game::aspect;
	//el maximo para el clamp en pantalla dependerá del tamaño del gameobject.
	if (clamping == ORIGIN) {
		maxX = Renderer::Instance()->getWidth() - width;
		maxY = Renderer::Instance()->getHeight() - height;
		minX = 0; minY = 0;
	}
	else if (clamping == CENTER) {
		maxX = Renderer::Instance()->getWidth() - width/2;
		maxY = Renderer::Instance()->getHeight() - height/2;
		minX = -width / 2; minY = -height / 2;
	}
	else{
		maxX = Renderer::Instance()->getWidth();
		maxY = Renderer::Instance()->getHeight();
		minX = -width; minY = -height;
	}

}

std::pair<float, float> GameObject::GetPos()
{
	 return std::make_pair(x, y); ;
}

float GameObject::GetX()
{
	return x;
}

float GameObject::GetY()
{
	return y;
}

float GameObject::GetW()
{
	return width;
}

float GameObject::GetH()
{
	return height;
}

void GameObject::SetX(float x_)
{
	x = x_;
}

void GameObject::SetY(float y_)
{
	y = y_;
}

void GameObject::SetW(float w_)
{
	width = w_ * Game::aspect;
	if (clamping == ORIGIN) {
		maxX = Renderer::Instance()->getWidth() - width;
	}
	else if (clamping == CENTER) {
		maxX = Renderer::Instance()->getWidth() - width / 2;
	}
	else {
		maxX = Renderer::Instance()->getWidth();
	}
}

void GameObject::SetH(float h_)
{
	height = h_ * Game::aspect;
	if (clamping == ORIGIN) {
		maxY = Renderer::Instance()->getHeight() - height;
	}
	else if (clamping == CENTER) {
		maxY = Renderer::Instance()->getHeight() - height / 2;
	}
	else {
		maxY = Renderer::Instance()->getHeight();
	}
}

