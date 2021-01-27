#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "hdr/Shader.h"
#include "hdr/ShaderProgram.h"
#include "hdr/VertexBuffer.h"
#include "hdr/VertexArray.h"
#include "hdr/ElementBuffer.h"
#include "hdr/Texture.h"
#include "hdr/Camera.h"

//#define WIREFRAME

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Camera camera, float deltaTime);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	Shader* lightVertexShader = new Shader("ShadersSources/vertex_light2.shader");
	Shader* lightFragmentShader = new Shader("ShadersSources/fragment_light2.shader");

	std::vector<Shader*> shaders = { lightVertexShader, lightFragmentShader };
	ShaderProgram* lightProgram = new ShaderProgram(shaders);

	Shader* lightedVertexShader = new Shader("ShadersSources/vertex_lighted2.shader");
	Shader* lightedFragmentShader = new Shader("ShadersSources/fragment_lighted2.shader");

	shaders = { lightedVertexShader, lightedFragmentShader };
	ShaderProgram* lightedProgram = new ShaderProgram(shaders);
	
	Texture container = Texture();
	container.AddTexture("Textures/container2.png", Texture::Channels::rgba, true);
	container.AddTexture("Textures/container2_specular.png", Texture::Channels::rgba, true);

	lightedProgram->Use();
	
	lightedProgram->SetUniform("material.diffuse", 0);
	lightedProgram->SetUniform("material.specular", 1);
	lightedProgram->SetUniform("material.shininess", 32.0f);

	lightedProgram->SetUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
	lightedProgram->SetUniform("dirLight.ambient",    0.2f, 0.2f, 0.2f);
	lightedProgram->SetUniform("dirLight.diffuse",    0.5f, 0.5f, 0.5f);
	lightedProgram->SetUniform("dirLight.specular",   1.0f, 1.0f, 1.0f);

	container.Active(0);
	container.Active(1);

	for (unsigned int i = 0; i < 4; i++)
	{
		std::string aux = "pointLight[" + std::to_string(i) + "]";
		lightedProgram->SetUniform(aux+".ambient", 0.2f, 0.2f, 0.2f);
		lightedProgram->SetUniform(aux+".diffuse", 0.5f, 0.5f, 0.5f);
		lightedProgram->SetUniform(aux+".specular", 1.0f, 1.0f, 1.0f);

		lightedProgram->SetUniform(aux+".constant",  1.0f);
		lightedProgram->SetUniform(aux+".linear",    0.09f);
		lightedProgram->SetUniform(aux+".quadratic", 0.032f);
	}

	lightedProgram->SetUniform("spotLight.ambient",  0.2f, 0.2f, 0.2f);
	lightedProgram->SetUniform("spotLight.diffuse",  0.5f, 0.5f, 0.5f);
	lightedProgram->SetUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);

	lightedProgram->SetUniform("spotLight.constant",  1.0f);
	lightedProgram->SetUniform("spotLight.linear",    0.09f);
	lightedProgram->SetUniform("spotLight.quadratic", 0.032f);

	float FL_cutOff = glm::cos(glm::radians(12.5f));
	float FL_outerCutOff = glm::cos(glm::radians(16.5f));
	lightedProgram->SetUniform("spotLight.cutOff", FL_cutOff);
	lightedProgram->SetUniform("spotLight.outerCutOff", FL_outerCutOff);


	lightProgram->Use();
	lightProgram->SetUniform("light.ambient",  0.2f, 0.2f, 0.2f);
	lightProgram->SetUniform("light.diffuse",  0.5f, 0.5f, 0.5f);
	lightProgram->SetUniform("light.specular", 1.0f, 1.0f, 1.0f);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float objVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float lightVertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f
	};

	unsigned int lightIndices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		7, 3, 0,
		0, 4, 7,
		6, 2, 1,
		1, 5, 6,
		0, 1, 5,
		5, 4, 0,
		3, 2, 6,
		6, 7, 3
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,   0.0f),
		glm::vec3(0.0f,  0.0f,  -5.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f,  -2.5f),
		glm::vec3(1.5f,  2.0f,  -2.5f),
		glm::vec3(1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 scaleFactor[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(2.4f, 0.4f, 2.5f),
		glm::vec3(1.7f, 2.0f, 2.5f),
		glm::vec3(0.5f, 1.3f, 0.5f),
		glm::vec3(1.5f, 1.0f, 0.5f),
		glm::vec3(1.5f, 0.2f, 1.5f),
		glm::vec3(1.3f, 1.0f, 1.5f)
	};


	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	VertexArray*   lightVAO = new VertexArray();
	ElementBuffer* lightVBO = new VertexBuffer(8);
	ElementBuffer* lightEBO = new ElementBuffer();

	lightVBO->BindData(GL_ARRAY_BUFFER, lightVertices, 24, Usage::setOnce_usedManyTimes);
	lightVBO->SetAttribPointer(0, 3);
	lightEBO->BindData(GL_ELEMENT_ARRAY_BUFFER, lightIndices, 36, Usage::setOnce_usedManyTimes);

	lightVAO->OnOff();
	delete(lightVBO);
	delete(lightEBO);

	VertexArray* objVAO = new VertexArray();
	ElementBuffer* VBO  = new VertexBuffer(36);

	VBO->BindData(GL_ARRAY_BUFFER, objVertices, 36*8, Usage::setOnce_usedManyTimes);
	VBO->SetAttribPointer(0, 3);
	VBO->SetAttribPointer(1, 3);
	VBO->SetAttribPointer(2, 2);

	objVAO->OnOff();
	delete(VBO);

	Camera camera = Camera(window);
	double time;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

	#ifdef WIREFRAME
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	#endif // WIREFRAME


	// render loop
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float r = 10.0f;


	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		processInput(window, camera, deltaTime);

		// render
		glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		lightedProgram->Use();

		lightedProgram->SetUniform("viewPos", camera.position.x, camera.position.y, camera.position.z);

		for (unsigned int i = 0; i < 4; i++)
		{
			std::string index = std::to_string(i);

			glm::vec3 aux = pointLightPositions[i] + glm::vec3(r*cos(time), 0, r*sin(time));

			lightedProgram->SetUniform("pointLight[" + index + "].position", aux.x, aux.y, aux.z);
		}
	
		lightedProgram->SetUniform("spotLight.position", camera.position.x, camera.position.y, camera.position.z);
		lightedProgram->SetUniform("spotLight.direction", camera.front.x, camera.front.y, camera.front.z);


		view = camera.GetViewMatrix();
		projection = camera.GetProjectionMatrix();

		lightedProgram->TransformCoords(view, "view");
		lightedProgram->TransformCoords(projection, "projection");

		objVAO->OnOff(1);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightedProgram->TransformCoords(model, "model");

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		objVAO->OnOff(0);



		lightProgram->Use();

		lightProgram->TransformCoords(view, "view");
		lightProgram->TransformCoords(projection, "projection");

		lightVAO->OnOff(1);
		for (unsigned int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i] + glm::vec3(r*cos(time), 0, r*sin(time)));
			model = glm::rotate(model, (float)time * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 1.0f));
			model = glm::scale(model, scaleFactor[2]);

			lightProgram->TransformCoords(model, "model");
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		lightVAO->OnOff(0);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	delete(objVAO);
	delete(lightVAO);
	delete(lightedProgram);
	delete(lightProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, Camera camera, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}