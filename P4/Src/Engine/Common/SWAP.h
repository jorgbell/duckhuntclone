#pragma once
#ifndef SWAP_H_
#define SWAP_H_
//macro para hacer swap
#define SWAP32(value) (((value) & 0xFF000000) >> 24) | (((value) & 0x00FF0000) >> 8) | (((value) & 0x0000FF00) << 8) | (((value) & 0x000000FF) << 24)
#define SWAP16(value) (value >> 8) | (value << 8);

#endif // !SWAP_H_
