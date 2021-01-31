#pragma once
#define DEBUG
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


enum class Usage {
	setOnce_usedFewTimes = GL_DYNAMIC_DRAW,
	setOnce_usedManyTimes = GL_STATIC_DRAW,
	changesAndUsedManyTimes = GL_DYNAMIC_DRAW
};

#define WIDTH 400
#define HEIGHT 300