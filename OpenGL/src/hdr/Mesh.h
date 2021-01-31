#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

struct sVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct sTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
	public:
		// mesh data
		std::vector<sVertex>       vertices;
		std::vector<unsigned int>  indices;
		std::vector<sTexture>      textures;

		Mesh(std::vector<sVertex> vertices, std::vector<unsigned int> indices, std::vector<sTexture> textures);
		void Draw(ShaderProgram &shaderProgram);
	private:
		//  render data
		unsigned int VAO, VBO, EBO;
		void SetupMesh();
};

