#include "Decoder.h"
#include <Platform.h>
#include <endianess.h>
#include <SWAP.h>
#include <iostream>
#include <vector>

/*
En este método se reserva la memoria a la hora de leer la imagen (los píxeles), y serán las propias imagenes (Clase Image) las que liberarán
dicha memoria cuando se cierre el programa.
*/
Image* Decoder::decodeImage(const char* filename)
{
	FILE* f = Platform::Instance()->openFile(filename);
	if (!f) {
		return nullptr;
	}

	uint32_t width;
	uint32_t height;

	//4 bytes de Width y 4 de Height
	size_t err;
	if (ferror(f) != 0)
	{
		std::cerr << "ERROR: Archivo vacío " << ferror(f) << std::endl;
		return nullptr;
	}
	err = fread(&width, sizeof(uint32_t), 1, f);
	if (err < 1) {
		std::cerr << "ERROR de lectura al leer el ancho " << ferror(f) << std::endl;
		return nullptr;
	}
	err = fread(&height, sizeof(uint32_t), 1, f);
	if (err < 1) {
		std::cerr << "ERROR de lectura al leer el alto " << ferror(f) << std::endl;
		return nullptr;
	}

#ifdef LITTLE_ENDIAN
	width = SWAP32(width);
	height = SWAP32(height);

#endif // LITTLE_ENDIAN
	int n = width * height; //7680 pixeles de 4 bytes

	//reservar memoria
	uint32_t* pixels = (uint32_t*)malloc(n * sizeof(uint32_t));
	//std::vector<uint32_t> v(n,1);
#ifdef LITTLE_ENDIAN
	uint32_t tempPixel;
	for (int i = 0; i < n; ++i) {
		err = fread(&tempPixel, sizeof(uint32_t), 1, f);
		if (err < 1) {
			std::cerr << "ERROR de lectura al leer los pixeles de la imagen " << ferror(f) << std::endl;
			return nullptr;
		}
		tempPixel = SWAP32(tempPixel);
		pixels[i] = tempPixel;
	}
#else //BIG_ENDIAN
	err = fread(&pixels, sizeof(uint32_t), n, f);
	if (err < n) {
		std::cerr << "ERROR de lectura al leer los pixeles de la imagen " << ferror(f) << std::endl;
		return nullptr;
	}
#endif // LITTLE_ENDIAN
	fclose(f);
	Image* i = Image::buildImage(pixels, width, height);
	if (i == nullptr) {
		std::cerr << "ERROR en la creacion de la imagen " << ferror(f) << std::endl;
		return nullptr;
	}
	return i;
}
