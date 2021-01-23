#include "hdr/ElementBuffer.h"


ElementBuffer::ElementBuffer()
{
	glGenBuffers(1, &(this->id));
	#ifdef DEBUG
		//Imposible hacer que printee VertexBuffer desde constructor o destructor
		std::cout << "\t[DEBUG] " << typeid(*this).name() << " Object CREATED \n"; 
	#endif // DEBUG
}

ElementBuffer::ElementBuffer(unsigned int numOfVec)
{
	//ElementBuffer(); HACER ESTO NO SIRVE xD
	glGenBuffers(1, &(this->id));
	#ifdef DEBUG
		//Imposible hacer que printee VertexBuffer desde constructor o destructor
		std::cout << "\t[DEBUG] " << typeid(*this).name() << " Object CREATED \n";
	#endif // DEBUG	

	this->numOfVec = numOfVec;
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &(this->id));

	#ifdef DEBUG
		std::cout << "\t\t[DEBUG] "<< typeid(*this).name() <<" Object DELETED \n";
	#endif // DEBUG
}

template <typename T>
void ElementBuffer::BindData(unsigned int target, T* data, unsigned int dataLarge, Usage usage)
{
	glBindBuffer(target, this->id);
	glBufferData(target, sizeof(T)*dataLarge, data, (GLenum)usage);
	
	if (this->numOfVec != 0) 
		this->stride = (sizeof(T) * dataLarge) / this->numOfVec;

	#ifdef DEBUG
		std::cout << "\t\t[DEBUG] " << typeid(*this).name() << " Object DATA BINDED \n";
	#endif // DEBUG
}