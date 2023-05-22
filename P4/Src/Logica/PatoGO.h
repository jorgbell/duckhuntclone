#pragma once
#include "GameObject.h"

#define LIFETIME 4000

class PatoGO :
    public GameObject
{
public: 
    //Enum que nos indicará en qué pared ha rebotado el pato
    enum PARED { LEFT, RIGHT, TOP, BOTTOM, NONE };
    //Enum que nos indicará que fila de sprites estamos usando, 
    //que luego junto a la variable loop usamos para saber cual de todos los sprites de la fila se necesitan.
    enum SPRITE_INDEX { HORIZONTAL_RIGHT, HORIZONTAL_LEFT, UP_RIGHT, UP_LEFT, UP, FALLING};

    PatoGO();
    ~PatoGO();

    //Actualiza el tiempo restante para 'morir' y el estado de la animacion (loop)
    virtual void update(float t);
    //Actualiza la posicion del pato en funcion de su velocidad y el tiempo
    virtual void updatePhysics(float t);
    /*comprueba con que pared esta rebotando un pato o
     *si un pato ya ha salido totalmente de la pantalla y debe morir*/
    bool CompruebaRebote();
    //cambia los datos del pato a la hora de reiniciarlo (uso en la pool)
    void changeData(float x_, float y_, int vel, float dirX, std::vector<Sprite> duckSprites);
    //Cambia la direccion de movimiento
    void changeDir(float dX, float dY);
    //Checkea en que sentido se está moviendo y selecciona el grupo de animaciones correspondiente
    void checkDirAnim();
    //cambia el flag para eliminar de la pool
    void kill();

    /// <summary>
    /// LIMPIAR ESTOS PUTOS FLAGS
    /// </summary>
    void shot();
    void falling();
    //Getters-Setters
    PARED getPared();
    bool isDead();
    bool isShot();
    bool isFalling();
    bool isCreated();
    
    bool checkShoot(float cX, float cY);

private:
    //velocidad pixeles/segundo
    int _vel;
    //direccion de movimiento (angulos)
    float _dirX, _dirY;
    //variables de control:
    /*
        bounce nos indica si el pato aun debe rebotar o si en su defecto, ha acabado el temporizador y debe salirse de la pantalla
        dead nos indica si debe eliminarse de la pool o aun no
    */
    bool _bounce, _dead;
    //pared con la que está rebotando. Orden definido en el enum PARED.
    PARED p = NONE;

    //LIMPIAR ESTOS FLAGS
    bool _shot = false, _falling = false, _isOutOfBush = false;

    //timer
    int lifetime = LIFETIME;
    float lastChange = 0;
    float shotTime = 0;
    int anim_change = 333; //3 veces por segundo
    int falling_change = 85;
    int airTime = 1000;

    //Anim
    // index el "grupo" de tres sprites de una misma "animacion" (va de 3 en 3)
    // paso del loop en el que estamos
    SPRITE_INDEX movementIndex = HORIZONTAL_RIGHT; int _loop = 0;

};

