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
	// ------------------------------------
	// vertex shader
	Shader* vertexShader = new Shader("ShadersSources/vertex_1.shader");
	
	// fragment shader
	Shader* fragmentShader = new Shader("ShadersSources/fragment_1.shader");

	std::vector<Shader*> shaders = { vertexShader, fragmentShader };

	// link shaders
	ShaderProgram* shaderProgram = new ShaderProgram(shaders);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,			
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 //REPEAT 2
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		//REPEAT 0

	
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		 //repeat 6
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		//repeat 4

		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		//repeat 10
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
		//repeat 8

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		//repeat 14,
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		//repeat 12,

	
		//repeat 10
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		//repeat 17
		//repeat 11,
		//repeat 10
	
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
		//repeat 13,
		//repeat 12
		//repeat 12
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
		//repeat 18
	};

	unsigned int indices[] = {  
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		10, 16, 17,
		17, 11, 10,
		18, 13, 12,
		12, 19, 18
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 scaleFactor[] = {
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(2.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 3.2f, 2.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(2.4f, 0.4f, 2.5f),
		glm::vec3(1.7f, 2.0f, 2.5f),
		glm::vec3(0.5f, 1.3f, 0.5f),
		glm::vec3(1.5f, 1.0f, 0.5f),
		glm::vec3(1.5f, 0.2f, 1.5f),
		glm::vec3(1.3f, 1.0f, 1.5f)
	};

	VertexArray*   VAO = new VertexArray();
	ElementBuffer* VBO = new VertexBuffer(20);
	ElementBuffer* EBO = new ElementBuffer();

	VBO->BindData(GL_ARRAY_BUFFER, vertices, 160, Usage::setOnce_usedManyTimes);
	VBO->SetAttribPointer(0, 3);
	VBO->SetAttribPointer(1, 3);
	VBO->SetAttribPointer(2, 2);

	EBO->BindData(GL_ELEMENT_ARRAY_BUFFER, indices, 36, Usage::setOnce_usedManyTimes);

	VAO->OnOff();
	
	delete(VBO);
	delete(EBO);
	
	Texture texture = Texture();
	texture.AddTexture("Textures/GL.jpg", GL_RGB, true);
	texture.AddTexture("Textures/saitama.png", GL_RGBA, true);

	shaderProgram->Use();
	shaderProgram->SetUniform("texture1");
	shaderProgram->SetUniform("texture2");

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

		texture.Active(0);
		texture.Active(1);

		// draw our first triangle
		shaderProgram->Use();
		VAO->OnOff(1);

		// update the uniform color

		float redValue   = cos(time) / 2.0f + 0.5;
		float greenValue = sin(time) / 2.0f + 0.5f;
		float blueValue  = 1 / (1 + exp(-tan(time)));

		float ourColor[] = { redValue, greenValue, 0.0f, 1.0f };
		shaderProgram->SetUniform("ourColor_U", ourColor);


		view       = camera.GetViewMatrix();
		projection = camera.GetProjectionMatrix();

		shaderProgram->TransformCoords(view, "view");
		shaderProgram->TransformCoords(projection, "projection");

		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);			

			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)time * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, scaleFactor[i]);
			shaderProgram->TransformCoords(model, "model");

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	delete(VAO);
	delete(shaderProgram);

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