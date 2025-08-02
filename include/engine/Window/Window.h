#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/Window/Camera.h"
#include "engine/Window/Events.h"

class Window {

public:

	Window(int SCR_WIDTH, int SCR_HEIGHT, const char* title);
	~Window();

	void ProcessEvents();

	GLFWwindow* GetWindow() const;

	glm::mat4 GetViewMatrix() const;
	Camera& GetCamera();

private:

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	GLFWwindow* window;
	Camera camera;
	Events events;

};