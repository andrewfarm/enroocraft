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

static const float crosshairGeometry[] = {0.0f, 0.0f};

#define CROSSHAIR_GEOMETRY_POSITION 0
#define CROSSHAIR_GEOMETRY_VERTEX_COMPONENTS 2

static const float selectionGeometry[] = {
    -0.005f,  1.005f, -0.005f,
     1.005f,  1.005f, -0.005f,
     1.005f,  1.005f,  1.005f,
    -0.005f,  1.005f,  1.005f,
    -0.005f, -0.005f, -0.005f,
     1.005f, -0.005f, -0.005f,
     1.005f, -0.005f,  1.005f,
    -0.005f, -0.005f,  1.005f,
};

static const unsigned char selectionIndices[] = {
    0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4
};

#define SELECTION_GEOMETRY_POSITION 0
#define SELECTION_GEOMETRY_VERTEX_COMPONENTS 3

Renderer::Renderer() {
    camPos = glm::vec3(0.5f, 70.0f, 0.5f);
    camPitch = 0;
    camYaw = 0;
    updateViewMatrix();
    
    drawSelectionCube = false;
    
    blockShaderProgram.load(
            "shaders/blockvertexshader.glsl",
            "shaders/blockfragmentshader.glsl");
    
    screenShaderProgram.load(
            "shaders/screenvertexshader.glsl",
            "shaders/screenfragmentshader.glsl");
    
    crosshairShaderProgram.load(
            "shaders/crosshairvertexshader.glsl",
            "shaders/crosshairfragmentshader.glsl");
    
    simpleShaderProgram.load(
            "shaders/simplevertexshader.glsl",
            "shaders/simplefragmentshader.glsl");
    
    printf("Loading texture atlas\n");
    texture = loadTexture("res/textures.png");
    
    glGenVertexArrays(1, &screenVertexArray);
    glBindVertexArray(screenVertexArray);
    
    glGenBuffers(1, &screenVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, screenVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenGeometry), screenGeometry, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, SCREEN_GEOMETRY_VERTEX_COMPONENTS * sizeof(float), (void *) (SCREEN_GEOMETRY_POSITION * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, SCREEN_GEOMETRY_VERTEX_COMPONENTS * sizeof(float), (void *) (SCREEN_GEOMETRY_UV * sizeof(float)));
    
    glGenVertexArrays(1, &crosshairVertexArray);
    glBindVertexArray(crosshairVertexArray);
    
    glGenBuffers(1, &crosshairVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairGeometry), crosshairGeometry, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, CROSSHAIR_GEOMETRY_VERTEX_COMPONENTS * sizeof(float), (void *) (CROSSHAIR_GEOMETRY_POSITION * sizeof(float)));
    
    glGenVertexArrays(1, &selectionVertexArray);
    glBindVertexArray(selectionVertexArray);
    
    glGenBuffers(1, &selectionVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, selectionVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(selectionGeometry), selectionGeometry, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SELECTION_GEOMETRY_VERTEX_COMPONENTS * sizeof(float), (void *) (SELECTION_GEOMETRY_POSITION * sizeof(float)));
    
    glGenBuffers(1, &selectionIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectionIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(selectionIndices), selectionIndices, GL_STATIC_DRAW);
}

void Renderer::setSize(float width, float height) {
    width  *= 2; //TODO replace hardcoded pixel density
    height *= 2;
    this->width  = width;
    this->height = height;
    
    projectionMatrix = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 1000.0f);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  renderedDepthTexture, 0);
    
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    
    // Always check that our framebuffer is ok
    // are you ok, framebuffer?
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer creation successful\n");
    } else {
        printf("Error creating framebuffer (status: %d)\n", status);
    }
}

