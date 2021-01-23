#include "hdr/VertexBuffer.h"

void VertexBuffer::SetAttribPointer(unsigned int attributeIndex, unsigned int vecX)
{
	glVertexAttribPointer(attributeIndex, vecX, GL_FLOAT, GL_FALSE, this->stride, (void*)(this->offSet * sizeof(float)));
	glEnableVertexAttribArray(attributeIndex);
	this->offSet += vecX;
}
