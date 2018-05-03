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
#include <OpenGL/gl.h>
#include <glm/glm.hpp>

class Renderer {
    glm::vec3 camPos;
    float camPitch, camYaw;
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 mvpMatrix;
    
    GLuint shaderProgram;
    GLuint u_MvpMatrixLocation;
    
    GLuint vertexArrayID;
    GLuint vertexBuffer;
    
    void updateViewMatrix();
    void updateMvpMatrix();
    
public:
    Renderer();
    void setSize(float width, float height);
    void render();
};

#endif /* renderer_h */
