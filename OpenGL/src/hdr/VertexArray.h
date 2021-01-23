#pragma once
#include "commons.h"
class VertexArray
{
private:
	unsigned int id;
public:
	VertexArray();
	~VertexArray();

	void OnOff(int unbindFlag = 0);
};

