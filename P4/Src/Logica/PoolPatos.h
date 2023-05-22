#pragma once
#include<vector>
#include <tuple>
#include"PatoGO.h"
class PoolPatos
{
public:
	//constructora
	PoolPatos(int m, int screenWidth, int screenHeight);
	//destructora
	~PoolPatos();
	//inicializacion en dos pasos
	bool InitPool();
	void InitObject(PatoGO* p);
	//recorre la pool e indica a cada pato que se renderice
	void render();
	//recorre la pool para hacer el update de cada uno de los patos
	void updatePhysics(float t);
	void update(float t);

	bool ReUsePatos(int k);
	std::vector<PatoGO*> GetPatos();
private:
	//pool
	std::vector<PatoGO*> _myPool;
	//vector de velocidades: se rellena en la constructora indicando la velocidad relativa de cada uno de los patos.
	//se rellena siguiendo el orden de colores que se sigue en el resto de archivos: azul-negro-rojo
	int _data[3];
	//tamaño de la pool
	int _maxPatos;
	int screen_width, screen_height;
	/*
		spritesArray: vector que contiene los tres 'vectores de sprites'*, uno por cada color de pato [azul, negro, rojo].
					 Estos vectores se rellenan según el orden de 0-45-90º, que utilizamos también para el uso del enum SPRITE_INDEX que se encuentra en la clase Pato.h
		*'vector de sprites': Contiene los 15 sprites de un mismo color para cada angulo
							std::vector<Sprite> = [6x0º, 6x45º, 3x90º]
	*/

	//vector que viene de Game.h que contiene todos los sprites posibles separados por colores.
	std::vector<std::vector<Sprite>> spritesArray;
	//Método de carga de imágenes y sprites.
	bool loadSprites();
	//vector de imágenes cargadas para su posterior eliminación
	std::vector<Image*> loadedImages;
	/*Devuelve un angulo aleatorio coherente para el rebote de los patos contra las paredes.
	* Se usa en el updatephysics para dar un nuevo sentido y direccion a la velocidad del pato*/
	float getAngle();
	//limpia toda la pool de patos
	void flush();
};

