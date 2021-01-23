#include "hdr/Camera.h"

bool   Camera::firstMouse = true;
double Camera::lastX = WIDTH;  //TODO: Manejar width y height al agrandar o achicar ventana
double Camera::lastY = HEIGHT; //TODO: Manejar width y height al agrandar o achicar ventana
float  Camera::sensitivity = 0.1f;
glm::vec3 Camera::position = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 Camera::front    = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::up       = glm::vec3(0.0f, 1.0f, 0.0f);
double Camera::pitch = 0.0f;
double Camera::yaw   = -90.0f;
float  Camera::fov = 45.0f;

Camera::Camera(GLFWwindow * window)
{
	this->window = window;

	this->speed = 6.0f;

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

Camera::~Camera()
{
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float speed = this->speed * deltaTime;

	if (direction == FORWARD)
		position += speed * front;
	if (direction == BACKWARD)
		position -= speed * front;
	if (direction == LEFT)
		position -= glm::normalize(glm::cross(front, up)) * speed;
	if (direction == RIGHT)
		position += glm::normalize(glm::cross(front, up)) * speed;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}

void Camera::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	glm::vec3 direction;
	direction.x = (float)(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	direction.y = (float)(sin(glm::radians(pitch)));
	direction.z = (float)(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	front = glm::normalize(direction);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
