#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "commons.h"

class ShaderProgram
{
private:
	unsigned int id;
	void checkLink();
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();

	unsigned int GetId() { return this->id; }
	void Use();
	template <typename T>
	void SetUniform(const std::string &name, T* value, unsigned int arrLength);
	template <typename T>
	void SetUniform(const std::string &name, T value0, T value1, T value2);
	template <typename T>
	void SetUniform(const std::string &name, T value);

	void TransformCoords(glm::mat4 transform, const char* name);
};

template void ShaderProgram::SetUniform<bool>(const std::string &name, bool* value, unsigned int arrLength);
template void ShaderProgram::SetUniform<int>(const std::string &name, int* value, unsigned int arrLength);
template void ShaderProgram::SetUniform<float>(const std::string &name, float* value, unsigned int arrLength);

template void ShaderProgram::SetUniform<int>(const std::string &name, int value0, int value1, int value2);
template void ShaderProgram::SetUniform<float>(const std::string &name, float value0, float value1, float value2);

template void ShaderProgram::SetUniform(const std::string &name, int value);
template void ShaderProgram::SetUniform(const std::string &name, unsigned int value);
template void ShaderProgram::SetUniform(const std::string &name, float value);