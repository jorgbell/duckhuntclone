#include "PatoGO.h"
#include <Platform.h>
#include <Renderer.h>
#include <Game.h>
#include <iostream>
PatoGO::PatoGO()
{

}
PatoGO::~PatoGO()
{

}
void PatoGO::update(float t)
{
	//empezaremos a contar el tiempo de vida del pato desde que salga del matorral por primera vez
	if (_isOutOfBush) {
		lifetime -= t;
		if (lifetime <= 0)
			_bounce = false;
	}

	//independientemente de si ha salido del matorral o no, modificamos la animacion
	lastChange += t;

	//si ha sido disparado, debemos comprobar si ha terminado el tiempo en el que el pato se debe quedar quieto en el aire
	if (_shot) {
		shotTime += t;
		if (shotTime > airTime) {
			//hacemos que el pato empiece a caer en el siguiente frame
			shotTime = 0;
			falling();
		}
	}
	//si está cayendo en picado
	else if (_falling)
	{
		if (lastChange > falling_change) {
			lastChange = 0;
			_loop++;	//pasa al siguiente frame de la animacion
			_loop %= 2;	//clamp
			actualRenderIndex = (movementIndex*3) + _loop;
		}
	}
	//si está haciendo un movimiento normal
	else {
		if (lastChange > anim_change) {
			lastChange = 0;
			_loop++;	//pasa al siguiente frame de la animacion
			_loop %= 4;
			if(_loop == 3)
				actualRenderIndex = (movementIndex*3) + 1 ;
			else
				actualRenderIndex = (movementIndex * 3) + _loop;
		}
	}
}

void PatoGO::updatePhysics(float t)
{
	//float mag = std::sqrt((_dirX * _dirX) + (_dirY * _dirY));
	x += _dirX * _vel * t;
	y += _dirY * _vel * t;
	//COMPRUEBA SI EL PATO HA SALIDO DEL MATORRAL
	if (!_isOutOfBush && y < Renderer::Instance()->getHeight() - (Game::ORIGINAL_TILE * Game::aspect * 8) - height) {
		_isOutOfBush = true;
	}
}

/*
	Este método devuelve true si:
		- El pato esta vivo (temporizador>0) y tiene que rebotar (cambiar su dirección)
		- El pato tiene que morir (temporizador <=0, bounce = false) y está totalmente fuera de la pantalla
*/
bool PatoGO::CompruebaRebote()
{
	p = NONE;

	bool hitRightWall = x + width >= Renderer::Instance()->getWidth();
	bool hitLeftWall = x <= 0;
	bool hitBottomWall = y + height >= Renderer::Instance()->getHeight();
	bool hitTopWall = y <= 0;

	if (!_bounce) {
		bool outOfScreen = (x + width < 0) || (x > Renderer::Instance()->getWidth()) || (y + height < 0) || (y > Renderer::Instance()->getHeight());
		return outOfScreen;
	}

	if (_dirY > 0) { // Abajo
		if (_dirX > 0) { // Derecha
			if (hitRightWall) {
				p = RIGHT;
				return true;
			}
			else if (hitBottomWall) {
				p = BOTTOM;
				return true;
			}
		}
		else { // Izquierda
			if (hitLeftWall) {
				p = LEFT;
				return true;
			}
			else if (hitBottomWall) {
				p = BOTTOM;
				return true;
			}
		}
	}
	else { // Arriba
		if (_dirX > 0) { // Derecha
			if (hitRightWall) {
				p = RIGHT;
				return true;
			}
			else if (hitTopWall) {
				p = TOP;
				return true;
			}
		}
		else { // Izquierda
			if (hitLeftWall) {
				p = LEFT;
				return true;
			}
			else if (hitTopWall) {
				p = TOP;
				return true;
			}
		}
	}

	return false;
}

void PatoGO::changeData(float x_, float y_, int vel, float dirX, std::vector<Sprite> duckSprites)
{
	//inicializa la posicion y direccion
	x = x_; y = y_; _vel = vel; _dirX = dirX; _dirY = -1 + std::abs(dirX);  
	//inicializa los sprites del GameObject
	GameObject::setSpriteArray(duckSprites);
	//inicializa la animacion
	movementIndex = HORIZONTAL_RIGHT;
	_loop = 0;
	//inicializa las variables de control: no está muerto, no ha sido disparado, no está cayendo, aún no ha salido del matorral
	_bounce = true;
	_dead = false;
	_shot = false;
	_falling = false;
	_isOutOfBush = false;
	//inicializa los contadores de tiempo de vida y de cambio de animacion
	lifetime = LIFETIME; // (ms) -> 4 segundos
	lastChange = Platform::Instance()->getTime();
}

