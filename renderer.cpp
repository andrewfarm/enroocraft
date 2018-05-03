//
//  renderer.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h> //TODO remove
#include <math.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "renderer.h"
#include "loadshaders.h"

Renderer::Renderer() {
    camPos = glm::vec3(0.0f, 0.0f, 32.0f);
    camPitch = atan(-camPos[1] / camPos[2]);
    camYaw = atan2f(camPos[0], camPos[2]);
    updateViewMatrix();
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
//    const GLfloat g_vertex_buffer_data[] = {
//        -1.0f, -1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,
//        0.0f,  1.0f, 0.0f,
//    };
    
//    glGenBuffers(1, &vertexBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    shaderProgram = loadShaders("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl");
    u_MvpMatrixLocation = glGetUniformLocation(shaderProgram, "u_MvpMatrix");
}

void Renderer::setSize(float width, float height) {
    glViewport(0, 0, width * 2, height * 2); //TODO replace hardcoded pixel density
    projectionMatrix = glm::perspective(glm::radians(60.0f), height / width, 0.1f, 100.0f);
    updateMvpMatrix();
}

void Renderer::setWorld(World *world) {
    this->world = world;
    
    std::vector<float> vertices = world->mesh();
    const GLfloat *g_vertex_buffer_data = &vertices[0];
    numVertices = (GLuint) vertices.size();
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), g_vertex_buffer_data, GL_STATIC_DRAW);
}

float Renderer::getCamX() { return camPos[0]; }
float Renderer::getCamY() { return camPos[1]; }
float Renderer::getCamZ() { return camPos[2]; }
float Renderer::getCamPitch() { return camPitch; }
float Renderer::getCamYaw() { return camYaw; }
void Renderer::setCamPos(float x, float y, float z) { camPos = glm::vec3(x, y, z); }
void Renderer::setCamPitch(float pitch) { camPitch = pitch; }
void Renderer::setCamYaw(float yaw) { camYaw = yaw; }

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(u_MvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (0 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glDisableVertexAttribArray(0);
}

void Renderer::updateViewMatrix() {
    viewMatrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(), -camPitch, glm::vec3(1.0f, 0.0f, 0.0f)), -camYaw, glm::vec3(0.0f, 1.0f, 0.0f)), -camPos);
    updateMvpMatrix();
}

void Renderer::updateMvpMatrix() {
    mvpMatrix = projectionMatrix * viewMatrix;
}
