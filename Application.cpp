#include <iostream>
#include "Application.h"
#include "Primitives.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->getHandler().handleMouse(xpos, ypos);
}

Application::Application(int width, int height, const char* title)
    : m_width(width), m_height(height), m_input(m_camera)
{
    initWindow(width, height, title);
    initScene();
}

Application::~Application() {
    glfwTerminate();
}

void Application::initWindow(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
}

void Application::initScene() {
    // Initialize Resources
    m_cubeMesh = std::make_unique<Mesh>(Primitives::cubeVertices, Primitives::cubeIndices);
    m_diffuseMap = std::make_unique<Texture>("wall-7-granite-DIFFUSE.jpg");
    m_specularMap = std::make_unique<Texture>("wall-7-granite-DIFFUSE.jpg");
    m_objectShader = std::make_unique<Shader>("shader.vs", "objectShader.fs");
    m_lightShader = std::make_unique<Shader>("shader.vs", "lightShader.fs");

    // Add Light
    m_sceneObjects.push_back({
        m_cubeMesh.get(),
        m_diffuseMap.get(),
        m_specularMap.get(),
        m_lightShader.get(),

        vec3{-1.0f, 2.5f, -2.0f},
        quat::fromAxisAngle(vec3{0.0f, 1.0f, 0.0f}, radians(45.0f)),
        vec3{0.5f, 0.5f, 0.5f},
        vec3{1.0f, 1.0f, 1.0f}
        });

    // Add Cubes
    m_sceneObjects.push_back({
        m_cubeMesh.get(),
        m_diffuseMap.get(),
        m_specularMap.get(),
        m_objectShader.get(),

        vec3{0.0f, 0.0f, 0.0f},
        quat::fromAxisAngle(vec3{1.0f, 0.0f, 0.0f}, 0.0f),
        vec3{1.0f, 1.0f, 1.0f},
        vec3{1.0f, 0.5f, 0.31f}
        });

    m_sceneObjects.push_back({
        m_cubeMesh.get(),
        m_diffuseMap.get(),
        m_specularMap.get(),
        m_objectShader.get(),

        vec3{-3.0f, 1.0f, -2.0f},
        quat::fromAxisAngle(vec3{0.0f, 0.0f, 1.0f}, 0.0f),
        vec3{1.5f, 1.5f, 1.5f},
        vec3{32.0f / 255.0f, 127.0f / 255.0f, 186.0f / 255.0f}
        });
}

void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        handleInput(m_deltaTime);
        update(m_deltaTime);
        render();
        updatePerformanceCounter();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::handleInput(float deltaTime) {
    m_input.handleKeyboard(m_window, deltaTime);
}

void Application::update(float deltaTime) {
    
}

void Application::render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 view = mat4::lookAt(m_camera.position, m_camera.position + m_camera.front(), m_camera.up());

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    mat4 projection = mat4::perspective((float)w / (float)h, radians(45.0f), 0.1f, 100.0f);

    vec3 lightPos = m_sceneObjects[0].position;

    for (auto& obj : m_sceneObjects) {
        obj.shader->use();
        obj.shader->setMat4("uModel", obj.getModelMatrix());
        obj.shader->setMat4("uView", view);
        obj.shader->setMat4("uProjection", projection);
        
        if (obj.shader == m_objectShader.get()) {
            obj.shader->setInt("uMaterial.diffuse", 0);
            obj.diffuseMap->bind(0);
            obj.shader->setInt("uMaterial.specular", 1);
            obj.specularMap->bind(1);

            obj.shader->setVec3("uLight.position", lightPos);
            obj.shader->setVec3("uLight.ambient", vec3{ 0.2f, 0.2f, 0.2f });
            obj.shader->setVec3("uLight.diffuse", vec3{ 0.5f, 0.5f, 0.5f });
            obj.shader->setVec3("uLight.specular", vec3{ 1.0f, 1.0f, 1.0f });

            obj.shader->setFloat("uLight.constant", 1.0f);
            obj.shader->setFloat("uLight.linear", 0.09f);
            obj.shader->setFloat("uLight.quadratic", 0.032f);

            obj.shader->setVec3("uCameraPos", m_camera.position);
        }
        else if (obj.shader == m_lightShader.get()) {
            obj.shader->setVec3("uObjectColor", obj.color);
        }

        obj.mesh->draw();
    }
}

void Application::updatePerformanceCounter() {
    double currentTime = glfwGetTime();
    m_nbFrames++;
    if (currentTime - m_lastTime >= 1.0) {
        std::string title = "NEA | " + std::to_string(1000.0 / m_nbFrames) + "ms | " + std::to_string(m_nbFrames) + " FPS";
        glfwSetWindowTitle(m_window, title.c_str());
        m_nbFrames = 0;
        m_lastTime += 1.0;
    }
}
