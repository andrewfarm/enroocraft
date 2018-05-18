//
//  vertexarray.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "vao.h"

VAO::VAO(const VertexAttrib attribList[], GLuint attribCount) :
attribCount(attribCount) {
    attribs = new VertexAttrib[attribCount];
    vertexComponents = 0;
    for (GLuint i = 0; i < attribCount; i++) {
        const VertexAttrib &attrib = attribList[i];
        attribs[i] = attrib;
        vertexComponents += attrib.components;
    }
    stride = vertexComponents * sizeof(vertex_component_t);
    glGenVertexArrays(1, &vaoID);
}

VAO::~VAO() {
    delete [] attribs;
    glDeleteVertexArrays(1, &vaoID);
}

GLsizei VAO::getVertexComponents() {
    return vertexComponents;
}

GLsizei VAO::getStride() {
    return stride;
}

void VAO::bind() {
    glBindVertexArray(vaoID);
}

void VAO::setPointers() {
    bind();
    for (GLuint i = 0; i < attribCount; i++) {
        VertexAttrib &attrib = attribs[i];
        glVertexAttribPointer(attrib.location, attrib.components, GL_FLOAT,
                GL_FALSE, stride, attrib.pointer);
        glEnableVertexAttribArray(attrib.location);
    }
}
