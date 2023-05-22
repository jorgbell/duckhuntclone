#include <Platform.h>
#include <Renderer.h>
#include <RenderThread.h>
#include <Input.h>
#include "Game.h"
#include <iostream>

#define DELTA 16;				//33ms para 30 fps, 16ms para 60 fps
/*
	PAR�METROS MODIFICABLES PARA HACER PRUEBAS:
		- TIMETOPRINTFPS: Intervalo entre impresiones de fps por consola en ms
		- NUMPATOS: Numero de patos en pantalla (5,20,50,100)
*/
#define TIMETOPRINTFPS 1000;
#define NUMPATOS 5;
#define WIN_WIDTH 1920;
#define WIN_HEIGHT 1080;
#define FULL_SCREEN false;
#define WINDOW_NAME "Patos Voladores";

int main(int argc, char* argv[]) { 
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*-----------------------------------------------INICIALIZACION DEL MOTOR----------------------------------------------------*/

	//inicializacion del motor
	if (!Platform::Init()) {
		std::cin.get();
		return 1;
	}
	int w = WIN_WIDTH;
	int h = WIN_HEIGHT;
	bool f = FULL_SCREEN;
	const char* n = WINDOW_NAME;

	if (!Renderer::Init(n, f, w, h)) {
		Platform::Release();
		std::cin.get();
		return 1;
	}

	Input::Init();

	//inicializamos el thread de renderizado.
	RenderThread::Init();

	int iniTime = Platform::Instance()->getTime();	//tiempo en el que inicia la logica, para hacer el recuento final
	int timeToPrint = TIMETOPRINTFPS; int numpat = NUMPATOS; float delta = DELTA;
	int tiemposobrante = 0;	int fps = 0;


	//inicializacion de la logica
	Game* game = new Game();
	if (!game->InitPool(numpat)) {

		//limpia las instancias del motor
		delete game;
		RenderThread::End();
		Input::Release();
		Renderer::Release();
		Platform::Release();
		std::cin.get();
		return 1;
	}


	//tiempo del ultimo frame
	int lastTime = Platform::Instance()->getTime();

	/*-----------------------------------------------BUCLE PRINCIPAL----------------------------------------------------*/
	
	while (Platform::Instance()->tick()) {	//Mientras en handleinput no devuelva false
		//calculos deltatime
		int actualTime = Platform::Instance()->getTime();
		int deltaTime = actualTime - lastTime;
		lastTime += deltaTime;	tiemposobrante += deltaTime;

		//clear pantalla (rgb)
		RenderThread::Enqueue({ 0x3CBCFC, CommandType::clear });

		//actualizamos el estado del Input, para luego coger el input cuando sea necesario
		Input::Instance()->tick();


		//update fisico (FixedUpdate)
		while (tiemposobrante > delta) {
			game->updatePhysics(delta / 1000.0); //ms
			tiemposobrante -= delta;
		}

		//update normal
		game->update(deltaTime);

		//Espera Activa
		while (RenderThread::getFramesDiff() > 2);
		//PRESENT
		RenderThread::Enqueue({ CommandType::present });


		//calculos de FPS
		timeToPrint -= deltaTime;
		if (timeToPrint <= 0) {
			timeToPrint = TIMETOPRINTFPS;
			std::cout << "fps: " << (fps / (timeToPrint/1000)) << std::endl;
			fps = 0;		
		}
		fps++;
	}
	
	//limpia las instancias del motor
	delete game;
	RenderThread::End();
	Input::Release();
	Renderer::Release();
	Platform::Release();
	return 0;
}