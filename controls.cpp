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

#define MAX_SELECT_DISTANCE 5.0f

const float HALF_PI = (float) (M_PI / 2);
const float TWO_PI  = (float) (M_PI * 2);

static int distSq(int ax, int ay, int az, int bx, int by, int bz) {
    int distX = bx - ax;
    int distY = by - ay;
    int distZ = bz - az;
    return (distX * distX) + (distY * distY) + (distZ * distZ);
}

static inline float fmodNeg(float a, float b) {
    return fmod(fmod(a, b) + b, b);
}

void updateControls(
        GLFWwindow *window,
        Renderer &renderer,
        World &world,
        double *prevMouseX,
        double *prevMouseY,
        double deltaTime) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    float newYaw = renderer.getCamYaw() +
            (LOOK_SPEED * deltaTime * float(*prevMouseX - mouseX));
    newYaw = fmodNeg(newYaw, TWO_PI);
    
    float newPitch = renderer.getCamPitch() +
            (LOOK_SPEED * deltaTime * float(*prevMouseY - mouseY));
    if (newPitch > HALF_PI) {
        newPitch = HALF_PI;
    } else if (newPitch < -HALF_PI) {
        newPitch = -HALF_PI;
    }
    
    renderer.setCamYaw(newYaw);
    renderer.setCamPitch(newPitch);
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
    
    renderer.setDrawSelectionCube(true);
    renderer.setSelectedBlock(0, 63, -5);
    
    // ray cast
    
    float camX = renderer.getCamX();
    float camY = renderer.getCamY();
    float camZ = renderer.getCamZ();
    float camPitch = renderer.getCamPitch();
    
    int initX = (int) floorf(camX); // if we don't use floor, negative values will round up
    int initY = (int) floorf(camY);
    int initZ = (int) floorf(camZ);
    
    int x = initX;
    int y = initY;
    int z = initZ;
    
    int stepX = (camYaw < M_PI) ? -1 : 1;
    int stepY = (camPitch < 0) ? -1 : 1;
    int stepZ = ((camYaw < HALF_PI) || (camYaw > (3 * HALF_PI))) ? -1 : 1;
    
    float tDeltaX = abs(1.0f / (cosf(camPitch) * sinf(camYaw)));
    float tDeltaY = abs(1.0f / sinf(camPitch));
    float tDeltaZ = abs(1.0f / (cosf(camPitch) * cosf(camYaw)));
    
    float tMaxX = (stepX < 0) ? (camX - x) : (x + 1 - camX);
    float tMaxY = (stepY < 0) ? (camY - y) : (y + 1 - camY);
    float tMaxZ = (stepZ < 0) ? (camZ - z) : (z + 1 - camZ);
    
    tMaxX *= tDeltaX;
    tMaxY *= tDeltaY;
    tMaxZ *= tDeltaZ;
    
    blocktype intersectedBlock = world.getBlock(x, y, z);
    while ((intersectedBlock <= BLOCK_AIR) &&
           (distSq(initX, initY, initZ, x, y, z) <
                    (MAX_SELECT_DISTANCE * MAX_SELECT_DISTANCE))) {
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                x += stepX;
                tMaxX += tDeltaX;
            } else {
                z += stepZ;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                y += stepY;
                tMaxY += tDeltaY;
            } else {
                z += stepZ;
                tMaxZ += tDeltaZ;
            }
        }
        intersectedBlock = world.getBlock(x, y, z);
    }
    if (intersectedBlock > BLOCK_AIR) {
        renderer.setDrawSelectionCube(true);
        renderer.setSelectedBlock(x, y, z);
    } else {
        renderer.setDrawSelectionCube(false);
    }
}
