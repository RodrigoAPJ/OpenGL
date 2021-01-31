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

ShaderProgram::ShaderProgram(const char * vertexPath, const char * fragmentPath)
{
	// Creamos un Shader Program y guardamos su id
	id = glCreateProgram();
	#ifdef DEBUG
		std::cout << "\t[DEBUG] Shader Program CREATED \n";
	#endif // DEBUG

	// Adjuntamos todas las shaders EN ORDEN al program shader	
	Shader vertexShader   = Shader(vertexPath);
	Shader fragmentShader = Shader(fragmentPath);

	glAttachShader(this->id, vertexShader.GetId());
	glAttachShader(this->id, fragmentShader.GetId());

	// Linkeamos todas las shaders en un único Shader y buscamos errores
	glLinkProgram(this->id);
	this->checkLink();
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
	else if (typeid(value) == typeid(int) || typeid(value) == typeid(unsigned int))
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
