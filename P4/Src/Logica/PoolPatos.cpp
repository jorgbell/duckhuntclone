#include "PoolPatos.h"
#include <ctime>
#include <iostream>
#include <Platform.h>
#include "Sprite.h"
#include "Game.h"
#include "Decoder.h"

PoolPatos::PoolPatos(int m, int screenWidth, int screenHeight) :
	_maxPatos(m), screen_width(screenWidth), screen_height(screenHeight)
{
	_myPool = std::vector<PatoGO*>();
	// --- CALCULO DE LA VELOCIDAD RELATIVA A LA PANTALLA
	_data[0] = 85.0f * Game::aspect;
	_data[1] = 50.0f * Game::aspect;
	_data[2] = 120.0f * Game::aspect;


}
PoolPatos::~PoolPatos() {
	flush();
	_myPool.clear();
	for (auto i : loadedImages) {
		delete i;
	}
	loadedImages.clear();
	spritesArray.clear();
}

bool PoolPatos::InitPool()
{
	if (!loadSprites())
		return false;
	for (int i = 0; i < _maxPatos; i++) {
		PatoGO* p = new PatoGO();

		InitObject(p);
		_myPool.push_back(p);

	}
	return true;

}

void PoolPatos::render()
{
	for (PatoGO* c : _myPool) {
		c->render();
	}
}


void PoolPatos::InitObject(PatoGO* p)
{
	//color y velocidad
	int type = Platform::Instance()->random() % 3;
	//escoge uno de los patos posibles segun el aleatorio
	int vel = _data[type];

	//direccion
	float dirX = getAngle();

	//posicion
	int init_x = Platform::Instance()->random() % (screen_width - (int)(p->GetH() * 1.25));
	//inicializamos ese pato con la informacion creada
	p->changeData(init_x, screen_height - (Game::ORIGINAL_TILE*Game::aspect*3), vel, dirX, spritesArray[type]);
	p->checkDirAnim();
}

//le pasamos como parametros el numero de patos que debe reutilizar para asi minimizar el coste del algoritmo de busqueda en tiempo
bool PoolPatos::ReUsePatos(int kills)
{
	int counter = 0;
	std::vector<PatoGO*>::iterator it;
	for (it = _myPool.begin(); counter < kills;)
	{
		PatoGO* p = (*it);
		if (p->isDead())
		{
			InitObject(p);
			++counter;
		}
		else {
			++it;
		}
	}

	//manejo de error
	if (counter == kills) return true;
	return false;
}

bool PoolPatos::loadSprites()
{
	spritesArray.resize(3);

	//Array estatico para cargar las imagenes. sigue este orden para facilitar la lectura:
	/*
	- primero leemos las seis imágenes que tienen 3x2 sprites
	- después leemos las imagenes que tienen 3x1 sprites
	- leemos las imágenes en el orden de: azul-negro-rojo, y las guardamos en el vector de sprites de la misma manera
	El orden de los sprites se utiliza de esta manera tambien para la definicion del enum SPRITE_INDEX de la clase Pato.h, que nos indicará cual de los
	grupos de ángulos se debe utilizar.
	*/
	std::string images[15] = { "assets/azul_0.rgba", "assets/negro_0.rgba", "assets/rojo_0.rgba",
							  "assets/azul_45.rgba", "assets/negro_45.rgba", "assets/rojo_45.rgba",
							  "assets/azul_90.rgba", "assets/negro_90.rgba", "assets/rojo_90.rgba",
							  "assets/azul_-90.rgba", "assets/negro_-90.rgba", "assets/rojo_-90.rgba",
							  "assets/azul_dado.rgba", "assets/negro_dado.rgba", "assets/rojo_dado.rgba",
	};

	for (int i = 0; i < 15; i++) {
		//leemos una imagen nueva
		Image* im = Decoder::decodeImage(images[i].c_str());
		if (!im)
			return false;
		loadedImages.push_back(im);

		//si es una de las seis primeras, tendrá 2 filas. Si no, tendrá una sola. Todas las imagenes contienen 3 columnas.
		int num_rows = (i < 6) ? 2 : 1;
		int num_cols;
		if (i < 9) 
			num_cols = 3;
		else if (i < 12) 
			num_cols = 2;
		else
			num_cols = 1;
		//por cada fila/columna, tenemos un nuevo sprite. Lo creamos y añadimos al array de imagenes correspondiente.
		for (int row = 0; row < num_rows; row++) {
			for (int col = 0; col < num_cols; col++) {
				const Sprite s = Sprite::fromRegularSpriteSheet(im, num_rows, num_cols, row, col);
				spritesArray[i % 3].push_back(s);
			}
		}
	}

	return true;
}

float PoolPatos::getAngle()
{
	int value = Platform::Instance()->random();
	float coord = (value % 60) / 100.0;
	if (value % 2 > 0) {
		coord = -coord;
	}

	return coord;
}

std::vector<PatoGO*> PoolPatos::GetPatos()
{
	return _myPool;
}

void PoolPatos::updatePhysics(float t)
{
	int kills = 0;
	float angle;
	for (PatoGO* _pato : _myPool)
	{
		//si tiene que realizar un rebote
		if (_pato->CompruebaRebote())
		{
			//comprueba la pared en la que ha rebotado
			PatoGO::PARED pared = _pato->getPared();

			if (pared == PatoGO::PARED::NONE)			//El pato debe ser eliminado
			{
				_pato->kill();
				kills++;
			}
			else {
				switch (pared)
				{
				case PatoGO::PARED::LEFT: {	//izquierda					
					angle = getAngle();
					if (!_pato->isCreated()) {
						angle = std::abs(angle) * -1;
					}
					_pato->changeDir(1 - std::abs(angle), angle);
					break;
				}
				case PatoGO::PARED::RIGHT: {	//derecha
					angle = getAngle();
					if (!_pato->isCreated()) {
						angle = std::abs(angle) * -1;
					}
					_pato->changeDir((-1 + std::abs(angle)), angle);
					break;
				}
				case PatoGO::PARED::TOP: {	//arriba
					angle = getAngle();
					_pato->changeDir(angle, (1 - std::abs(angle)));
					break;
				}
				case PatoGO::PARED::BOTTOM: {	//abajo
					angle = getAngle();
					_pato->changeDir(angle, (-1 + std::abs(angle)));
					break;
				}
				default:
					break;
				}
			}

		}
		//Actualizacion de la posicion del pato
		_pato->updatePhysics(t);

	}
	//Eliminacion de los patos apuntados en la lista de muerte
	if (kills > 0) {
		ReUsePatos(kills);
	}
}

void PoolPatos::update(float t)
{
	for (PatoGO* _pato : _myPool) {
		_pato->update(t);
	}
	render();
}

void PoolPatos::flush() {
	std::vector<PatoGO*>::iterator it;
	for (it = _myPool.begin(); it != _myPool.end();)
	{
		PatoGO* c = (*it);
		it = _myPool.erase(it);
		delete(c);
	}
}

