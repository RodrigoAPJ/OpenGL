#pragma once
#include "commons.h"
#include "ElementBuffer.h"

class VertexBuffer : public ElementBuffer
{
private:
	unsigned int offSet = 0;
public:
	
	//Receives the number of vectors inside one vertex
	VertexBuffer(unsigned int numOfVec) : ElementBuffer(numOfVec)
	{
		#ifdef DEBUG
			std::cout << "\t\t[DEBUG] Previous Object was a " << typeid(*this).name() << "\n";
		#endif // DEBUG
	};

	void SetAttribPointer(unsigned int attributeIndex, unsigned int vecX);
};

