#pragma once
#include "engine/Window/Camera.h"
#include <GlfW/glfw3.h>

class Events {

public:

	Events(Camera* camera);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void processInput(GLFWwindow* window, float deltaTime);

private:

	Camera* camera;
	bool firstMouse = true;
	float lastX = 400.0f;
	float lastY = 300.0f;

};