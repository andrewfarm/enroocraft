//
//  vertexarray.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "vao.h"

VAO::VAO(VertexAttrib *attribList, GLuint attribCount) :
attribCount(attribCount) {
    attribs = new VertexAttrib[attribCount];
    vertexComponents = 0;
    for (GLuint i = 0; i < attribCount; i++) {
        VertexAttrib &attrib = attribList[i];
        attribs[i] = attrib;
        vertexComponents += attrib.components;
    }
    stride = vertexComponents * sizeof(float);
    glGenVertexArrays(1, &vertexArrayID);
}

VAO::~VAO() {
    delete [] attribs;
    glDeleteVertexArrays(1, &vertexArrayID);
}

GLsizei VAO::getVertexComponents() {
    return vertexComponents;
}

GLsizei VAO::getStride() {
    return stride;
}

void VAO::setPointers() {
    for (GLuint i = 0; i < attribCount; i++) {
        VertexAttrib &attrib = attribs[i];
        glVertexAttribPointer(attrib.location, attrib.components, GL_FLOAT,
                GL_FALSE, stride, attrib.pointer);
        glEnableVertexAttribArray(attrib.location);
    }
}
