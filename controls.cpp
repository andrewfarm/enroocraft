//
//  controls.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/3/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "controls.h"

#define LOOK_SPEED 0.1f
#define MOVE_ACC 200.0f

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

Controls::Controls(GLFWwindow *window, Renderer *renderer, World *world, Player *player) :
window(window),
renderer(renderer),
world(world),
player(player),
canBreakBlock(true),
canPlaceBlock(true),
escapeKeyDown(false),
paused(false)
{
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool Controls::arePaused() {
    return paused;
}

void Controls::update(double deltaTime) {
    int newWindowWidth, newWindowHeight;
    glfwGetWindowSize(window, &newWindowWidth, &newWindowHeight);
    if ((newWindowWidth != windowWidth) || (newWindowHeight != windowHeight)) {
        windowWidth = newWindowWidth;
        windowHeight = newWindowHeight;
        renderer->setSize(newWindowWidth, newWindowHeight);
    }
    
    int escapeStatus = glfwGetKey(window, GLFW_KEY_ESCAPE);
    if (!escapeKeyDown && (escapeStatus == GLFW_PRESS)) {
        escapeKeyDown = true;
        paused = !paused;
        glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        prevMouseX = windowWidth / 2;
        prevMouseY = windowHeight / 2;
        glfwSetCursorPos(window, prevMouseX, prevMouseY);
    } else if (escapeKeyDown && (escapeStatus == GLFW_RELEASE)) {
        escapeKeyDown = false;
    }
    
    if (paused) {
        return;
    }
    
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    float newYaw = player->getLookYaw() +
            (LOOK_SPEED * deltaTime * float(prevMouseX - mouseX));
    newYaw = fmodNeg(newYaw, TWO_PI);
    
    float newPitch = player->getLookPitch() +
            (LOOK_SPEED * deltaTime * float(prevMouseY - mouseY));
    if (newPitch > HALF_PI) {
        newPitch = HALF_PI;
    } else if (newPitch < -HALF_PI) {
        newPitch = -HALF_PI;
    }
    
    player->setLookYaw(newYaw);
    player->setLookPitch(newPitch);
    prevMouseX = mouseX;
    prevMouseY = mouseY;
    
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
    glm::vec3 move = MOVE_ACC * (float) deltaTime * glm::vec3(directionX, directionY, directionZ);
    float camYaw = player->getLookYaw();
    move = glm::rotate(move, camYaw, glm::vec3(0.0f, 1.0f, 0.0f));
    player->acc(move);
    
    renderer->setCamPos(player->getPos());
    renderer->setCamYaw(player->getLookYaw());
    renderer->setCamPitch(player->getLookPitch());
    
    renderer->updateViewMatrix();
    
    // ray cast
    
    float camX = renderer->getCamX();
    float camY = renderer->getCamY();
    float camZ = renderer->getCamZ();
    float camPitch = renderer->getCamPitch();
    
    int initX = (int) floorf(camX); // if we don't use floor, negative values will round up
    int initY = (int) floorf(camY);
    int initZ = (int) floorf(camZ);
    
    int x = initX;
    int y = initY;
    int z = initZ;
    
    int prevX;
    int prevY;
    int prevZ;
    
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
    
    blocktype intersectedBlock = world->getBlock(x, y, z);
    while (!isSolid(intersectedBlock) &&
           (distSq(initX, initY, initZ, x, y, z) <
                    (MAX_SELECT_DISTANCE * MAX_SELECT_DISTANCE))) {
        prevX = x;
        prevY = y;
        prevZ = z;
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
        intersectedBlock = world->getBlock(x, y, z);
    }
    if (isSolid(intersectedBlock)) {
        renderer->setDrawSelectionCube(true);
        renderer->setSelectedBlock(x, y, z);
    } else {
        renderer->setDrawSelectionCube(false);
    }
    
    int leftMouseButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (leftMouseButtonStatus == GLFW_PRESS) {
        if (canBreakBlock && isSolid(intersectedBlock)) {
            world->setBlock(x, y, z, BLOCK_AIR);
            renderer->updateMesh(x, y, z);
        }
        canBreakBlock = false;
    } else if (leftMouseButtonStatus == GLFW_RELEASE) {
        canBreakBlock = true;
    }
    
    int rightMouseButtonStatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (rightMouseButtonStatus == GLFW_PRESS) {
        if (canPlaceBlock && isSolid(intersectedBlock)) {
            world->setBlock(prevX, prevY, prevZ, BLOCK_WOOD);
            renderer->updateMesh(prevX, prevY, prevZ);
        }
        canPlaceBlock = false;
    } else if (rightMouseButtonStatus == GLFW_RELEASE) {
        canPlaceBlock = true;
    }
}
