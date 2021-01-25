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

	enum Channels {
		rgb = GL_RGB,
		rgba = GL_RGBA
	};

	void AddTexture(const std::string path, unsigned int channel, bool flip);
	void Active(unsigned int id);
};

