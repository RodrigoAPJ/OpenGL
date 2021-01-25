#include "hdr/ShaderProgram.h"
#include <iostream>

void ShaderProgram::checkLink()
{
	int  success;
	char infoLog[512];

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "[ERROR] SHADER PROGRAM LINKING_FAILED\n" << infoLog << "\n DELETING PROGRAM SHADER\n";
		
		delete(this);
	}

	std::cout << "\t\t[DEBUG] Shader program SUCCESFULY LINKED\n";
}

ShaderProgram::ShaderProgram(std::vector<Shader*>& shaders)
{
	// Creamos un Shader Program y guardamos su id
	this->id = glCreateProgram();
	#ifdef DEBUG
		std::cout << "\t[DEBUG] Shader Program CREATED \n";
	#endif // DEBUG

	// Adjuntamos todas las shaders EN ORDEN al program shader
	for (int i = 0; i < shaders.size(); i++)
	{
		Shader* aux = shaders.at(i);
		glAttachShader(this->id, aux->GetId());
	}
		

	// Linkeamos todas las shaders en un único Shader y buscamos errores
	glLinkProgram(this->id);
	this->checkLink();
	
	// Eliminamos shaders que ya no usaremos
	for (int i = 0; i < shaders.size();)
	{
		Shader* aux = shaders.at(i);
		shaders.erase(shaders.begin());

		delete(aux);
	}

}

ShaderProgram::~ShaderProgram()
{
#ifdef DEBUG
	std::cout << "\t\t[DEBUG] Shader Program with id: " << this->id << " DELETED\n";
#endif // DEBUG

	glDeleteProgram(this->id);
}

void ShaderProgram::Use()
{
	glUseProgram(this->id);
}

template <typename T>
void ShaderProgram::SetUniform(const std::string & name, T value)
{
	if (typeid(value) == typeid(float))
		glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
	else if (typeid(value) == typeid(int))
		glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
	else
	#ifdef DEBUG
		std::cout << "[ERROR] AT SET " << name.c_str() << " UNIFORM (not valid type)\n";
	#endif // DEBUG

}

void ShaderProgram::TransformCoords(glm::mat4 transform, const char* name)
{
	int transformLoc = glGetUniformLocation(this->id, name);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

}

template <typename T>
void ShaderProgram::SetUniform(const std::string &name, T* value, unsigned int arrLength)
{
	if(arrLength == 4)
		glUniform4f(glGetUniformLocation(this->id, name.c_str()), value[0], value[1], value[2], value[3]);
	else if(arrLength == 3)
		glUniform3f(glGetUniformLocation(this->id, name.c_str()), value[0], value[1], value[2]);
	else
	#ifdef DEBUG
			std::cout << "\t[DEBUG] UNIFORM " << name.c_str() << " error\n";
	#endif // DEBUG
}

template<typename T>
void ShaderProgram::SetUniform(const std::string & name, T value0, T value1, T value2)
{
	glUniform3f(glGetUniformLocation(this->id, name.c_str()), value0, value1, value2);
}
