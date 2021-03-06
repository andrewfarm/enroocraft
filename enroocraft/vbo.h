//
//  buffer.h
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef vbo_h
#define vbo_h

#include <GL/glew.h>

class VBO {
    GLenum target;
    GLuint vboID;
    GLsizeiptr bufferSize;
    GLenum usage;
    
public:
    VBO(GLenum target, GLenum usage);
    ~VBO();
    VBO(const VBO &) = delete; // no copy constructor
    VBO &operator=(const VBO &) = delete; // no assignment operator
    
    GLsizeiptr size();
    
    void bind();
    
    /* Sets the contents of the buffer. Can be called more than once. */
    void setData(const GLvoid *data, GLsizeiptr size);
};

#endif /* vbo_h */