void loadMesh(int prevVertexCount, std::vector<float> &meshData, mesh &mesh) {
    GLsizei meshSize = (GLsizei) meshData.size();
    GLsizei vertexCount = meshSize / FACE_GEOMETRY_STRIDE;
    mesh.vertexCount = vertexCount;
    
    glBindVertexArray(mesh.vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferID);
    if (vertexCount > prevVertexCount) {
        // increase size of buffer
        glBufferData(GL_ARRAY_BUFFER, meshSize * sizeof(meshData[0]), &meshData[0], GL_DYNAMIC_DRAW);
    } else {
        // update existing buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, meshSize * sizeof(meshData[0]), &meshData[0]);
    }
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            FACE_GEOMETRY_STRIDE * sizeof(meshData[0]),
            (void *) (FACE_GEOMETRY_POSITION * sizeof(meshData[0])));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            FACE_GEOMETRY_STRIDE * sizeof(meshData[0]),
            (void *) (FACE_GEOMETRY_NORMAL * sizeof(meshData[0])));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            FACE_GEOMETRY_STRIDE * sizeof(meshData[0]),
            (void *) (FACE_GEOMETRY_UV * sizeof(meshData[0])));
}

void Renderer::loadChunkMesh(int chunkX, int chunkZ, const std::vector<blocktype> &blocks) {
    // get pointer to mesh struct, and create new one if necessary
    const auto chunkMeshEntry = chunkMeshes.find(std::pair<int, int>(chunkX, chunkZ));
    chunkMesh *p_chunkMesh;
    int prevOpaqueVertexCount = -1;
    int prevTransparentVertexCount = -1;
    if (chunkMeshEntry != chunkMeshes.end()) {
        // mesh for this chunk already exists
        p_chunkMesh = &(chunkMeshEntry->second);
        prevOpaqueVertexCount = p_chunkMesh->opaqueMesh.vertexCount;
        prevTransparentVertexCount = p_chunkMesh->transparentMesh.vertexCount;
    } else {
        // create new mesh entry along with vertex array and buffer
        chunkMesh temp;
        std::pair<int, int> key(chunkX, chunkZ);
        chunkMeshes.insert(std::make_pair(key, temp));
        p_chunkMesh = &(chunkMeshes.find(key)->second);
        glGenVertexArrays(1, &(p_chunkMesh->opaqueMesh.vertexArrayID));
        glGenVertexArrays(1, &(p_chunkMesh->transparentMesh.vertexArrayID));
        glGenBuffers(1, &(p_chunkMesh->opaqueMesh.bufferID));
        glGenBuffers(1, &(p_chunkMesh->transparentMesh.bufferID));
    }
    
    // generate mesh
    std::vector<float> opaqueMeshData;
    std::vector<float> transparentMeshData;
    world->mesh(opaqueMeshData, transparentMeshData, chunkX, chunkZ, blocks);
    
    // send vertex data to OpenGL
    loadMesh(prevOpaqueVertexCount, opaqueMeshData, p_chunkMesh->opaqueMesh);
    loadMesh(prevTransparentVertexCount, transparentMeshData, p_chunkMesh->transparentMesh);
}

void Renderer::updateMesh(int chunkX, int chunkZ) {
    const auto chunks = world->getChunks();
    const auto &chunkEntry = chunks->find(std::pair<int, int>(chunkX, chunkZ));
    if (chunkEntry != chunks->end()) {
        loadChunkMesh(chunkX, chunkZ, chunkEntry->second);
    }
}

void Renderer::updateMesh(int x, int y, int z) {
    int chunkX = (int) floor((float) x / CHUNK_SIZE);
    int chunkZ = (int) floor((float) z / CHUNK_SIZE);
    updateMesh(chunkX, chunkZ);
    
    //update adjacent chunks
    int internalX = x - (chunkX * CHUNK_SIZE);
    int internalZ = z - (chunkZ * CHUNK_SIZE);
    if (internalX == 0) {
        updateMesh(chunkX - 1, chunkZ);
    } else if (internalX == CHUNK_SIZE - 1) {
        updateMesh(chunkX + 1, chunkZ);
    }
    if (internalZ == 0) {
        updateMesh(chunkX, chunkZ - 1);
    } else if (internalZ == CHUNK_SIZE - 1) {
        updateMesh(chunkX, chunkZ + 1);
    }
}

