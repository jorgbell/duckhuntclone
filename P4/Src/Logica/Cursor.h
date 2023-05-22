#pragma once
#include "GameObject.h"
/*
Gameobject que gestionará el cursor con el que apuntaremos y dispararemos.
*/
class Cursor :
	public GameObject
{
public:
	Cursor();
	// Heredado vía GameObject
	virtual void update(float t) override;
	//comprobará el input para moverse a si mismo.
	//además, comprobará en el input si se ha pulsado el botón de disparar y si en ese instante, la posición central del cursor coincide con la de un pato.
	//En caso afirmativo, se comunicará con dicho pato para marcar el disparo.
	virtual void handleInput() override;
private:
	//variable de control para controlar que solo realice un disparo por cada pulsación. 
	//Se reiniciará cuando se suelten todos los botones de disparo después de haber disparado a un pato
	bool shot = false;
};

