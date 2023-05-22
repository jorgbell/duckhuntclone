#ifndef INPUTPC_H_
#define INPUTPC_H_
#include "SDLEventObserver.h"
#include "InputInfo.h"
#include <vector>

//Número mágico usado para que la experiencia de mover el ratón (que estará mapeado al gyro)
//  el mando de PS4 se sientan similares. Se multiplicará el input de ratón por este número.
//De esta manera, en la lógica se recibirá un mismo input y reaccionará igual independientemente de la plataforma.
#define PS4_TO_MOUSE (INT16_MAX / 1600.0)
//forward declaration
struct _SDL_Joystick;
struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

class InputPC : SDLEventObserver{
public:
	//devuelve la instancia
	static InputPC* Instance();
	//inicializa la instancia
	static void Init();
	//libera la instancia y libera el mando
	static void Release();
	//recorre todos los eventos de SDL que ha recibido en este frame, los gestiona
	//y posteriormente libera el vector de eventos.
	void tick();
	//devuelve el input del frame actual,
	//mapeado al mando de PS4 que se usará en la lógica.
	InputInfo getFrameInput() { return frameInput; }
	//Método heredado de SDLEventObserver. Añade un evento recibido al vector de eventos del frame.
	virtual void listen(const SDL_Event& sdlevent);

private:
	//instancia
	static InputPC * _instance;
	//input mapeado al mando de PS4.
	InputInfo frameInput;
	//controlador del mando para SDL.
	SDL_GameController* controller = nullptr;
	//vector de eventos del frame actual.
	std::vector<SDL_Event> sdlEvents;
	//variable de control que usamos porque tanto el movimiento con las flechas como con el joystick del mando se mapean al stick izquierdo del mando de ps4.
	//cuando se usa un mando, se debe indicar que el valor del frame anterior sea el inicial del siguiente frame.
	//con las flechas, no se debe hacer eso. Por eso usamos esta variable de control.
	bool resetLeftStick = false;

};
#endif // !INPUTPC_H_
#pragma once
