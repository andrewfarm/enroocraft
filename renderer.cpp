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
#include "shaderutils.h"
#include "textureutils.h"

static const float screenGeometry[] = {
     1.0f,  1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,   0.0f, 1.0f,
     1.0f, -1.0f,   1.0f, 0.0f,
    -1.0f, -1.0f,   0.0f, 0.0f,
};

#define SCREEN_GEOMETRY_POSITION 0
#define SCREEN_GEOMETRY_UV 2
#define SCREEN_GEOMETRY_VERTEX_COMPONENTS 4

Renderer::Renderer() {
    camPos = glm::vec3(0.5f, 70.0f, 0.5f);
    camPitch = 0;
    camYaw = 0;
    updateViewMatrix();
    
    blockShaderProgram = loadShaders(
            "shaders/blockvertexshader.glsl",
            "shaders/blockfragmentshader.glsl");
    u_MvpMatrixLocation = glGetUniformLocation(blockShaderProgram, "u_MvpMatrix");
    u_TextureLocation   = glGetUniformLocation(blockShaderProgram, "u_Texture");
    
    screenShaderProgram = loadShaders(
            "shaders/screenvertexshader.glsl",
            "shaders/screenfragmentshader.glsl");
    u_ColorTextureLocation = glGetUniformLocation(screenShaderProgram, "u_ColorTexture");
    u_DepthTextureLocation = glGetUniformLocation(screenShaderProgram, "u-DepthTexture");
    
    printf("Loading texture atlas\n");
    texture = loadTexture("res/textures.png");
    
    glGenVertexArrays(1, &worldMeshVertexArray);
    glGenVertexArrays(1, &screenVertexArray);
    glBindVertexArray(screenVertexArray);
    
    glGenBuffers(1, &screenVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, screenVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenGeometry), screenGeometry, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, SCREEN_GEOMETRY_VERTEX_COMPONENTS * sizeof(float), (void *) (SCREEN_GEOMETRY_POSITION * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SCREEN_GEOMETRY_VERTEX_COMPONENTS * sizeof(float), (void *) (SCREEN_GEOMETRY_UV * sizeof(float)));
}

void Renderer::setSize(float width, float height) {
    width  *= 2; //TODO replace hardcoded pixel density
    height *= 2;
    this->width  = width;
    this->height = height;
    
    projectionMatrix = glm::perspective(glm::radians(60.0f), height / width, 0.1f, 1000.0f);
    updateMvpMatrix();
    
    printf("Creating textured framebuffer\n");
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glGenTextures(1, &renderedColorTexture);
    glBindTexture(GL_TEXTURE_2D, renderedColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedColorTexture, 0);
    
    glGenTextures(1, &renderedDepthTexture);
    glBindTexture(GL_TEXTURE_2D, renderedDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  renderedDepthTexture, 0);
    
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    
    // Always check that our framebuffer is ok
    // framebuffer, are you ok?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer creation successful\n");
    } else {
        printf("Error creating framebuffer (status: %d)\n", status);
    }
}

void Renderer::setWorld(World *world) {
    this->world = world;
    
    std::vector<float> vertices = world->mesh();
    const GLfloat *g_vertex_buffer_data = &vertices[0];
    numVertices = (GLuint) vertices.size();
    
    glBindVertexArray(worldMeshVertexArray);
    
    glGenBuffers(1, &worldMeshVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, worldMeshVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          FACE_GEOMETRY_STRIDE * sizeof(float),
                          (void *) (FACE_GEOMETRY_POSITION * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          FACE_GEOMETRY_STRIDE * sizeof(float),
                          (void *) (FACE_GEOMETRY_NORMAL * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          FACE_GEOMETRY_STRIDE * sizeof(float),
                          (void *) (FACE_GEOMETRY_UV * sizeof(float)));
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    glClearColor(0.8f, 0.95f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glUseProgram(blockShaderProgram);
    glUniformMatrix4fv(u_MvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(u_TextureLocation, 0);

    glBindVertexArray(worldMeshVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    glUseProgram(screenShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedColorTexture);
    glUniform1i(u_ColorTextureLocation, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderedDepthTexture);
    glUniform1i(u_DepthTextureLocation, 1);
    
    glBindVertexArray(screenVertexArray);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::updateViewMatrix() {
    viewMatrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(), -camPitch, glm::vec3(1.0f, 0.0f, 0.0f)), -camYaw, glm::vec3(0.0f, 1.0f, 0.0f)), -camPos);
    updateMvpMatrix();
}

void Renderer::updateMvpMatrix() {
    mvpMatrix = projectionMatrix * viewMatrix;
}
