#pragma once
#ifndef INPUTINFO_H_
#define INPUTINFO_H_
/*
Struct compartido por ambas instancias del Engine. Se trata del mapeo del mando de PS4 que se usará posteriormente en la lógica para
observar el input recogido.
*/

struct InputInfo {
	float gyroX;
	float gyroY;
	float gyroZ;
	float leftStickX;
	float leftStickY;
	bool cross;
	bool circle;
	float l2;
	float r2;
	InputInfo() : gyroX(0.0f), gyroY(0.0f), gyroZ(0.0f), leftStickX(0.0f), leftStickY(0.0f), cross(false), circle(false), l2(0.0f), r2(0.0f) {}
};
#endif // !INPUTINFO_H_
