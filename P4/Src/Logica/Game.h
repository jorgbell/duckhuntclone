#pragma once
#include <Input.h>
#include "PoolPatos.h"
#include "GameObject.h"
#include "Cursor.h"

//clase de mas alto nivel que gestiona la logica del juego
class Game
{
public:
	//Se inicializa llamado a init y pasando el numero de patos simultaneos deseados en pantalla
	Game();
	//Elimina sus estructuras de datos (pool de patos, array de images y sprites)
	~Game();
	//Para cada pato, comrpueba su posicion, la actualiza y elimina los patos muertos de la pool
	void updatePhysics(float t);
	//Llama al update de cada pato y manda a la pool a renderizarse
	void update(float t);
	//llama a 'loadSprites', crea la pool y la inicializa rellenandola de patos
	bool InitPool(int max = 5);
	//asumiendo que las pantallas siempre seran 16:9, siempre será el mismo ratio.
	static float aspect;
	//valores originales de ancho, alto de pantalla
	static float ORIGINAL_WIDTH;
	static float ORIGINAL_HEIGHT;
	//valor original que tenía un tile en el videojuego. Usaremos ese valor para colocar los objetos en pantalla, sabiendo cuantos tiles ocupaban.
	static int ORIGINAL_TILE;
	//umbrales de uso de los gatillos.
	static float PRESS_UMBRAL;
	static float RELEASE_UMBRAL;
	//posicion a partir de la cual empezarán a poder ser disparados los patos (encima de los arbustos).
	static float SUELO;
	//pool de patos. es pública para que sea accesible por el Cursor.
	static PoolPatos* _poolPatos;

private:
	
	//Método de carga de imágenes y sprites.
	bool loadGO();
	void handleInput();

	//vector de objetos
	std::vector<GameObject*> gameObjects;
	//referencia al objeto cursor
	Cursor* game_cursor;
	//input mapeado al mando de ps4 en cada frame
	InputInfo catchedInput;

};

