#include "InputPC.h"
#include "SDL.h"
#include "Platform.h"
#include <iostream>

InputPC* InputPC::_instance = nullptr;


InputPC* InputPC::Instance() {
	return _instance;
}
void InputPC::Init()
{
	if (_instance == nullptr) {
		_instance = new InputPC();
	}
	Platform::Instance()->addObserver(_instance);
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_SetRelativeMouseMode(SDL_TRUE);

}

void InputPC::tick()
{
	//igualamos el eje del mando al del frame anterior y reseteamos el resto de valores
	float axisx = frameInput.leftStickX; float axisy = frameInput.leftStickY;
	frameInput = InputInfo();
	if (_instance->resetLeftStick) {
		frameInput.leftStickX = axisx; frameInput.leftStickY = axisy;
	}
	//variables locales para recoger input
	int x, y;
	float val;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	float h;
	//recorremos todos los eventos de sdl
	for (const SDL_Event e : _instance->sdlEvents) {

		switch (e.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) frameInput.circle = true;
			else if (e.button.button == SDL_BUTTON_RIGHT) frameInput.cross = true;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetRelativeMouseState(&x, &y);
			h = PS4_TO_MOUSE;
			//le damos la vuelta para adaptarlo al gyro de la PS4 y dividimos por un numero magico que se siente como en PS4
			frameInput.gyroX += -y / PS4_TO_MOUSE;
			frameInput.gyroY += -x / PS4_TO_MOUSE;
			break;
		case SDL_KEYUP || SDL_MOUSEBUTTONUP:
			std::cout << "SDL_BUTTON/KEY_UP" << std::endl;
			break;
		case SDL_CONTROLLERAXISMOTION:
			//valor recogido por los ejes
			val = (float)e.caxis.value / INT16_MAX;
			//segun el eje modificado, modificamos uno u otro valor
			switch (e.caxis.axis) {
			case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				frameInput.l2 = val;
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				frameInput.r2 = val;
				break;
			case SDL_CONTROLLER_AXIS_LEFTX:
				if (std::abs(val) > 0.1) {
					_instance->resetLeftStick = true;
					frameInput.leftStickX = val;
				}
				else {
					frameInput.leftStickX = 0;
				}
				break;
			case SDL_CONTROLLER_AXIS_LEFTY:
				if (std::abs(val) > 0.1) {
					_instance->resetLeftStick = true;
					frameInput.leftStickY = val;
				}
				else {
					frameInput.leftStickY = 0;
				}
				break;
			default:
				break;
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			if (controller == nullptr) {
				controller = SDL_GameControllerOpen(0);
				std::cout << "Controller connected" << std::endl;
			}
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			controller = nullptr;
			std::cout << "Controller disconnected" << std::endl;
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
				frameInput.circle = true;
			else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
				frameInput.cross = true;
			break;
		case SDL_KEYDOWN:
			_instance->resetLeftStick = false;
			break;
		}

	}
	//pilla las teclas de manera simultanea
	if (keys[SDL_SCANCODE_UP]) {
		frameInput.leftStickY = -1;
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		frameInput.leftStickY = 1;
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		frameInput.leftStickX = -1;
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		frameInput.leftStickX = 1;
	}
	if (keys[SDL_SCANCODE_RSHIFT]) {
		frameInput.r2 = 1.0f;
	}
	if (keys[SDL_SCANCODE_RCTRL]) {
		frameInput.l2 = 1.0f;
	}
	_instance->sdlEvents.clear();


}

void InputPC::Release()
{

	_instance->controller = nullptr;
	delete _instance;
	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

}

void InputPC::listen(const SDL_Event& sdlevent) {
	_instance->sdlEvents.push_back(sdlevent);
}