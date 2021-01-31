#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <FileSystem>

#define STB_IMAGE_IMPLEMENTATION
#include <STB_IMAGE/stb_image.h>

#include "hdr/ShaderProgram.h"
#include "hdr/Model.h"
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

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	ShaderProgram ourShaderProgram = ShaderProgram("ShadersSources/1.model_loading.vs", "ShadersSources/1.model_loading.fs");
	ourShaderProgram.Use();
	ourShaderProgram.SetUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
	ourShaderProgram.SetUniform("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	ourShaderProgram.SetUniform("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	ourShaderProgram.SetUniform("dirLight.specular", 1.0f, 1.0f, 1.0f);

	ourShaderProgram.SetUniform("pointLight[0].position", 4.0f, 4.0f, 4.0f);
	ourShaderProgram.SetUniform("pointLight[0].ambient", 0.2f, 0.2f, 0.2f);
	ourShaderProgram.SetUniform("pointLight[0].diffuse", 0.5f, 0.5f, 0.5f);
	ourShaderProgram.SetUniform("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
	ourShaderProgram.SetUniform("pointLight[0].constant", 1.0f);
	ourShaderProgram.SetUniform("pointLight[0].linear", 0.09f);
	ourShaderProgram.SetUniform("pointLight[0].cuadratic", 0.032f);

	ourShaderProgram.SetUniform("spotLight.ambient", 0.2f, 0.2f, 0.2f);
	ourShaderProgram.SetUniform("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	ourShaderProgram.SetUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
	ourShaderProgram.SetUniform("spotLight.constant",  1.0f);
	ourShaderProgram.SetUniform("spotLight.linear",    0.09f);
	ourShaderProgram.SetUniform("spotLight.cuadratic", 0.032f);
	float FL_cutOff = glm::cos(glm::radians(12.5f));
	float FL_outerCutOff = glm::cos(glm::radians(16.5f));
	ourShaderProgram.SetUniform("spotLight.cutOff", FL_cutOff);
	ourShaderProgram.SetUniform("spotLight.outerCutOff", FL_outerCutOff);


	// load models
	Model ourModel = Model("resources/objects/backpack/backpack.obj");


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

	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		processInput(window, camera, deltaTime);

		// render
		glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ourShaderProgram.Use();
		ourShaderProgram.SetUniform("spotLight.position", camera.position.x, camera.position.y, camera.position.z);
		ourShaderProgram.SetUniform("spotLight.direction", camera.front.x, camera.front.y, camera.front.z);
		ourShaderProgram.SetUniform("viewPos", camera.position.x, camera.position.y, camera.position.z);

		// view/projection transformations
		projection = camera.GetProjectionMatrix();
		view = camera.GetViewMatrix();
		ourShaderProgram.TransformCoords(projection, "projection");
		ourShaderProgram.TransformCoords(view, "view");

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShaderProgram.TransformCoords(model, "model");
		ourModel.Draw(ourShaderProgram);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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