//
//  renderer.h
//  enroocraft
//
//  Created by Andrew Farm on 5/2/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef renderer_h
#define renderer_h

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "world.h"
#include "shaderprogram.h"

class Renderer {
    World *world;
    
    int width, height;
    
    glm::vec3 camPos;
    float camPitch, camYaw;
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 mvpMatrix;
    
    glm::mat4 selectionModelMatrix;
    
    ShaderProgram blockShaderProgram;
    ShaderProgram screenShaderProgram;
    ShaderProgram crosshairShaderProgram;
    ShaderProgram simpleShaderProgram;
    
    GLuint worldMeshVertexArray;
    GLuint worldMeshVertexBuffer;
    GLuint numVertices;
    GLuint screenVertexArray;
    GLuint screenVertexBuffer;
    GLuint crosshairVertexArray;
    GLuint crosshairVertexBuffer;
    GLuint selectionVertexArray;
    GLuint selectionVertexBuffer;
    GLuint selectionIndexBuffer;
    
    GLuint texture;
    
    GLuint framebuffer;
    GLuint renderedColorTexture;
    GLuint renderedDepthTexture;
    
    void updateMvpMatrix();
    
public:
    Renderer();
    void setSize(float width, float height);
    void setWorld(World *world);
    
    float getCamX();
    float getCamY();
    float getCamZ();
    float getCamPitch();
    float getCamYaw();
    void setCamPos(float x, float y, float z);
    void setCamPitch(float pitch);
    void setCamYaw(float yaw);
    
    void updateViewMatrix();
    
    void render();
};

#endif /* renderer_h */
