#pragma once
#include "Sprite.h"
#include <utility>
#include <string>
#include <vector>
class GameObject
{
public:
	/*
	Clase para gestionar las posiciones, tamanos y carga de imagenes de las entidades del juego
	*/
	enum CLAMP_CONFIG{CENTER,ORIGIN,INV_ORIGIN};
	GameObject(float x_ = 0.0f, float y_ = 0.0f, CLAMP_CONFIG c = ORIGIN, float width_ = 0.0f, float height_ = 0.0f);
	virtual ~GameObject();
	virtual void update(float t){};
	virtual void updatePhysics(float t){};
	virtual void handleInput(){};

	//carga una nueva imagen y la añade como un sprite al array de sprites a dibujar.
	//la carga se realiza aquí y el gameobject gestionará la limpieza de la imagen.
	//en caso de que se quiera crear un gameobject cuyas imagenes se carguen fuera (ej: repetir misma imagen para varios gameobejcts),
	//la gestión de la imagen se realizará fuera, y se le añadirá simplemente un array de sprites a dibujar.
	bool loadImageAsSingle(const char* path);
	//método que indica al gameObject el array de sprites que debe de usar (previamente cargados)
	virtual void setSpriteArray(std::vector<Sprite> s);
	
	//metodo para limpiar el array de sprites
	void clearSpriteArray();
	//renderiza el indice actual del array de sprites
	virtual void render();
	//setters y getters.
	//cuando se hace un set del ancho, alto, o ambos a la vez, se cambian los valores de los máximos de la pantalla para el clamp.
	void SetPos(float x_, float y_);
	void SetSize(float w_, float h_);
	std::pair<float, float> GetPos();
	float GetX();float GetY();float GetW();float GetH();
	void SetX(float x_);void SetY(float y_);void SetW(float w_);void SetH(float h_);

protected:
	//posicion, ancho, alto
	float x, y, width, height;
	//máximos y minimos de la pantalla para el clamp del objeto
	float maxX, maxY, minX, minY;
	//vector de sprites del cual se escogerá el que toque renderizar
	std::vector<Sprite> _sprites;
	//indice de renderizado para elegir el sprite que toque renderizar. Desde esta clase no se modificará.
	int actualRenderIndex = 0; 
	//imagenes cargadas, en caso de almacenarse aquí.
	std::vector<const Image*> loadedImages;
	
	//clamp
	CLAMP_CONFIG clamping = ORIGIN;
	//método para mantener el gameobject en los confines de la pantalla. Comprueba si la posición supera alguno de los lados y lo coloca dentro.
	void clampPosToWindowSize() {
		if (x >= maxX) x = maxX;
		else if (x <= minX) x = minX;

		if (y >= maxY) y = maxY;
		else if (y <= minY) y = minY;
	}
};