#include "hdr/Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const char *filePath)
{
	std::ifstream stream(filePath);

	std::string line;
	std::stringstream shaderSource;
	GLenum type = 1;

	#ifdef DEBUG
		std::string sType = "";
	#endif // DEBUG

	//Sacamos datos importantes del shader source y tipo shader
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = GL_VERTEX_SHADER;

				#ifdef DEBUG
					sType = "Vertex";
				#endif // DEBUG
				
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = GL_FRAGMENT_SHADER;

				#ifdef DEBUG
					sType = "Fragment";
				#endif // DEBUG
			}
				
		}
		else
			shaderSource << line << '\n';
	}
	
	//Creamon un Shader y guardamos referencia en id
	this->id = glCreateShader(type);
	#ifdef DEBUG
		std::cout << "\t[DEBUG] "<< sType <<" Shader with id: " << this->id << " CREATED\n";
	#endif // DEBUG
	
	//Adjuntamos los datos necesarios al shader recien creado
	std::string src = shaderSource.str();
	const char* src2 = src.c_str();
	glShaderSource(this->id, 1, &src2, nullptr);

	//Compilamos y revisamos estado del shader
	glCompileShader(this->id);
	this->CheckCompilation(type);
}


Shader::~Shader()
{
	#ifdef DEBUG
		std::cout << "\t\t[DEBUG] Shader with id: " << this->id << " DELETED\n";
	#endif // DEBUG

	glDeleteShader(this->id);
}


void Shader::CheckCompilation(GLenum type)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "[ERROR] SHADER TYPE " << type << " COMPILATION FAILED\n" << infoLog << "\n DELETING SHADER\n";

		this->~Shader();
	}
}