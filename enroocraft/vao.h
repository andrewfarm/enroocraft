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

typedef float vertex_component_t;

struct VertexAttrib {
    GLvoid *pointer;
    GLuint location;
    GLint components;
    
    VertexAttrib() {}
    
    VertexAttrib(GLuint location, GLint components, GLsizeiptr offset) :
    location(location),
    components(components),
    pointer((GLvoid *) (offset * sizeof(vertex_component_t)))
    {}
};

class VAO {
    GLuint vaoID;
    VertexAttrib *attribs;
    GLuint attribCount;
    GLsizei vertexComponents;
    GLsizei stride;
    
public:
    VAO(const VertexAttrib attribList[], GLuint attribCount);
    ~VAO();
    VAO(const VAO &) = delete; // no copy constructor
    VAO &operator=(const VAO &) = delete; // no assignment operator
    
    GLsizei getVertexComponents();
    GLsizei getStride();
    
    void bind();
    
    void setPointers();
};

#endif /* vao_h */
