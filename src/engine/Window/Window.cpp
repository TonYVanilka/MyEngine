#include "engine/Window/Window.h"

#include <iostream>

Window::Window(int SCR_WIDTH, int SCR_HEIGHT, const char* title) : camera(), events(&camera)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL); // <-- fix here
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(window, &events);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) {
        auto* e = static_cast<Events*>(glfwGetWindowUserPointer(win));
        if (e) e->framebuffer_size_callback(win, w, h);
        });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        auto* e = static_cast<Events*>(glfwGetWindowUserPointer(win));
        if (e) e->mouse_callback(win, x, y);
        });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double x, double y) {
        auto* e = static_cast<Events*>(glfwGetWindowUserPointer(win));
        if (e) e->scroll_callback(win, x, y);
        });
}

Window::~Window()
{
    glfwTerminate();
	std::cout << "Window destroyed" << std::endl;
}

void Window::ProcessEvents()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwSwapBuffers(window);
    glfwPollEvents();

    events.processInput(window, deltaTime);
}

GLFWwindow* Window::GetWindow() const
{
    return window;
}

glm::mat4 Window::GetViewMatrix() const {
    return camera.GetViewMatrix();
}

Camera& Window::GetCamera() {
    return camera;
}
