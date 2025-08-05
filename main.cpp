#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <thread>

#include "engine/ShaderProgram.h"
//#include "engine/Mesh.h"
#include "engine/Vertex.h"
#include "engine/Window/Window.h"
#include "engine/Texture.h"
#include "engine/Voxels/ChunkManager.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main() {

   /* for (uint32_t i = 0; i < vertices.size(); ++i) {
        indices.push_back(i);
    }*/

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);

    // build and compile our shader program
    Window win(SCR_WIDTH, SCR_HEIGHT, "Engine");
    GLFWwindow* window = win.GetWindow();

    Shader ourShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    //create texture

	Texture texture1("assets/textures/atlas.png", true, 0);

    ourShader.use();

    ourShader.setInt("texture", 0);

	ChunkManager chunkManager;

	chunkManager.GenerateTerrains();
	chunkManager.GenerateMeshes();

    glm::vec3 lightDirection = glm::normalize(glm::vec3(0.5f, -1.0f, 0.5f)); // Пример: свет под углом
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // Белый свет

    while (!glfwWindowShouldClose(window))
    {

        //Camera
        glm::mat4 view = win.GetViewMatrix();
        ourShader.setMat4("view", view);

		chunkManager.RenderChunks(ourShader);

		//chunk.Render();
		//model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        ourShader.setVec3("lightDir", lightDirection);
        ourShader.setVec3("lightColor", lightColor);
        
        win.ProcessEvents();
    }

    return 0;

}