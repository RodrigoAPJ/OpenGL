#include "hdr/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &(this->id));
	this->OnOff(1);
}

VertexArray::~VertexArray()
{
	this->OnOff();
	glDeleteVertexArrays(1, &(this->id));
}

void VertexArray::OnOff(int unbindFlag)
{
	if(unbindFlag == 0)
		glBindVertexArray(0);
	else 
		glBindVertexArray(this->id);
}
