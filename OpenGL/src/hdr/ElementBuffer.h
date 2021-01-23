#pragma once
#include "commons.h"

class ElementBuffer
{
protected:
	unsigned int id;
	unsigned int stride;
	unsigned int numOfVec = 0;
public:
	ElementBuffer();
	ElementBuffer(unsigned int numOfVec);
	~ElementBuffer();
	template <typename T>
	void BindData(unsigned int target, T* data, unsigned int dataLarge, Usage usage);
	virtual void SetAttribPointer(unsigned int attributeIndex, unsigned int vecX) {};
};

template void ElementBuffer::BindData<unsigned int>(unsigned int target, unsigned int* data, unsigned int dataLarge, Usage usage);
template void ElementBuffer::BindData<float>(unsigned int target, float* data, unsigned int dataLarge, Usage usage);
