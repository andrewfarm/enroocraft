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
    GLuint bufferID;
    GLsizeiptr bufferSize;
    GLenum usage;
    
public:
    VBO(GLenum target, GLenum usage);
    ~VBO();
    VBO(const VBO &) = delete; // no copy constructor
    VBO &operator=(const VBO &) = delete; // no assignment operator
    GLsizeiptr size();
    void bind();
    void setData(GLfloat *data, GLsizeiptr count);
};

#endif /* vbo_h */
