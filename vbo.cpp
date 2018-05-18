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
    glGenBuffers(1, &vboID);
}

VBO::~VBO() {
    glDeleteBuffers(1, &vboID);
}

GLsizeiptr VBO::size() {
    return bufferSize;
}

void VBO::bind() {
    glBindBuffer(target, vboID);
}

/* Sets the contents of the buffer. Can be called more than once. */
void VBO::setData(const GLvoid *data, GLsizeiptr size) {
    bind();
    if (size > bufferSize) {
        // increase size of buffer
        bufferSize = size;
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    } else {
        // update existing buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}
