#pragma once
#include "commons.h"

class Shader
{
private:
	unsigned int id;
	void CheckCompilation(GLenum type);

public:
	Shader(const char *filePath);
	~Shader();

	unsigned int GetId() { return this->id; }
};