void PatoGO::changeDir(float dX, float dY)
{
	_dirX = dX;
	_dirY = dY;
	checkDirAnim();
}

void PatoGO::checkDirAnim()
{
	/*
	Comprobamos el ángulo del pato para determinar cual de todos los posibles grupos de sprites es.
	Este metodo se utilizara para cambiar entre los sprites de movimiento, pero no de los sprites de muerte
	El sprite exacto se determinará según el indice indicado aquí (de 3 en 3) + el loop en el que se encuentre (3 posibles sprites por cada grupo).
	*/

	if (_dirX > 0.0f) {		//I & IV
		if (_dirY < 0.0f) {	//I
			if (_dirX > 0.866f) {	//[0º,30º]
				movementIndex = HORIZONTAL_RIGHT;
			}
			else if (_dirX > 0.5f) {	//[30º,60º]
				movementIndex = UP_RIGHT;
			}
			else {	//[60º, 90º]
				movementIndex = UP;
			}
		}
		else {				//IV

			if (_dirX > 0.866f) {		//[330º,0º]
				movementIndex = HORIZONTAL_RIGHT;
			}
			else {						//[270º, 330º]
				movementIndex = UP_RIGHT;
			}
		}
	}
	else {					//II & III
		if (_dirY < 0.0f) {	//II

			if (_dirX > -0.5f) {		//[90º,120º]
				movementIndex = UP;
			}
			else if (_dirX > -0.866f) {	//[120º,150º]
				movementIndex = UP_LEFT;
			}
			else {						//[150º, 180º]
				movementIndex = HORIZONTAL_LEFT;
			}
		}
		else {				//III
			if (_dirX < -0.866f) {		//[180º,210º]
				movementIndex = HORIZONTAL_LEFT;
			}
			else {						//[210º, 270º]
				movementIndex = UP_LEFT;
			}
		}
	}
	//cambiamos el tamaño de los sprites en caso de que sea un sprite horizontal
	if (movementIndex == HORIZONTAL_RIGHT || movementIndex == HORIZONTAL_LEFT) 
		width = height * 1.25;
	else 
		width = height;
}


void PatoGO::shot()
{
	//cambia la variable de control
	_shot = true;
	//cambia el frame que se debe animar al ultimo del array (es el de shot)
	actualRenderIndex = (FALLING * 3) + 2; //en el array, se encuentra justo despues de la posicion de falling + 2 puesto que falling se compone de 2 sprites
	//cambia la velocidad y direccion para dejar al sprite quieto en el aire
	_vel = 0;
	_dirX = 0;
	_dirY = 0;
	//nos aseguramos de que el tamaño sea cuadrado
	width = height;
}

void PatoGO::falling()
{
	//cambia las variables de control
	_shot = false;
	_falling = true;
	_bounce = false;
	//cambia el estado de la animacion
	movementIndex = FALLING;
	_loop = 0;
	//cambia la velocidad y direccion para que el gameobject caiga en picado
	_dirY = 1;
	_vel = 100 * Game::aspect;
	//Nos aseguramos de que el ancho y el alto sean los correspondientes
	width = height * 0.75;
}

void PatoGO::kill() { _dead = true; }

bool PatoGO::isDead() { return _dead; }

bool PatoGO::isCreated() { return _isOutOfBush; }

bool PatoGO::isShot() { return _shot; }

bool PatoGO::isFalling() { return _falling; }

bool PatoGO::checkShoot(float cX, float cY)
{
	//solo se podra disparar a un pato que ya haya salido de los matorrales
	if (!_isOutOfBush)
		return false;
	//comprobamos si el cursor colisiona con el pato
	if (cX >= x && cX <= (x + width) && cY >= y && cY <= (y + height)) {
		return true;
	}
	else return false;
}

PatoGO::PARED PatoGO::getPared() { return p; }
