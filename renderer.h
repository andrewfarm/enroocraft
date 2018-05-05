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

class Renderer {
    World *world;
    
    int width, height;
    
    glm::vec3 camPos;
    float camPitch, camYaw;
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 mvpMatrix;
    
    GLuint blockShaderProgram;
    GLint u_MvpMatrixLocation;
    GLint u_TextureLocation;
    
    GLuint screenShaderProgram;
    GLint u_ColorTextureLocation;
    GLint u_DepthTextureLocation;
    
    GLuint worldMeshVertexArray;
    GLuint worldMeshVertexBuffer;
    GLuint numVertices;
    GLuint screenVertexArray;
    GLuint screenVertexBuffer;
    
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