void Renderer::setWorld(World *world) {
    this->world = world;
    
    const auto chunks = world->getChunks();
    const auto chunksEnd = chunks->end();
    for (auto chunkEntry = chunks->begin(); chunkEntry != chunksEnd; ++chunkEntry) {
        loadChunkMesh(chunkEntry->first.first, chunkEntry->first.second, chunkEntry->second);
    }
}

float Renderer::getCamX() { return camPos[0]; }
float Renderer::getCamY() { return camPos[1]; }
float Renderer::getCamZ() { return camPos[2]; }
float Renderer::getCamPitch() { return camPitch; }
float Renderer::getCamYaw() { return camYaw; }
void Renderer::setCamPos(float x, float y, float z) { camPos = glm::vec3(x, y, z); }
void Renderer::setCamPitch(float pitch) { camPitch = pitch; }
void Renderer::setCamYaw(float yaw) { camYaw = yaw; }

void Renderer::setDrawSelectionCube(bool drawSelectionCube) {
    this->drawSelectionCube = drawSelectionCube;
}

void Renderer::setSelectedBlock(int x, int y, int z) {
    selectionModelMatrix = glm::translate(glm::mat4(), glm::vec3((float) x, (float) y, (float) z));
}

void Renderer::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    blockShaderProgram.useProgram();
    glUniformMatrix4fv(blockShaderProgram.uniforms["u_MvpMatrix"], 1, GL_FALSE, &mvpMatrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(blockShaderProgram.uniforms["u_Texture"], 0);
    
    for (auto &chunkMeshEntry : chunkMeshes) {
        chunkMesh &chunkMesh = chunkMeshEntry.second;
        glBindVertexArray(chunkMesh.opaqueMesh.vertexArrayID);
        glDrawArrays(GL_TRIANGLES, 0, chunkMesh.opaqueMesh.vertexCount);
    }
    
    glDepthMask(GL_FALSE);
    for (auto &chunkMeshEntry : chunkMeshes) {
        chunkMesh &chunkMesh = chunkMeshEntry.second;
        glBindVertexArray(chunkMesh.transparentMesh.vertexArrayID);
        glDrawArrays(GL_TRIANGLES, 0, chunkMesh.transparentMesh.vertexCount);
    }
    glDepthMask(GL_TRUE);
    
    if (drawSelectionCube) {
        simpleShaderProgram.useProgram();
        glUniformMatrix4fv(simpleShaderProgram.uniforms["u_MvpMatrix"], 1, GL_FALSE,
                &(mvpMatrix * selectionModelMatrix)[0][0]);
        glUniform3f(simpleShaderProgram.uniforms["u_Color"], 0.0f, 0.0f, 0.0f);
        
        glBindVertexArray(selectionVertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectionIndexBuffer);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, (void *) 0);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    screenShaderProgram.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedColorTexture);
    glUniform1i(blockShaderProgram.uniforms["u_ColorTexture"], 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderedDepthTexture);
    glUniform1i(blockShaderProgram.uniforms["u_DepthTexture"], 1);
    
    glBindVertexArray(screenVertexArray);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
    
    crosshairShaderProgram.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(crosshairShaderProgram.uniforms["u_Texture"], 0);
    glUniform2f(crosshairShaderProgram.uniforms["u_StartUV"], 0.0f, 0.0f);
    glUniform1f(crosshairShaderProgram.uniforms["u_TextureSize"], TEXTURE_ATLAS_SIZE_RECIPROCAL);
    
    glBindVertexArray(crosshairVertexArray);
    glDrawArrays(GL_POINTS, 0, 1);
}

void Renderer::updateViewMatrix() {
    viewMatrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(), -camPitch, glm::vec3(1.0f, 0.0f, 0.0f)), -camYaw, glm::vec3(0.0f, 1.0f, 0.0f)), -camPos);
    updateMvpMatrix();
}

void Renderer::updateMvpMatrix() {
    mvpMatrix = projectionMatrix * viewMatrix;
}
