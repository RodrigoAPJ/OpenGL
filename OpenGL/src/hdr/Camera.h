#pragma once
#include "commons.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
	static bool   firstMouse;
	static double lastX;
	static double lastY;
	static float sensitivity;
	float speed;
	GLFWwindow* window;

public:
	static enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	static glm::vec3 position;
	static glm::vec3 front;
	static glm::vec3 up;

	static double pitch;
	static double yaw;
	
	static float fov;

	Camera(GLFWwindow* window);
	~Camera();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(); //TODO: Manejar width y height al agrandar o achicar ventana

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};


