#pragma once
#ifndef PLATFORMPC_H_
#define PLATFORMPC_H_

#include <stdint.h>
#include <ostream>
#include <vector>

#include <SDL_events.h>
class SDLEventObserver;
class PlatformPC {
public:
	//constructora
	PlatformPC();
	//devuelve la instancia
	static PlatformPC* Instance();
	//inicializa la instancia
	static bool Init();
	//libera la instancia y sale de la ventana de SDL
	static bool Release();

	//lectura de eventos de input
	bool tick();
	//metodo auxiliar que devuelve un numero aleatorio
	int random();
	//metodo auxiliar que devuelve el tiempo actual
	uint32_t getTime();
	//metodo para intentar abrir un archivo. Devolvera un FILE* relleno o nulo según se haya o no realizado bien la apertura del fichero.
	//el error se gestionará en la logica en caso de ser un archivo nulo.
	FILE* openFile(const char* fileName_);
	//añade un observer a la lista de observer que quieran escuchar los eventos de sdl.
	void addObserver(SDLEventObserver* o);
	//manda un evento de SDL recibido a cada uno de los observers asociados a la plataforma.
	void sendToObserver(const SDL_Event& e);
private:
	//instancia
	static PlatformPC* _instance;
	//lista de observers.
	std::vector <SDLEventObserver*> observers;
};
#endif // !PLATFORMPC_H_