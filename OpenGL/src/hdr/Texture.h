#pragma once
#include <string>

#include "commons.h"

class Texture
{
private:
	unsigned int ids[16];
	unsigned int newPos;
	unsigned int getUnit(unsigned int unit);
public:
	Texture();
	~Texture();

	void AddTexture(const std::string path, unsigned int channel, bool flip);
	void Active(unsigned int id);
};

