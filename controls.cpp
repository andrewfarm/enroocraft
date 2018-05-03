//
//  controls.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/3/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <math.h>

#include "controls.h"

#define LOOK_SPEED 0.1f
#define MOVE_SPEED 10.0f

void updateControls(
        GLFWwindow *window,
        Renderer &renderer,
        double *prevMouseX,
        double *prevMouseY,
        double deltaTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    renderer.setCamYaw(renderer.getCamYaw() +
        (LOOK_SPEED * deltaTime * float(*prevMouseX - mouseX)));
    renderer.setCamPitch(renderer.getCamPitch() +
        (LOOK_SPEED * deltaTime * float(*prevMouseY - mouseY)));
    *prevMouseX = mouseX;
    *prevMouseY = mouseY;

    int directionX = 0, directionY = 0, directionZ = 0;
    if ((glfwGetKey(window, GLFW_KEY_A)     == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS)) {
        directionX -= 1;
    }
    if ((glfwGetKey(window, GLFW_KEY_D)     == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
        directionX += 1;
    }
    if ((glfwGetKey(window, GLFW_KEY_W)     == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS)) {
        directionZ -= 1;
    }
    if ((glfwGetKey(window, GLFW_KEY_S)     == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS)) {
        directionZ += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        directionY -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE)  == GLFW_PRESS) {
        directionY += 1;
    }
    float moveX = MOVE_SPEED * deltaTime * directionX;
    float moveY = MOVE_SPEED * deltaTime * directionY;
    float moveZ = MOVE_SPEED * deltaTime * directionZ;
    float camYaw = renderer.getCamYaw();
    float sinYaw = sinf(camYaw);
    float cosYaw = cosf(camYaw);
    renderer.setCamPos(
        renderer.getCamX() + (moveX * cosYaw) + (moveZ * sinYaw),
        renderer.getCamY() + moveY,
        renderer.getCamZ() + (moveX * -sinYaw) + (moveZ * cosYaw));

    renderer.updateViewMatrix();
}
