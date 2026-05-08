#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string>
#include "Camera.h"
#include "InputHandler.h"
#include "RenderObject.h"

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();

    void run();

    InputHandler& getHandler() { return m_input; }

private:
    // Initialization
    void initWindow(int width, int height, const char* title);
    void initScene();

    // Loop steps
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void render();
    void updatePerformanceCounter();

    // Window data
    GLFWwindow* m_window;
    int m_width, m_height;

    // Systems
    Camera m_camera;
    InputHandler m_input;

    // Resources (Shaders, Meshes, etc)
    std::unique_ptr<Shader> m_objectShader;
    std::unique_ptr<Shader> m_lightShader;
    std::unique_ptr<Mesh> m_cubeMesh;
    std::unique_ptr<Texture> m_diffuseMap;
    std::unique_ptr<Texture> m_specularMap;

    std::vector<RenderObject> m_sceneObjects;

    // Timing
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;
    double m_lastTime = 0.0;
    int m_nbFrames = 0;
};
