//
//  buffer.cpp
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#include "vbo.h"

VBO::VBO(GLenum target, GLenum usage) :
target(target),
bufferSize(0),
usage(usage)
{
    glGenBuffers(1, &bufferID);
}

VBO::~VBO() {
    glDeleteBuffers(1, &bufferID);
}

GLsizeiptr VBO::size() {
    return bufferSize;
}

void VBO::bind() {
    glBindBuffer(target, bufferID);
}

/* Sets the contents of the buffer. Can be called more than once. */
void VBO::setData(GLfloat *data, GLsizeiptr count) {
    bind();
    GLsizeiptr minBufferSize = count * sizeof(GLfloat);
    if (minBufferSize > bufferSize) {
        // increase size of buffer
        bufferSize = minBufferSize;
        glBufferData(GL_ARRAY_BUFFER, minBufferSize, data, usage);
    } else {
        // update existing buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, minBufferSize, data);
    }
}
