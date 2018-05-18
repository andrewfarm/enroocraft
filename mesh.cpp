//
//  mesh.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "mesh.h"

Mesh::Mesh(const VertexAttrib attribList[], GLuint attribCount, GLenum usage, GLenum drawMode) :
vao(attribList, attribCount),
vbo(GL_ARRAY_BUFFER, usage),
vertexCount(0),
drawMode(drawMode)
{}

/* Sets the data for the mesh. Can be called more than once. */
void Mesh::setData(float *data, GLsizeiptr count) {
    vertexCount = (GLsizei) count / vao.getVertexComponents();
    vbo.setData(data, count * sizeof(data[0]));
    vbo.bind();
    vao.setPointers();
}

void Mesh::draw() {
    vao.bind();
    glDrawArrays(drawMode, 0, vertexCount);
}
