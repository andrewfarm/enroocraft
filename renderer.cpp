//
//  renderer.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include <stdio.h>
#include <math.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <glm/gtx/string_cast.hpp> //TODO remove

#include "renderer.h"
#include "loadshaders.h"

Renderer::Renderer() {
    camPos = glm::vec3(0.0f, 0.0f, 5.0f);
    camPitch = atan(-camPos[1] / camPos[2]);
    camYaw = atan2f(camPos[0], camPos[2]);
    updateViewMatrix();
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    shaderProgram = loadShaders("shaders/vertexshader.glsl", "shaders/fragmentshader.glsl");
    u_MvpMatrixLocation = glGetUniformLocation(shaderProgram, "u_MvpMatrix");
}

void Renderer::setSize(float width, float height) {
    projectionMatrix = glm::perspective(glm::radians(75.0f), height / width, 0.1f, 100.0f);
    updateMvpMatrix();
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(u_MvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void Renderer::updateViewMatrix() {
    viewMatrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(), camYaw, glm::vec3(0.0f, 1.0f, 0.0f)), camPitch, glm::vec3(1.0f, 0.0f, 0.0f)), -camPos);
    updateMvpMatrix();
}

void Renderer::updateMvpMatrix() {
    mvpMatrix = projectionMatrix * viewMatrix * glm::mat4(); //TODO remove identity matrix
}
