//
//  vao
//  enroocraft
//
//  Created by Andrew Farm on 5/17/18.
//  Copyright © 2018 Andrew Farm. All rights reserved.
//

#ifndef vao_h
#define vao_h

#include <GL/glew.h>

struct VertexAttrib {
    GLuint location;
    GLint components;
    GLvoid *pointer;
    VertexAttrib() {}
    VertexAttrib(GLuint location, GLint components, GLsizeiptr offset) :
    location(location),
    components(components),
    pointer((GLvoid *) offset)
    {}
};

class VAO {
    GLuint vertexArrayID;
    VertexAttrib *attribs;
    GLuint attribCount;
    GLsizei vertexComponents;
    GLsizei stride;
    
public:
    VAO(VertexAttrib *attribList, GLuint attribCount);
    ~VAO();
    VAO(const VAO &) = delete; // no copy constructor
    VAO &operator=(const VAO &) = delete; // no assignment operator
    GLsizei getVertexComponents();
    GLsizei getStride();
    void setPointers();
};

#endif /* vao_h */
