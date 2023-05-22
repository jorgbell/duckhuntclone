#pragma once
#include "GameObject.h"
/*
Gameobject que gestionar� el cursor con el que apuntaremos y dispararemos.
*/
class Cursor :
	public GameObject
{
public:
	Cursor();
	// Heredado v�a GameObject
	virtual void update(float t) override;
	//comprobar� el input para moverse a si mismo.
	//adem�s, comprobar� en el input si se ha pulsado el bot�n de disparar y si en ese instante, la posici�n central del cursor coincide con la de un pato.
	//En caso afirmativo, se comunicar� con dicho pato para marcar el disparo.
	virtual void handleInput() override;
private:
	//variable de control para controlar que solo realice un disparo por cada pulsaci�n. 
	//Se reiniciar� cuando se suelten todos los botones de disparo despu�s de haber disparado a un pato
	bool shot = false;
};

