#pragma once

#ifndef ENDIANESS_H_
#define ENDIANESS_H_

#include <SDL_endian.h>
#if  SDL_BYTEORDER == SDL_BIG_ENDIAN
#define BIG_ENDIAN
#else
#define LITTLE_ENDIAN
#endif
#endif