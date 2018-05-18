//
//  mesh.h
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include <GL/glew.h>

#include "vao.h"
#include "vbo.h"

class Mesh {
    VAO vao;
    VBO vbo;
    GLsizei vertexCount;
    GLenum drawMode;
    
public:
    Mesh(const VertexAttrib attribList[],
         GLuint attribCount,
         GLenum usage,
         GLenum drawMode);
    Mesh(const Mesh &) = delete; // no copy constructor
    Mesh &operator=(const Mesh &) = delete; // no assignment operator
    
    GLsizei getVertexCount();
    GLenum getDrawMode();
    
    void bindVAO();
    void bindVBO();
    
    /* Sets the data for the mesh. Can be called more than once. */
    void setData(const float *data, GLsizeiptr count);
    
    /* Draws the mesh. It is undefined to call draw() before setData(). */
    void draw();
};

#endif /* mesh_h */
