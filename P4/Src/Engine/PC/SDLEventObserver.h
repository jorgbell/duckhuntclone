#pragma once
#ifndef SDLEVENTOBSERVER_H_
#define SDLEVENTOBSERVER_H_
/*
Esta clase se implementará en otras para que cualquier clase del motor que requiera de eventos de SDL los pueda recibir.
Los eventos de SDL se reciben en Platform, pero este los reenviará a cada uno de los observer
*/
#include <SDL_events.h>
class SDLEventObserver {
public:
	virtual void listen(const SDL_Event& sdlevent) = 0;
};
#endif // SDLEVENTOBSERVER_H_