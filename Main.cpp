#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Utilities.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "vec.h"
#include "mat4.h"
#include "quat.h"



// Resizes window to specified dimensions
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width1, height);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    InputHandler* input = static_cast<InputHandler*>(glfwGetWindowUserPointer(window));
    input->handleMouse(xpos, ypos);
}



int main()
{
    // Initialise GLFW and GLAD and create window
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "NEA", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    constexpr int WIDTH = 800, HEIGHT = 600;

    glViewport(0, 0, WIDTH, HEIGHT);

    // Handles window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    
    // Define mesh data
    Vertex vertices[] = {
        // Front face
        { vec3{-0.5f, -0.5f,  0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f,  0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f,  0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f,  0.5f,  0.5f}, vec2{0.0f, 1.0f} },

        // Back face
        { vec3{ 0.5f, -0.5f, -0.5f}, vec2{0.0f, 0.0f} },
        { vec3{-0.5f, -0.5f, -0.5f}, vec2{1.0f, 0.0f} },
        { vec3{-0.5f,  0.5f, -0.5f}, vec2{1.0f, 1.0f} },
        { vec3{ 0.5f,  0.5f, -0.5f}, vec2{0.0f, 1.0f} },

        // Left face
        { vec3{-0.5f, -0.5f, -0.5f}, vec2{0.0f, 0.0f} },
        { vec3{-0.5f, -0.5f,  0.5f}, vec2{1.0f, 0.0f} },
        { vec3{-0.5f,  0.5f,  0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f,  0.5f, -0.5f}, vec2{0.0f, 1.0f} },

        // Right face
        { vec3{ 0.5f, -0.5f,  0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f, -0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f, -0.5f}, vec2{1.0f, 1.0f} },
        { vec3{ 0.5f,  0.5f,  0.5f}, vec2{0.0f, 1.0f} },

        // Top face
        { vec3{-0.5f,  0.5f,  0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f,  0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f,  0.5f, -0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f,  0.5f, -0.5f}, vec2{0.0f, 1.0f} },

        // Bottom face
        { vec3{-0.5f, -0.5f, -0.5f}, vec2{0.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f, -0.5f}, vec2{1.0f, 0.0f} },
        { vec3{ 0.5f, -0.5f,  0.5f}, vec2{1.0f, 1.0f} },
        { vec3{-0.5f, -0.5f,  0.5f}, vec2{0.0f, 1.0f} }
    };

    unsigned int indices[] = {
        // Front
        0, 1, 2,
        0, 2, 3,

        // Back
        4, 5, 6,
        4, 6, 7,

        // Left
        8, 9, 10,
        8, 10, 11,

        // Right
        12, 13, 14,
        12, 14, 15,

        // Top
        16, 17, 18,
        16, 18, 19,

        // Bottom
        20, 21, 22,
        20, 22, 23
    };

    // Initialse objects and variables
    Mesh mesh = Mesh(vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
    Shader shader = Shader("shader.vs", "shader.fs");
    Texture cubeTex("grunge-wall-texture.jpg");

    Camera camera;
    InputHandler input(&camera);
    glfwSetWindowUserPointer(window, &input);
    glfwSetCursorPosCallback(window, mouse_callback);
   
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float angle = 0.0f;

    float rClear = 37.0f / 255.0f;
    float gClear = 41.0f / 255.0f;
    float bClear = 48.0f / 255.0f;



    // Render loop
    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(rClear, gClear, bClear, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        input.handleKeyboard(window, deltaTime);

        // Create MVP matrices
        angle += fmodf(deltaTime * 100, 360.0f);
        quat rotation = quat::fromAxisAngle(vec3{ 1.0f, 0.0f, 0.0f }, radians(angle));
        mat4 model = mat4::scale(1.0f, 1.0f, 1.0f) * mat4::rotate(rotation) * mat4::translate(0.0f, 0.0f, 0.0f);

        mat4 view = mat4::lookAt(camera.position, camera.position + camera.front(), camera.up());

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (float)width / (float)height;
        mat4 projection = mat4::perspective(aspect, radians(45.0f), 0.1f, 100.0f);

        // Set uniforms
        shader.use();

        shader.setMat4("uModel", model);
        shader.setMat4("uView", view);
        shader.setMat4("uProjection", projection);

        cubeTex.bind(0);
        shader.setInt("uTexture", 0);

        mesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}
