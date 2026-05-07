#pragma once
#include <GLFW/glfw3.h>
#include "Camera.h"

class InputHandler {
public:
    Camera& camera;
    float lastX = 400, lastY = 300;
    bool firstMouse = true;

    InputHandler(Camera& cam) : camera(cam) {}

    void handleMouse(double xpos, double ypos) {
        float x = float(xpos);
        float y = float(ypos);

        if (firstMouse) {
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        float xoffset = x - lastX;
        float yoffset = lastY - y;
        lastX = x;
        lastY = y;

        xoffset *= camera.sensitivity;
        yoffset *= camera.sensitivity;

        camera.rotateYaw(-xoffset);
        camera.rotatePitch(yoffset);
    }

    void handleKeyboard(GLFWwindow* window, float dt) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float velocity = camera.speed * dt;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.position += camera.front() * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.position -= camera.front() * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.position -= camera.right() * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.position += camera.right() * velocity;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.rotateRoll(-camera.rollSpeed * dt);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.rotateRoll(camera.rollSpeed * dt);
    }
};
