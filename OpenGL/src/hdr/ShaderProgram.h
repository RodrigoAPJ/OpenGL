#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "commons.h"

class ShaderProgram
{
private:
	unsigned int id;
	unsigned int height = 0;
	void checkLink();
public:
	ShaderProgram(std::vector<Shader*>& shaders);
	~ShaderProgram();

	unsigned int GetId() { return this->id; }
	void Use();
	template <typename T>
	void SetUniform(const std::string &name, T* value);
	void SetUniform(const std::string &name);

	void TransformCoords(glm::mat4 transform, const char* name);
};

template void ShaderProgram::SetUniform<bool>(const std::string &name, bool* value);
template void ShaderProgram::SetUniform<int>(const std::string &name, int* value);
template void ShaderProgram::SetUniform<float>(const std::string &name, float* value);