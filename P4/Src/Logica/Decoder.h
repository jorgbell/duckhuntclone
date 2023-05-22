#pragma once
#ifndef DECODER_H_
#define DECODER_H_

#include <ostream>
#include <Image.h>
class Decoder
{
public:
	static Image* decodeImage(const char* filename);
};

#endif // !DECODER_H_




